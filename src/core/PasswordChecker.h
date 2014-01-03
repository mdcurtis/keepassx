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

#ifndef KEEPASSX_PASSWORDCHECKER_H
#define KEEPASSX_PASSWORDCHECKER_H

#include <QScopedPointer>

struct PasswordAssessment
{
    int entropy;
    double timeToCrack;
};

class QScriptEngine;
class QScriptValue;

class PasswordChecker
{
public:
  PasswordAssessment assess( const QString & );

  static PasswordChecker *instance();

  bool valid() const;

private:
  PasswordChecker();
  ~PasswordChecker();
  Q_DISABLE_COPY( PasswordChecker );

  QScopedPointer< QScriptEngine > m_scriptEngine;
  QScopedPointer< QScriptValue >  m_zxcvbn;

  bool m_valid;

  static PasswordChecker *m_instance;
};

#endif

