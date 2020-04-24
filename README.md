# OpenGL + Qt Tutorial
A tutorial to use OpenGL within Qt Widgets applications. A comprehensive discussion of many Qt Wrapper and convenience classes and functions to assist with OpenGL development. Includes discussion of best-practice approaches for many topics, including  input handling and battery-efficient render-on-demand.

The generated documentation can be found on:

- https://ghorwin.github.io/OpenGLWithQt-Tutorial

The offline-PDF version can be downloaded from the repo: 

https://github.com/ghorwin/OpenGLWithQt-Tutorial/raw/master/docs/OpenGLQtTutorial.pdf


## Contents

Tutorial discusses modern OpenGL approach with shader programs and vertex buffers/arrays. The following Qt classes are discussed:

- `QMatrix4x4`
- `QVector3D` and `QVector4D`
- `QOpenGLWindow` and `QOpenGLWidget`
- `QOpenGLContext`, `QSurfaceFormat` and `QOpenGLFunctions`
- `QOpenGLShaderProgram`
- `QOpenGLTexture`
- `QOpenGLFramebufferObject`
- `QOpenGLVertexArrayObject` and `QOpenGLBuffer`
- `QOpenGLDebugLogger`
- `QOpenGLTimerQuery`, `QOpenGLTimeMonitor`, and `QElapsedTimer`


## Writing and generating the docs

Uses Asciidoctor. Go to directory `text` and run `build.sh`.



