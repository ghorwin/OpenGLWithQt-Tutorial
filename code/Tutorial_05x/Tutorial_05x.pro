#------------------------------------------------------------------
#
# Testproject to check for OpenGL functionality in a Qt application
#
# New: use of element buffers and rect meshes to draw the planes of
# the cube
#------------------------------------------------------------------

QT       += core gui opengl widgets

TARGET = Var_05_ElementBuffers
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
	Camera.cpp \
	GridObject.cpp \
	KeyboardMouseHandler.cpp \
	RectMesh.cpp \
	ShaderProgram.cpp \
	Transform3d.cpp \
	OpenGLRenderWindow.cpp \
	main.cpp \
	Dialog.cpp

HEADERS += \
	Camera.h \
	Dialog.h \
	GridObject.h \
	KeyboardMouseHandler.h \
	RectMesh.h \
	ShaderProgram.h \
	Transform3d.h \
	OpenGLRenderWindow.h \
	Vertex.h

FORMS += \
	Dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	resources.qrc
