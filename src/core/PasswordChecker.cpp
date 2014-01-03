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

#include "PasswordChecker.h"

#include <QtScript>
#include <QFile>
#include <QHash>

#include "core/Global.h"

PasswordChecker *PasswordChecker::m_instance( Q_NULLPTR );

PasswordChecker::PasswordChecker() :
  m_scriptEngine( new QScriptEngine() ),
  m_zxcvbn( new QScriptValue() ),
  m_valid( false )
{
  Q_INIT_RESOURCE( zxcvbn );

  QFile scriptFile(":zxcvbn.js");
  if (!scriptFile.open(QIODevice::ReadOnly)) {
    return;
  }

  QTextStream stream(&scriptFile);
  QString contents = stream.readAll();
  scriptFile.close();

  m_scriptEngine->evaluate( "exports = {};" );

  QScriptValue zxcvbnJS = m_scriptEngine->evaluate( contents );
  if( m_scriptEngine->hasUncaughtException() ) {
    return;
  }

  QScriptValue exports = m_scriptEngine->evaluate( "exports" );
  *m_zxcvbn = exports.property( "zxcvbn" );

  m_valid = true;
}

PasswordChecker::~PasswordChecker()
{

}

PasswordAssessment PasswordChecker::assess( const QString & password )
{
  PasswordAssessment result;
  result.entropy = 0;
  result.timeToCrack = 0;

  QScriptValueList args;
  args << password;

  QScriptValue rawResult = m_zxcvbn->call( QScriptValue(), args );

  QHash< QString, QString > hashedResult;

  QScriptValueIterator it(rawResult);
  while (it.hasNext()) {
   it.next();
   hashedResult[ it.name() ] = it.value().toString();
  }

  result.entropy = hashedResult[ "entropy" ].toDouble();
  
  result.timeToCrack = hashedResult[ "crack_time" ].toDouble();

  return result;
}

PasswordChecker *PasswordChecker::instance()
{
  if( !m_instance ) {
    m_instance = new PasswordChecker();
  }

  return m_instance;
}

bool PasswordChecker::valid() const
{
  return m_valid;
}


