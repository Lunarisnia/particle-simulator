# collect all imgui files
file(GLOB SOURCES vendor/imgui/*.cpp)
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

	# Object
	core/object/object.cpp

	# World
	core/world/world.cpp

	# Material
	core/material/material.cpp

	# Texture
	core/texture/texture.cpp
	core/texture/texture_manager.cpp
	
	# Static Camera
	core/static_camera/static_camera.cpp
	
	# Time
	core/time/time.cpp

	# Math
	core/math/math.cpp

	# Physics
	core/physics/physics_world.cpp

	# Framebuffer
	core/framebuffer/framebuffer.cpp

	# Renderbuffer
	core/renderbuffer/renderbuffer.cpp

	# Procedural
	core/procedural/procedural.cpp


	# Components
	core/components/component.cpp
	core/components/mesh.cpp
	core/components/transform.cpp
	core/components/rigidbody2d.cpp
	core/components/point_light.cpp
	core/components/spot_light.cpp

	# Particle
	particle/app.cpp
	particle/simulation/simulation.cpp
	particle/primitive/primitive.cpp
	particle/model/model.cpp

	# Editor
	editor/editor.cpp
	editor/hierarchy/hierarchy.cpp
	editor/inspector/inspector.cpp

	# stbi image
	vendor/stbi_image/stbi_image.cpp

	# Imgui
	${SOURCES}
	vendor/imgui/backends/imgui_impl_opengl3.cpp
	vendor/imgui/backends/imgui_impl_glfw.cpp
	vendor/imgui/misc/cpp/imgui_stdlib.cpp

	# ImGuizmo
	# ${IMGUIZMO_SOURCES}

)

# inlcude
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
# imgui include
target_include_directories(${PROJECT_NAME} PUBLIC vendor/imgui)
# target_include_directories(${PROJECT_NAME} PUBLIC vendor/ImGuizmo)


# Link to vendor
target_link_libraries(${PROJECT_NAME} vendor)
