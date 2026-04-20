include(AMXConfig)

set(PLUGIN_SOURCES
	${PROJECT_SOURCE_DIR}/lib/pawn/amxplugin.cpp
	ColAndreas.cpp
	ColAndreasDatabaseReader.cpp
	ColObject.cpp
	DynamicWorld.cpp
	Natives.cpp
	dffread.cpp
	natives_table.cpp
	openmp_component.cpp
	renderware.cpp
)

add_library(${PROJECT_NAME} MODULE ${PLUGIN_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# amxplugin.cpp is a thin C-style dispatcher; skip any PCH treatment for it.
set_source_files_properties(${PROJECT_SOURCE_DIR}/lib/pawn/amxplugin.cpp PROPERTIES
	SKIP_PRECOMPILE_HEADERS ON
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}/lib/pawn
	${PROJECT_SOURCE_DIR}/lib/pawn/amx
	${PROJECT_SOURCE_DIR}/lib/bullet3/src
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	OMP-SDK
	BulletDynamics
	BulletCollision
	LinearMath
)

# Legacy ColAndreas sources pass string literals into logprintf (char *format). MSVC
# tolerates it; GCC/Clang want the warning muted. ColObject.cpp also triggers
# -Wno-write-strings from the original Makefile.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wno-write-strings -Wno-deprecated-declarations)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wno-attributes)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wno-ignored-attributes)
endif()

if(WIN32)
	# WIN32_LEAN_AND_MEAN keeps rpcndr.h/objidl.h out of the windows.h dragnet that
	# renderware.h pulls in. Without it, rpcndr's global `byte` collides with std::byte
	# inside span-lite (and anything else using std::byte) via the `using namespace std;`
	# in ColAndreasDatabaseReader.h / dffread.cpp. Can't do _HAS_STD_BYTE=0 globally —
	# open.mp-sdk's span-lite needs std::byte.
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		NOMINMAX
		WIN32_LEAN_AND_MEAN
		WIN32
		_WIN32
		_CRT_SECURE_NO_WARNINGS
	)
endif()

if(UNIX AND NOT APPLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE LINUX)
	target_compile_options(${PROJECT_NAME} PRIVATE -fno-strict-aliasing)
endif()

# CA_ARCH is set at top-level (so -m32 propagates to Bullet). Just log it here.
message(STATUS "ColAndreas: building ${CA_VERSION} for ${CA_ARCH} (pointer size: ${CMAKE_SIZEOF_VOID_P})")
