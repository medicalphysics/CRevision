cmake_minimum_required(VERSION 3.9)

set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo MinSizeRel)

project(CRevision VERSION 0.1.1 DESCRIPTION "CRevision description" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

##find QT
## Include QT
## QT specifics for CMake

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql LinguistTools REQUIRED)

#set(TS_FILES translations/CTCAExplorer_en_US.ts)
set(RS_FILES resources/resources.qrc)

set(PROJECT_SOURCES
    src/app.cpp
    src/mainwindow.cpp
    src/userwidget.cpp
    src/models.cpp
    src/questionwidget.cpp
    src/questioncollectionwidget.cpp
    src/casewidget.cpp
    src/nextwidget.cpp
    src/rolewidget.cpp
    src/infowidget.cpp
    ${TS_FILES}
    ${RS_FILES}
    )
set(PROJECT_HEADERS
    include/mainwindow.h
    include/userwidget.h
    include/models.h
    include/questionwidget.h
    include/questioncollectionwidget.h
    include/casewidget.h
    include/nextwidget.h
    include/rolewidget.h
    include/infowidget.h
    )


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CRevision WIN32
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(CRevision WIN32
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        )
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(CRevision PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
target_include_directories(CRevision PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/CRevision")

##deploy all win32 dlls since all are not included by default for cmake 3.19
include(InstallRequiredSystemLibraries)

if (WIN32)
    get_target_property(QT_QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(QT_WINDEPLOYQT_EXECUTABLE ${QT_QMAKE_EXECUTABLE} PATH)
    set(QT_WINDEPLOYQT_EXECUTABLE "${QT_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
    add_custom_target(qtdeploy ALL
        COMMAND ${QT_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE_DIR:CRevision>)

    ##deploy all win32 dlls since all are not included by default for cmake 3.19
    include(InstallRequiredSystemLibraries)
    # For windows we need to include msvcp_atomic_wait library manually
	foreach(WIN32LIB ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
		get_filename_component(WIN32LIBFOLDER ${WIN32LIB} DIRECTORY)
		file(GLOB WIN32ALLLIBS "${WIN32LIBFOLDER}/*.dll")
		foreach(WIN32LIBPATH ${WIN32ALLLIBS})
			set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${WIN32LIBPATH} ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})        
		endforeach()    
	endforeach()
	list(REMOVE_DUPLICATES CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
	foreach(WIN32LIB ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
		get_filename_component(WIN32LIBNAME ${WIN32LIB} NAME)
        set(outpath "${CMAKE_BINARY_DIR}")
		add_custom_command(TARGET qtdeploy POST_BUILD
			COMMENT "Copying WIN32 dll ${WIN32LIB} to ${outpath}"
			DEPENDS ${WIN32LIB}
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${WIN32LIB}
			${outpath}
			)
	endforeach()


endif(WIN32)


#Copying application resources
set (fileresources
#    "resources/icons/icon.png"
    "resources/data/sampleCases.txt"
    "resources/data/sampleQuestions.txt"
    )


foreach(fileresource ${fileresources})
    set(outpath "${CMAKE_BINARY_DIR}/${fileresource}")
    set(inpath "${CMAKE_CURRENT_SOURCE_DIR}/${fileresource}")
    message("Setup copying ${inpath} to ${outpath}")
    add_custom_command(TARGET qtdeploy POST_BUILD
        COMMENT "Copying file ${inpath} to ${outpath}."
        DEPENDS ${inpath}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${inpath}
        ${outpath}
        )
endforeach()


