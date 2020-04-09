#ifndef OPENGLEXCEPTION_H
#define OPENGLEXCEPTION_H

#include <stdexcept>
#include <list>

#include <QString>

class OpenGLException : public std::exception {
public:
	OpenGLException(const QString & msg);
	OpenGLException(const QString & msg, const QString & where);
	OpenGLException(OpenGLException & previous, const QString & msg);
	OpenGLException(OpenGLException & previous, const QString & msg, const QString & where);
	void writeMsgStackToStream(std::ostream & strm) const;

private:
	std::list<std::pair<QString, QString> > m_msgStack;
};

#define FUNCID(x) const char * const FUNC_ID = "[" #x "]"

#endif // OPENGLEXCEPTION_H
