#include "OpenGLException.h"

OpenGLException::OpenGLException(const char * const msg) {
	m_msgStack.push_back( std::make_pair(QString(msg), QString()));
}

OpenGLException::OpenGLException(const char * const msg, const QString & detailedText) {
	m_msgStack.push_back( std::make_pair(QString(msg), detailedText));
}

OpenGLException::OpenGLException(OpenGLException & previous, const char * const msg) :
	m_msgStack(previous.m_msgStack)
{
	m_msgStack.push_back( std::make_pair(QString(msg), QString()));
}

OpenGLException::OpenGLException(OpenGLException & previous, const char * const msg, const QString & detailedText)  :
	m_msgStack(previous.m_msgStack)
{
	m_msgStack.push_back( std::make_pair(QString(msg), detailedText));
}

void OpenGLException::writeMsgStackToStream(std::ostream & strm) const {
	for (std::list<std::pair<QString, QString> >::const_iterator it = m_msgStack.begin();
		it != m_msgStack.end(); ++it)
	{
		strm << "*** " << it->first.toStdString() << " ***\n" << it->second.toStdString() << std::endl;
	}
}
