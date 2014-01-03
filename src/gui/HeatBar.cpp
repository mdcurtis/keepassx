#include "HeatBar.h"

#include <QPainter>
#include <QGradient>

HeatBar::HeatBar( QWidget *parent ) : QLabel( parent ), m_minimum( 0 ), m_maximum( 100 ), m_value( 0 )
{

  setFixedHeight( 24 );

  setFrameStyle( QFrame::Panel | QFrame::Sunken );

}

void HeatBar::paintEvent( QPaintEvent *event  )
{
  QPainter painter( this );

  QRect bar = rect();
  bar.adjust( lineWidth(), lineWidth(), -1 - lineWidth(), -1 - lineWidth() );

  QLinearGradient gradient( QPointF( bar.left(), 0 ), QPointF( bar.right(), 0 ) );

  gradient.setColorAt( 0,    QColor::fromHsl(   0, 248, 140 ) ); // HSL: red
  gradient.setColorAt( 0.37, QColor::fromHsl(  42, 255, 126 ) ); // HSL: mid yellow
  gradient.setColorAt( 0.75, QColor::fromHsl(  92, 199, 108 ) ); // HSL: green

  bar.setWidth( qMin( bar.width(), (m_value - m_minimum) * bar.width() / m_maximum ) );

  painter.setBrush( gradient );
  painter.setPen( Qt::NoPen );

  painter.drawRect( bar );

  painter.end();

  QLabel::paintEvent( event );

}


int HeatBar::minimum() const
{
  return m_minimum;
}

int HeatBar::maximum() const
{
  return m_maximum;
}

int HeatBar::value() const
{
  return m_value;
}

void HeatBar::setMinimum( int minimum )
{
  m_minimum = minimum;

  update();
}

void HeatBar::setMaximum( int maximum )
{
  m_maximum = maximum;

  update();
}

void HeatBar::setValue( int value )
{
  m_value = value;

  update();
}
