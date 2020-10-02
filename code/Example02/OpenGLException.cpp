#include "OpenGLException.h"

#include <QStringList>
#include <iostream>

OpenGLException::OpenGLException(const QString & msg) {
	m_msgStack.push_back( std::make_pair(QString(msg), QString()));
}

OpenGLException::OpenGLException(const QString & msg, const QString & where) {
	m_msgStack.push_back( std::make_pair(QString(msg), where));
}

OpenGLException::OpenGLException(OpenGLException & previous, const QString & msg) :
	m_msgStack(previous.m_msgStack)
{
	m_msgStack.push_back( std::make_pair(QString(msg), QString()));
}

OpenGLException::OpenGLException(OpenGLException & previous, const QString & msg, const QString & where)  :
	m_msgStack(previous.m_msgStack)
{
	m_msgStack.push_back( std::make_pair(QString(msg), where));
}

void OpenGLException::writeMsgStackToStream(std::ostream & strm) const {
	for (std::list<std::pair<QString, QString> >::const_iterator it = m_msgStack.begin();
		it != m_msgStack.end(); ++it)
	{
		QStringList lines = it->first.split("\n");
		QString indx("[%1] ");
		indx = indx.arg(std::distance(m_msgStack.begin(),it));
		for (const QString & l : lines) {
			if (it->second.isEmpty())
				strm << (indx + l + "\n").toStdString();
			else
				strm << (indx + it->second + " : " + l + "\n").toStdString();
		}
		strm.flush();
	}
}
