 cmake_minimum_required(VERSION 3.20)
 project(forge3d LANGUAGES CXX)
 set(CMAKE_CXX_STANDARD 23)
 
set(CMAKE_CXX_STANDARD_REQUIRED ON) # makes it Fail-Loudly instead of silently downgrading

# Note: Common structure for all CMakeLists.txt files goes in order:
# Version->Project->Standard block -> find_package calls -> targets ie(add_executable) -> link/include wiring

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(forge3d main.cpp)

target_link_libraries(forge3d PRIVATE glfw Vulkan::Vulkan)
