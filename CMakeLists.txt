cmake_minimum_required(VERSION 3.9)

#set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo MinSizeRel CACHE TYPE INTERNAL FORCE)
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo MinSizeRel)

project(CRevision VERSION 0.0.1 DESCRIPTION "CRevision description" LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)


# Build types 
set(CMAKE_CONFIGURATION_TYPES )

## QT specifics for CMake
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts Sql)
find_package(Qt5Charts)
IF(UNIX AND NOT APPLE)
    find_package(Qt5 REQUIRED COMPONENTS X11Extras )
ENDIF()
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Charts_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# Define an executable (WIN32 argument to prevent console window on windows)
add_executable(CRevision WIN32
    src/app.cpp
	src/mainwindow.h
	src/mainwindow.cpp
	src/userinfowidget.h
	src/userinfowidget.cpp
	src/models.h
	src/models.cpp
    )


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/CRevision")

# Define the libraries this project depends upon
target_link_libraries(CRevision
        Qt5::Widgets
		Qt5::Core
		Qt5::Gui
		Qt5::Charts
		Qt5::Sql
        )
##adding REQUIRED dlls to the binary folder of executable
#adding target to copy qt dlls 
if (WIN32)
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
	add_custom_target(qtdeploy ALL
       COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_BINARY_DIR} $<TARGET_FILE_DIR:CRevision>)
endif(WIN32)


#Copying application resources
add_custom_target(addresources ALL)
set (fileresources 
	"icons/icon.png"
	"data/sampleCases.txt"
	"data/sampleQuestions.txt"
	)
foreach(fileresource ${fileresources})
	set(outpath "${CMAKE_BINARY_DIR}/resources/${fileresource}")
	set(inpath "${CMAKE_CURRENT_SOURCE_DIR}/resources/${fileresource}")
	message("Setup cpoying ${inpath} to ${outpath}")
	add_custom_command(TARGET addresources POST_BUILD
		COMMENT "Copying file ${inpath} to ${outpath}."
		DEPENDS ${inpath}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		${inpath}
		${outpath}
	)
endforeach()