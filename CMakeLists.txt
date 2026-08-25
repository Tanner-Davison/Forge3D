# Version->Project->Standard block -> find_package calls -> targets ->
# link/include wiring

# STANDARD BLOCK
# ----------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
project(forge3d LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Makes it Fail-Loudly instead of silently
                                    # downgrading
# FIND
# PACKAGES------------------------------------------------------------------------------------------
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(SOURCES src/main.cpp)
# TARGETS--------------------------------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# LINKING &
# INCLUDES------------------------------------------------------------------------------------------------------
target_link_libraries(forge3d PRIVATE glfw Vulkan::Vulkan)

if(APPLE)
  target_link_libraries(${PROJECT_NAME} PUBLIC "-framework Metal" "-framework MetalKit" "-framework Cocoa")
endif()

target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/src)
