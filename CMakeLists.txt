cmake_minimum_required(VERSION 3.14)

# Project name and version
project(PixelPuppet VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Qt directory (update for Qt6)
set(Qt6_DIR "C:/Qt/6.8.1/mingw_64/lib/cmake/Qt6")

# Find Qt6 packages for Widgets, Network, and Core
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Core)

# Check if Qt6 is found
if (NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found!")
endif()

# Add the executable as a GUI application (WIN32 suppresses the console window)
add_executable(${PROJECT_NAME} WIN32
    main.cpp
    pixelpuppetmain.cpp
    pixelpuppetmain.h
    canvas.cpp
    canvas.h
    toolpanel.h
    toolpanel.cpp
    layermanager.h
    brush.h
    layermanager.cpp
    brush.cpp
    resources.qrc
    NetworkManager.h
    networkmanager.cpp
    PaintTogetherDialog.h
    painttogetherdialog.cpp
    sessioncreateddialog.h
    sessioncreateddialog.h sessioncreateddialog.cpp
)

# Link Qt6 libraries, including Qt6::Widgets, Qt6::Network, and Qt6::Core
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Network Qt6::Core)

# Enable the Qt MOC (Meta-Object Compiler), UIC for UI files, and RCC for resources
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Include directories for NetworkManager and PaintTogetherDialog
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
