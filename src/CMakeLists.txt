# collect all imgui files
# file(GLOB SOURCES vendor/imgui/*.cpp)
# file(GLOB IMGUIZMO_SOURCES vendor/ImGuizmo/*.cpp)

# Add the source files
add_executable(${PROJECT_NAME} 
	main.cpp 
	core/app.cpp

	# Window
	core/window/window.cpp

	# Renderer
	core/renderer/renderer.cpp

	# Input
	core/input/mouse.cpp
	core/input/input.cpp

	# Shader
	core/shader/shader.cpp

	# Material
	core/material/material.cpp

	# Components
	core/components/mesh.cpp

	# stbi image
	# vendor/stbi_image/stbi_image.cpp

	# Imgui
	# ${SOURCES}
	# vendor/imgui/backends/imgui_impl_opengl3.cpp
	# vendor/imgui/backends/imgui_impl_glfw.cpp
	# vendor/imgui/misc/cpp/imgui_stdlib.cpp

	# ImGuizmo
	# ${IMGUIZMO_SOURCES}

)

# inlcude
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
# imgui include
# target_include_directories(${PROJECT_NAME} PUBLIC vendor/imgui)
# target_include_directories(${PROJECT_NAME} PUBLIC vendor/ImGuizmo)


# Link to vendor
target_link_libraries(${PROJECT_NAME} vendor)
