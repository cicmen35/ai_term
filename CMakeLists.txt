cmake_minimum_required(VERSION 3.14)
project(ai_term)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/llama.cpp)

add_executable(ai_term src/main.cpp src/wrapper.cpp)

target_include_directories(ai_term PRIVATE external/llama.cpp/include)
target_link_libraries(ai_term PRIVATE llama)
