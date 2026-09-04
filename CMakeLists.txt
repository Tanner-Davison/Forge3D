# Version->Project->Standard block -> find_package calls -> targets ->
# link/include wiring

# STANDARD BLOCK
# -----------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
project(tannery LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Makes it Fail-Loudly instead of silently
                                    # downgrading
# FIND PACKAGES
# -----------------------------------------------------------------------------------------
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(SOURCES
    core/src/main.cpp
    core/src/App.cpp
    core/src/VulkanInstance.cpp
    core/src/Window.cpp
    core/src/physicalDevice.cpp
    core/src/queueFamilies.cpp
    core/src/LogicalDevice.cpp
    core/src/Surface.cpp
    core/src/swapchainSupport.cpp
    core/src/Swapchain.cpp)
# TARGETS
# -------------------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# LINKING/INCLUDES
# -------------------------------------------------------------------------------------------
target_link_libraries(tannery PRIVATE glfw Vulkan::Vulkan)

if(APPLE)
    target_link_libraries(
        ${PROJECT_NAME} PUBLIC "-framework Metal" "-framework MetalKit"
                               "-framework Cocoa")
endif()

target_include_directories(
    ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core/include
                            ${CMAKE_CURRENT_SOURCE_DIR}/core/src)
