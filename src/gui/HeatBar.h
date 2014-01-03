#ifndef HEATBAR_H
#define HEATBAR_H

#include <QLabel>

class HeatBar : public QLabel
{
  Q_OBJECT
public:
  HeatBar( QWidget *parent = 0 );

  int minimum() const;
  int maximum() const;
  int value() const;

public Q_SLOTS:
  void setMinimum( int minimum );
  void setMaximum( int maximum );

  void setValue( int value );

protected:
  void paintEvent( QPaintEvent * );

private:
  int m_minimum, m_maximum, m_value;
};

#endif
