cmake_minimum_required(VERSION 3.14)

# Project name and version
project(PixelPuppet VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Qt directory (if necessary for your environment)
set(Qt5_DIR "D:/Qt/6.7.2/mingw_64/lib/cmake/Qt6")

# Find Qt6 package for Widgets
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Add the executable as a GUI application (WIN32 suppresses the console window)
add_executable(${PROJECT_NAME} WIN32
    main.cpp
    pixelpuppetmain.cpp
    pixelpuppetmain.h
)

# Link Qt6 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Enable the Qt MOC (Meta-Object Compiler)
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
