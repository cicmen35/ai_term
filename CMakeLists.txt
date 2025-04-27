cmake_minimum_required(VERSION 3.10)
project(ai_term)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)  # Enable Qt's Meta-Object Compiler
set(CMAKE_AUTORCC ON)  # Enable Qt's Resource Compiler
set(CMAKE_AUTOUIC ON)  # Enable Qt's UI Compiler

# Add llama.cpp as a subdirectory to build it
add_subdirectory(external/llama.cpp)

# Find Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Define our executable
add_executable(ai_term
    src/main.cpp
    src/wrapper.cpp
    src/mainwindow.cpp
)

# Include directories
target_include_directories(ai_term PRIVATE 
    src
    external/llama.cpp
    external/llama.cpp/include
)

# Link libraries
target_link_libraries(ai_term PRIVATE 
    llama
    Qt5::Widgets
)
