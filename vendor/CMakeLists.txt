# Configure SDL by calling its CMake file.
# we use EXCLUDE_FROM_ALL so that its install targets and configs don't
# pollute upwards into our configuration.
# add_subdirectory(SDL EXCLUDE_FROM_ALL)


# GLM
add_subdirectory(glm EXCLUDE_FROM_ALL)

# glad
add_library(glad STATIC glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# glfw
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# Add library named vendor as an interface
add_library(vendor INTERFACE)

target_link_libraries(vendor INTERFACE glad)
target_link_libraries(vendor INTERFACE glfw)
target_link_libraries(vendor INTERFACE glm::glm)
