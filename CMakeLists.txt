cmake_minimum_required(VERSION 3.27)
project(Projekt_AiSD2 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)

add_executable(Projekt_AiSD2 main.cpp)
target_sources(Projekt_AiSD2 PRIVATE main.cpp)
#target_link_libraries(Projekt_AiSD2 PRIVATE libs_here)