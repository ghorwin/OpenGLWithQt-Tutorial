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

# Test for Qt5 modules
find_package(Qt5Widgets REQUIRED)

# set corresponding libraries
set( QT_LIBRARIES
	Qt5::Widgets
)

# we need OpenGL
find_package( OpenGL REQUIRED )


# -------------------------------------------------------------
# Application
# -------------------------------------------------------------

# automatically add CMAKE_CURRENT_SOURCE_DIR and CMAKE_CURRENT_BINARY_DIR to the include directories in every processed CMakeLists.txt
set( CMAKE_INCLUDE_CURRENT_DIR ON )

include_directories(
	${PROJECT_SOURCE_DIR}		# needed so that ui-generated header files find our own headers
	${Qt5Widgets_INCLUDE_DIRS}
)

# collect a list of all designer ui files
file( GLOB APP_UIS ${PROJECT_SOURCE_DIR}/*.ui )

# collect a list of all header files (to be used in MOC compiler)
file( GLOB APP_HDRS ${PROJECT_SOURCE_DIR}/*.h )

# collect a list of all source files in this directory
file( GLOB APP_SRCS ${PROJECT_SOURCE_DIR}/*.cpp )

# collect the Qt resource files (*.qrc)
file( GLOB APP_QRCS ${PROJECT_SOURCE_DIR}/*.qrc )

# look for Windows rc file
file( GLOB APP_WIN_RC ${PROJECT_SOURCE_DIR}/*.rc )

# look for Apple icns file
file( GLOB APP_MACOS_ICNS ${PROJECT_SOURCE_DIR}/*.icns )

qt5_wrap_ui( APP_UI_SRCS ${APP_UIS} )
qt5_add_resources( APP_RC_SRCS ${APP_QRCS} )
qt5_wrap_cpp( APP_MOC_SRCS ${APP_HDRS} )

# build application executable for the different platforms
if( WIN32 )
	add_executable( ${PROJECT_NAME} WIN32
					${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS} ${APP_WIN_RC}
	)

	# enable console window (to see debug/profiler messages)
	set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE" )
endif( WIN32 )

if( UNIX )
	if( APPLE )
		add_executable( ${PROJECT_NAME} MACOSX_BUNDLE
						${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS} ${APP_MACOS_ICNS}
		)
	else( APPLE )
		add_executable( ${PROJECT_NAME}
						${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS}
		)
	endif( APPLE )
endif( UNIX )


# link libraries
target_link_libraries( ${PROJECT_NAME}
	${QT_LIBRARIES}
	${OPENGL_LIBRARIES}
)


