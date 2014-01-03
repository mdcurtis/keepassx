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

#ifndef _PASSWORDSTRENGTH_H
#define _PASSWORDSTRENGTH_H

#include <QScopedPointer>
#include <QWidget>
#include <QHash>

class PasswordChecker;
class Ui_PasswordStrength;

class PasswordStrength : public QWidget
{
  Q_OBJECT 
  
  public:
    PasswordStrength( QWidget *parent = 0 );
    ~PasswordStrength();

    unsigned int targetEntropy() const;

    void setTargetEntropy( unsigned int targetEntropy );

  public Q_SLOTS:
    void checkPassword( const QString & password );

  protected:
    void changeEvent( QEvent * );

  private:
    void fontChanged();

    QString secondsToString( double );

  private:
    QScopedPointer< Ui_PasswordStrength > m_ui;
      
    double m_targetEntropy;

    class ScoreInfo;
    QHash< int, ScoreInfo > m_scoreTable;
};

#endif
