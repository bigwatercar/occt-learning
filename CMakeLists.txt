cmake_minimum_required(VERSION 3.21)

# Auto-pick vcpkg toolchain for first configure when user did not pass it explicitly.
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
  set(_vcpkg_toolchain "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
  if (EXISTS "${_vcpkg_toolchain}")
    set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_toolchain}" CACHE STRING "vcpkg toolchain file")
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
  endif()
endif()

project(occt_qt_scaffold
  VERSION 0.1.0
  LANGUAGES CXX
)

if (NOT WIN32)
  message(FATAL_ERROR "This scaffold currently targets Windows only (uses WNT_Window).")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if (NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET x64-windows CACHE STRING "vcpkg triplet")
endif()

if (MSVC)
  add_compile_options(/permissive- /Zc:__cplusplus /utf-8)
endif()

find_package(Qt5 5.15 REQUIRED COMPONENTS Widgets)

# vcpkg opencascade provides OpenCASCADEConfig.cmake; TK* imported targets have no namespace.
find_package(OpenCASCADE REQUIRED)

add_executable(occt_qt_scaffold
  src/main.cpp
  src/OccDocument.h
  src/OccDocument.cpp
  src/MainWindow.h
  src/MainWindow.cpp
  src/OccView.h
  src/OccView.cpp
)

target_include_directories(occt_qt_scaffold PRIVATE src)

target_link_libraries(occt_qt_scaffold
  PRIVATE
    Qt5::Widgets
    TKernel
    TKMath
    TKBRep
    TKGeomBase
    TKGeomAlgo
    TKTopAlgo
    TKPrim
    TKG3d
    TKV3d
    TKService
    TKOpenGl
)

if (WIN32)
  target_compile_definitions(occt_qt_scaffold PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()
