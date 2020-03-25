# CMakeLists.txt file for OpenGL + Qt Tutorial Series

# The project name
project( Tutorial_05 )

# Require a fairly recent cmake version
cmake_minimum_required( VERSION 2.8.12 )

# Set default build type
if (NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# -------------------------------------------------------------
# Packages
# -------------------------------------------------------------

find_package( OpenGL REQUIRED )

# Test for Qt5
find_package(Qt5Widgets REQUIRED)

if( APPLE )
	find_library(IOKIT NAMES IOKit REQUIRED)
	find_library(COREFOUNDATION NAMES CoreFoundation REQUIRED)
	find_library(SECURITY_FRAMEWORK Security REQUIRED)
	find_library(SYSTEM_CONFIGURATION SystemConfiguration REQUIRED)

	set(APPLE_FRAMEWORKS
		${IOKIT}
		${SECURITY_FRAMEWORK}
		${COREFOUNDATION}
		${SYSTEM_CONFIGURATION}
	)
endif( APPLE )


# -------------------------------------------------------------
# Application
# -------------------------------------------------------------

include_directories(
	${Qt5Widgets_INCLUDE_DIRS}
)

# collect a list of all header files (to be used in MOC compiler)
file( GLOB APP_HDRS ${PROJECT_SOURCE_DIR}/*.h )

# collect a list of all source files in this directory
file( GLOB APP_SRCS ${PROJECT_SOURCE_DIR}/*.cpp )

# collect the Qt resource files (*.qrc)
file( GLOB APP_QRCS ${PROJECT_SOURCE_DIR}/*.qrc )

qt5_add_resources( APP_RC_SRCS ${APP_QRCS} )
qt5_wrap_cpp( APP_MOC_SRCS ${APP_HDRS} )


# build application executable for the different platforms
if( WIN32 )
	add_executable( ${PROJECT_NAME} WIN32
					 ${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS} ${APP_WIN_RC}
	)

	set(WIN_FRAMEWORKS iphlpapi gdiplus gdi32 user32 ole32)
endif( WIN32 )

if( UNIX )
	if( APPLE )
		add_executable( ${PROJECT_NAME} MACOSX_BUNDLE
						 ${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS}
						 ${PROJECT_SOURCE_DIR}/Tutorial.icns
		)
	else( APPLE )
		add_executable( ${PROJECT_NAME}
						 ${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS}
		)
	endif( APPLE )
endif( UNIX )


# link libraries
target_link_libraries( ${PROJECT_NAME}
	Qt5::Widgets
	${OPENGL_LIBRARIES}
	${APPLE_FRAMEWORKS}
	${WIN_FRAMEWORKS}
)


