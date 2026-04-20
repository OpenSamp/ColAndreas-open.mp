cmake_minimum_required(VERSION 3.19)

# Several vendored subtrees (Bullet's glm vendor, open.mp-sdk/lib/glm, ...) still
# declare cmake_minimum_required(VERSION 2.x/3.0), which CMake 3.31+ refuses. Force a
# policy-version floor for every sub-project so configure works out of the box.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "")

project(colandreas)

set(CA_VERSION "v1.6.0")
set(CA_DATABASE_VERSION 2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
	CA_VERSION="${CA_VERSION}"
	CA_DATABASE_VERSION=${CA_DATABASE_VERSION}
)

if(MSVC)
	add_compile_options(/MP)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib/cmake-modules")

# Architecture selector propagated to ALL subprojects (Bullet, open.mp-sdk, ...).
#
# Set here BEFORE any add_subdirectory so Bullet's targets compile/link with the same
# bitness as our plugin — otherwise Linux x86 builds link x86 colandreas.o against x64
# Bullet .a files and fail at link. Windows uses the VS generator's -A Win32/x64 flag
# instead, which handles this globally.
# ---------------------------------------------------------------------------
if(NOT DEFINED CA_ARCH)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CA_ARCH "x86")
	else()
		set(CA_ARCH "x64")
	endif()
endif()

if(CA_ARCH STREQUAL "x86" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(-m32)
	add_link_options(-m32)
endif()

# Bullet's static libs get linked into our shared module (colandreas.so). Without
# -fPIC on every subproject, ld rejects the link on Linux:
#   "relocation R_X86_64_TPOFF32 ... can not be used when making a shared object;
#    recompile with -fPIC".
# No-op on Windows (MSVC always produces position-independent code for DLLs).
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# open.mp SDK (interface target; propagates NOMINMAX, /fp:strict, glm, etc.)
add_subdirectory(lib/open.mp-sdk)

# ---------------------------------------------------------------------------
# Bullet Physics (vendored under lib/bullet3, shallow submodule).
#
# Bullet's own CMake builds dozens of extras (demos, pybullet, robotics, unit tests).
# We only want LinearMath + BulletCollision + BulletDynamics, so flip everything else
# off before include. These caches must be set BEFORE add_subdirectory to take effect.
# ---------------------------------------------------------------------------
set(BUILD_BULLET2_DEMOS      OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3            OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS          OFF CACHE BOOL "" FORCE)
set(BUILD_ENET               OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET           OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS             OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS      OFF CACHE BOOL "" FORCE)
set(BUILD_PYBULLET           OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS         OFF CACHE BOOL "" FORCE)
set(USE_GRAPHICAL_BENCHMARK  OFF CACHE BOOL "" FORCE)
set(USE_GLUT                 OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS        OFF CACHE BOOL "" FORCE)
set(INSTALL_LIBS             OFF CACHE BOOL "" FORCE)
set(INSTALL_CMAKE_FILES      OFF CACHE BOOL "" FORCE)

# Bullet uses MSVC /MT by default, which conflicts with other open.mp-sdk libs that use
# the default /MD. Force the standard runtime selection so linking lines up.
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "" FORCE)

# Suppress Bullet's warning noise — we don't modify its source.
if(MSVC)
	add_compile_options(/wd4005 /wd4244 /wd4267)
endif()

add_subdirectory(lib/bullet3 EXCLUDE_FROM_ALL)

add_subdirectory(src)
