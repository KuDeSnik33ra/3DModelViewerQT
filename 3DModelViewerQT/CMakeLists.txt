cmake_minimum_required (VERSION 3.11)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)

set(app
	app/main.cpp
	app/MainWindow.ui
	app/MainWindow.cpp
	app/MainWindow.h
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${app})

set(math
	math/Ray.h
	math/Plane.h
	math/Triangle.h
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${math})

set(shaders
	shaders/shaders.qrc
	shaders/fshaderColor.glsl
	shaders/fshaderTexture.glsl
	shaders/vshader.glsl
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${shaders})

set(textures
	textures/textures.qrc
	textures/texture0.jpg
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${shaders})

set(engine
	engine/Camera.cpp
	engine/Camera.h
	engine/Material.cpp
	engine/Material.h
	engine/Mesh.cpp
	engine/Mesh.h
	engine/Shape.cpp
	engine/Shape.h
	engine/Line.cpp
	engine/Line.h
	engine/SceneManager.cpp
	engine/SceneManager.h
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${engine})

add_executable (3DModelViewerQT 
		${app}
		${math}
		${engine}
		${shaders}
		${textures})

target_include_directories(3DModelViewerQT SYSTEM
	PRIVATE
	)

target_link_libraries(3DModelViewerQT
	PRIVATE
	Qt5::Core Qt5::Gui Qt5::Widgets
	)

target_compile_definitions(3DModelViewerQT
	PRIVATE
	QT_NO_DEBUG
	)

set(DESTINATION_BIN_ROOT "${CMAKE_SOURCE_DIR}/bin")

set_target_properties(
    3DModelViewerQT PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DESTINATION_BIN_ROOT}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DESTINATION_BIN_ROOT}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DESTINATION_BIN_ROOT}"
    RUNTIME_OUTPUT_NAME_RELEASE 3DModelViewer64
    RUNTIME_OUTPUT_NAME_RELWITHDEBINFO 3DModelViewer64
    RUNTIME_OUTPUT_NAME_DEBUG 3DModelViewer64d
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 3DModelViewerQT)
set_property(TARGET 3DModelViewerQT PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${DESTINATION_BIN_ROOT}")

if(WIN32)
    if($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_DEBUG)
    else($<CONFIG:Debug>)
        get_target_property(WIDGETDLL Qt5::Widgets IMPORTED_LOCATION_RELEASE)
    endif($<CONFIG:Debug>)
    
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::Widgets>
            $<TARGET_FILE_DIR:3DModelViewerQT>
    )
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::Core>
            $<TARGET_FILE_DIR:3DModelViewerQT>
    )
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::Gui>
            $<TARGET_FILE_DIR:3DModelViewerQT>
    )
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:3DModelViewerQT>/styles
    )
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Qt5::QWindowsVistaStylePlugin>
        $<TARGET_FILE_DIR:3DModelViewerQT>/styles
    )
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:3DModelViewerQT>/platforms
    )
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>
        $<TARGET_FILE_DIR:3DModelViewerQT>/platforms
    )
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:3DModelViewerQT>/imageformats
    )
    add_custom_command(
        TARGET 3DModelViewerQT POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Qt5::QJpegPlugin>
        $<TARGET_FILE_DIR:3DModelViewerQT>/imageformats
    )
endif(WIN32)