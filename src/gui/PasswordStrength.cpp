/*
 *  Copyright (C) 2014 Michael Curtis
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PasswordStrength.h"

#include <QtScript>
#include <QTime>
#include <QHash>
#include <QDebug>

#include "ui_PasswordStrength.h"

#include "core/PasswordChecker.h"

class PasswordStrength::ScoreInfo
{
  public:
    ScoreInfo() {}
    ScoreInfo( QString name, QColor color = Qt::black ) : name( name ), color( color ) {}

    QString name;
    QColor color;
};

PasswordStrength::PasswordStrength( QWidget *parent ) : QWidget( parent ),
  m_ui( new Ui_PasswordStrength() )
{
  m_ui->setupUi( this );

  m_scoreTable[ 0 ] = ScoreInfo( tr( "Very poor",  "passwordRating" ), Qt::red );
  m_scoreTable[ 1 ] = ScoreInfo( tr( "Poor",       "passwordRating" ), Qt::red );
  m_scoreTable[ 2 ] = ScoreInfo( tr( "OK",         "passwordRating" ), Qt::darkYellow );
  m_scoreTable[ 3 ] = ScoreInfo( tr( "Good",       "passwordRating" ), Qt::yellow );
  m_scoreTable[ 4 ] = ScoreInfo( tr( "Very Strong","passwordRating" ), Qt::green );

  fontChanged();

  setTargetEntropy( 64 );

  // Initialise everything
  checkPassword( "" );
}

PasswordStrength::~PasswordStrength()
{

}

void PasswordStrength::changeEvent( QEvent *event )
{
  if( event->type() == QEvent::FontChange ) {
    fontChanged();
  }
}

void PasswordStrength::checkPassword( const QString & password )
{
  if( PasswordChecker::instance() && PasswordChecker::instance()->valid() ) {
    PasswordAssessment assessment = PasswordChecker::instance()->assess( password );

    m_ui->heat->setValue( assessment.entropy );
    m_ui->heat->setText( tr( "%n bit(s)", "entropy", assessment.entropy ) );

    int score = int( double(assessment.entropy) / m_targetEntropy * 4.0 );
    score = qMin( 4, qMax( 0, score ) ); 

    if( m_scoreTable.contains( score ) ) {
      m_ui->score->setText( m_scoreTable.value( score ).name );
    } else {
      m_ui->score->setText( "Unknown" );
    }

    m_ui->crackTime->setText( secondsToString( assessment.timeToCrack ) );
  } else {
    m_ui->crackTime->setText( tr( "Unknown" ) );
    m_ui->score->setText( tr( "Unknown" ) );

    m_ui->heat->setValue( 0 );
    m_ui->heat->setText( "" );
  }
}

void PasswordStrength::fontChanged()
{
  // Set the minimum width of the score label box to be the minimum of all text boxes so the layout doesn't constantly have to adjust
  int minWidth = 0;

  Q_FOREACH( ScoreInfo score, m_scoreTable ) {
    minWidth = qMax( minWidth, fontMetrics().width( score.name ) );
  }

  m_ui->score->setMinimumWidth( minWidth );

  QFont smallFont = font();
  smallFont.setPointSize( smallFont.pointSizeF() * 0.8 );

  m_ui->crackTimeLabel->setFont( smallFont );
  m_ui->crackTime->setFont( smallFont );
}

QString PasswordStrength::secondsToString( double seconds )
{
  const static double kSecondsPerHour = 3600;
  const static double kHoursPerDay = 24;
  const static double kDaysPerWeek = 7;
  const static double kDaysPerMonth = 30;// It's an estimate, so average of 30 days/month is good enough
  const static double kDaysPerYear = 365;

  double hours = seconds / kSecondsPerHour;
  double days = hours / kHoursPerDay;
  double weeks = days / kDaysPerWeek;
  double months = days / kDaysPerMonth; 
  double years = days / kDaysPerYear;

  if( hours < 0.5 ) {
    return tr( "an instant", "time" );
  } else if( days < 2 ) {
    return tr( "%n hour(s)", "time", ceil( hours + 0.5 ) );
  } else if( weeks < 2 ) {
    return tr( "%n day(s)", "time", ceil( days + 0.5 ) );
  } else if( months < 2 ) {
    return tr( "%n week(s)", "time", ceil( weeks + 0.5 ) );
  } else if( years < 2 ) {
    return tr( "%n month(s)", "time", ceil( months + 0.5 ) );
  } else if( years < 100 ) {
    return tr( "%n years(s)", "time", ceil( years + 0.5 ) );
  } else {
    return tr( "centuries" );
  }
}

unsigned int PasswordStrength::targetEntropy() const
{
  return m_targetEntropy;
}

void PasswordStrength::setTargetEntropy( unsigned int targetEntropy )
{
  m_targetEntropy = targetEntropy;
  m_ui->heat->setMaximum( targetEntropy + targetEntropy / 4 );
}
