project(ssl-logtools)

set(USE_QT5 FALSE CACHE BOOL "Use Qt5 instead of Qt4")
if(USE_QT5)
	cmake_minimum_required(VERSION 2.8.9)
	if(POLICY CMP0020)
	    cmake_policy(SET CMP0020 NEW) # remove if CMake >= 2.8.11 required
	endif()
	if(POLICY CMP0043) # compatibility with CMake 3.0.1
	    cmake_policy(SET CMP0043 OLD)
	endif()
	if(POLICY CMP0071) # compatibility with CMake 3.10.0
	    cmake_policy(SET CMP0071 OLD)
	endif()
else()
	cmake_minimum_required(VERSION 2.8.2)
endif()

find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)

if(USE_QT5)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	set(CMAKE_AUTOMOC ON)
	find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)
	# replaced by automoc
	macro(qt4_wrap_cpp VARNAME)
		set(${VARNAME} "")
	endmacro()
	# wrap functions
	macro(qt4_wrap_ui)
		qt5_wrap_ui(${ARGN})
	endmacro()
	macro(qt4_add_resources)
		qt5_add_resources(${ARGN})
	endmacro()
else()
	find_package(Qt4 4.6.0 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
endif()

find_package(Boost 1.42.0 COMPONENTS program_options REQUIRED)
find_package(ZLIB REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
