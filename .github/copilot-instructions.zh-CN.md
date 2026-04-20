# 仓库 AI 上下文（迁移就绪）

## 开发者背景
- 有经验的 C/C++ 程序员。
- 目前正在学习计算机图形学基础知识，以及基于 OCCT 的 CAD 应用开发。
- 对计算机图形学概念和 OCCT API 尚不熟悉——解释时请从基础原理出发，避免假设已有相关前置知识。

## 项目背景
- 项目名称：`occt_qt_scaffold`
- 用途：基于 Qt + OpenCascade 的 CAD 类桌面开发最小脚手架。
- 当前技术栈：
  - Qt5（Widgets）
  - OpenCascade（OCCT）7.9.3
  - CMake（Windows 下使用 VS2022 生成器）
  - vcpkg manifest 模式（`vcpkg.json`）
- 主平台：Windows（当前视图绑定使用 `WNT_Window`）。

## 依赖与工具链规则
- 依赖项在 `vcpkg.json` 中声明。
- 推荐使用 vcpkg toolchain 进行配置。
- 未显式传入 toolchain 时，CMake 可从 `VCPKG_ROOT` 自动探测。
- 默认 vcpkg triplet 为 `x64-windows`。

## 构建与运行（Windows）
- 配置：
  - `cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows`
- 编译：
  - `cmake --build build --config Release`
- 运行：
  - `build\Release\occt_qt_scaffold.exe`

## 源码结构
- `src/main.cpp`：Qt 应用入口。
- `src/MainWindow.*`：主窗口 UI 框架、菜单栏、多文档 Tab。
- `src/OccDocument.*`：每个文档的 OCCT Viewer/Context/模型状态。
- `src/OccView.*`：OCCT 原生视图 Widget 及鼠标交互。

## 运行行为
- 默认场景显示一个圆锥体。
- 基础交互通过 `AIS_ViewController` 实现：
  - 轨道旋转
  - 平移
  - 滚轮缩放

## 迁移注意事项
将项目迁移到其他机器或仓库时：
1. 确保 `VCPKG_ROOT` 已设置，或显式传入 `CMAKE_TOOLCHAIN_FILE`。
2. 保持编译器/工具链与已安装的 Qt5 和 OCCT 二进制兼容。
3. 若迁移离开 Windows，需替换/适配 `OccView.cpp` 中的 `WNT_Window` 用法，并调整 OCCT 平台相关的窗口绑定。
4. 以 `CMakeLists.txt` 和 `vcpkg.json` 作为依赖版本与特性的唯一来源。

## AI 助手改动约束
- 优先做最小、精准的修改，避免大范围重构。
- 保持当前架构（`MainWindow` + `OccDocument` + `OccView`）不变。
- 构建/性能验证请求默认使用 Release 配置，除非明确要求 Debug。
