cmake_minimum_required(VERSION 3.14)

# Project name and version
project(PixelPuppet VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Qt directory (update for Qt6)
set(Qt6_DIR "C:/Qt/6.8.1/mingw_64/lib/cmake/Qt6")

# Find Qt6 package for Widgets
find_package(Qt6 REQUIRED COMPONENTS Widgets)

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
    resources.qrc  # Make sure this file exists in your project
)

# Link Qt6 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Enable the Qt MOC (Meta-Object Compiler)
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
