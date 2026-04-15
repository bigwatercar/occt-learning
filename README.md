## OCCT 7.9.3 + Qt5 (VS2022) 脚手架

### 目标

- **OpenCascade (OCCT) 7.9.3**
- **Qt 5.15.14 或更高**（通过 vcpkg 的 `qt5` 包提供）
- **VS2022 + CMake** 工程
- 主窗口包含常用菜单入口（不实现具体功能）
- 主窗口中心区域为 OCCT 视图，支持基础鼠标交互（缩放/旋转/平移）
- 默认显示一个 **圆锥**

### 依赖管理（推荐：vcpkg manifest）

本工程使用 `vcpkg.json`（manifest）声明依赖：

- `opencascade`
- `qt5`

### 构建步骤（Windows / VS2022）

1. 安装 vcpkg（任意路径均可），并确保可用 CMake toolchain：

- 参考 vcpkg 官方文档，安装后记下 toolchain 文件路径：`<vcpkg_root>/scripts/buildsystems/vcpkg.cmake`

1. 生成 VS2022 工程（推荐 out-of-source）：

```powershell
cd D:\Personal\github\occt-learning
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="<vcpkg_root>\scripts\buildsystems\vcpkg.cmake"

cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
```

> 说明：这是 **vcpkg manifest** 模式，第一次配置会自动解析 `vcpkg.json` 并安装依赖。

1. 编译：

```powershell
cmake --build build --config Release
```

1. 运行：

```powershell
.\build\Release\occt_qt_scaffold.exe
```

### 不用 vcpkg 的方式（严格使用你安装的 OCCT 7.9.3 / Qt 5.15.14+）

如果你已经安装好了 **OCCT 7.9.3**（带 OpenGL）和 **Qt 5.15.14+**，也可以直接指定它们的 CMake 包路径：

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DQt5_DIR="C:\Qt\5.15.14\msvc2019_64\lib\cmake\Qt5" `
  -DOpenCASCADE_DIR="C:\OpenCASCADE-7.9.3\cmake"
cmake --build build --config Release
```

### 说明

- `src/OccView.*`：封装 OCCT Viewer/Context/View，默认显示圆锥，并使用 `AIS_ViewController` 实现基础交互。
- 如果你希望 **强制锁定 OCCT 7.9.3**（以及 Qt 版本），建议在仓库中加入 `vcpkg-configuration.json` 并指定你本机 vcpkg 的 builtin baseline（一个 commit hash）。你把 `vcpkg --version` 和 vcpkg 当前 commit 发我，我可以帮你生成对应文件。

