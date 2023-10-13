cmake_minimum_required(VERSION 3.24)
project(OCR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SW_BUILD_SHARED_LIBS 1)

set(DEPENDENCIES
        org.sw.demo.google.tesseract.libtesseract-5.3.3
        org.sw.demo.intel.opencv.imgproc-4.8.1
        org.sw.demo.intel.opencv.highgui-4.8.1
)

find_package(SW REQUIRED)
sw_add_package(${DEPENDENCIES})
sw_execute()

find_package(Qt6 COMPONENTS
        Core
        Core5Compat
        Gui
        Widgets
        PdfWidgets
        REQUIRED
)

add_executable(OCR main.cpp
        mainwindow.ui
        mainwindow.cpp
        mainwindow.h
        mainwindow.qrc
)
target_link_libraries(OCR PUBLIC
        ${DEPENDENCIES}
        Qt6::Core
        Qt6::Core5Compat
        Qt6::Gui
        Qt6::Widgets
        Qt6::PdfWidgets
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

file(GLOB_RECURSE LIB_FILES "${PROJECT_SOURCE_DIR}/cmake-build-debug-visual-studio/Debug/*")


foreach (LIB ${LIB_FILES})
    get_filename_component(LIB_NAME ${LIB} NAME)

    configure_file("${PROJECT_SOURCE_DIR}/cmake-build-debug-visual-studio/Debug/${LIB_NAME}"
            ${PROJECT_SOURCE_DIR}/cmake-build-debug-visual-studio/${LIB_NAME} COPYONLY)
endforeach (LIB)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets PdfWidgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
