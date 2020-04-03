#ifndef OPENGLEXCEPTION_H
#define OPENGLEXCEPTION_H

#include <stdexcept>
#include <list>

#include <QString>

class OpenGLException : public std::exception {
public:
	OpenGLException(const char * const msg);
	OpenGLException(const char * const msg, const QString & detailedText);
	OpenGLException(OpenGLException & previous, const char * const msg);
	OpenGLException(OpenGLException & previous, const char * const msg, const QString & detailedText);
	void writeMsgStackToStream(std::ostream & strm) const;

private:
	std::list<std::pair<QString, QString> > m_msgStack;
};

#endif // OPENGLEXCEPTION_H
