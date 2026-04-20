# Repository AI Context (Migration Ready)

## Developer Background
- Experienced C/C++ programmer.
- Currently learning computer graphics fundamentals and OCCT-based CAD application development.
- Not yet familiar with computer graphics concepts or the OCCT API — prefer explanations that build from first principles.

## Project Background
- Project name: `occt_qt_scaffold`
- Purpose: Minimal scaffold for CAD-like desktop development using Qt + OpenCascade.
- Current stack:
  - Qt5 (Widgets)
  - OpenCascade (OCCT) 7.9.3
  - CMake (VS2022 generator on Windows)
  - vcpkg manifest mode (`vcpkg.json`)
- Primary platform: Windows (current view binding uses `WNT_Window`).

## Dependency and Toolchain Rules
- Dependencies are declared in `vcpkg.json`.
- Preferred configure path is vcpkg toolchain.
- CMake supports auto-detecting toolchain from `VCPKG_ROOT` when not passed explicitly.
- Default vcpkg triplet is `x64-windows`.

## Build and Run (Windows)
- Configure:
  - `cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows`
- Build:
  - `cmake --build build --config Release`
- Run:
  - `build\Release\occt_qt_scaffold.exe`

## Source Layout
- `src/main.cpp`: Qt app entry.
- `src/MainWindow.*`: Main UI shell, menus, multi-document tabs.
- `src/OccDocument.*`: Per-document OCCT viewer/context/model state.
- `src/OccView.*`: OCCT native view widget and mouse interactions.

## Runtime Behavior
- Default scene displays a cone shape.
- Basic interactions are wired through `AIS_ViewController`:
  - Orbit / rotate
  - Pan
  - Wheel zoom

## Migration Notes
When migrating this project to another machine/repo:
1. Ensure `VCPKG_ROOT` is set, or pass `CMAKE_TOOLCHAIN_FILE` explicitly.
2. Keep compiler/toolchain compatible with the installed Qt5 and OCCT binaries.
3. If moving off Windows, replace/adapt `WNT_Window` usage in `OccView.cpp` and adjust platform-specific OCCT window binding.
4. Keep `CMakeLists.txt` and `vcpkg.json` as the source of truth for dependency versions/features.

## Guidance for AI Assistants
- Prefer minimal, surgical changes over broad refactors.
- Preserve current architecture (`MainWindow` + `OccDocument` + `OccView`).
- For build/perf validation requests, prefer Release configuration unless explicitly requested otherwise.
