cmake_minimum_required(VERSION 3.20)
project(VulkanTriangle)

add_subdirectory(Utility)
add_subdirectory(GLFWwrappers)
add_subdirectory(VulkanWrappers)
add_subdirectory(Rendering)

add_executable(vulkanTriangle main.cpp)
target_link_libraries(vulkanTriangle PRIVATE Rendering "${CMAKE_SOURCE_DIR}/steam_api64.lib")

add_custom_command(TARGET vulkanTriangle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/Shaders/vert.spv"
    "$<TARGET_FILE_DIR:vulkanTriangle>/vert.spv"
)

add_custom_command(TARGET vulkanTriangle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/Shaders/frag.spv"
    "$<TARGET_FILE_DIR:vulkanTriangle>/frag.spv"
)

add_custom_command(TARGET vulkanTriangle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/steam_appid.txt"
    "$<TARGET_FILE_DIR:vulkanTriangle>/steam_appid.txt"
)
