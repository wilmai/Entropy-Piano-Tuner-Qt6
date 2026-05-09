# Entropy Piano Tuner (Qt 6 Migration Fork)

This is a fork of the Entropy Piano Tuner project, updated to support **Qt 6** and modern building environments.

## Information
The original homepage of the Entropy Piano Tuner project (piano-tuner.org) is no longer available. This fork maintains the source code and provides updates for modern systems.

## Key Changes in this Fork (Qt 6 Migration)
This version of the project has been significantly refactored to support **Qt 6.10+**. Key changes include:
- **Multimedia Migration**: Replaced deprecated `QMediaPlayer` APIs with the new Qt 6 Multimedia module (incl. `QAudioOutput`).
- **Audio API Updates**: Migrated from `QAudioDeviceInfo` to `QAudioDevice` and updated audio mode handling.
- **UI & Layouts**: Fixed deprecated layout margins (`setMargin` -> `setContentsMargins`) and font metrics (`width` -> `horizontalAdvance`).
- **Encoding**: Removed dependencies on `QTextCodec` as Qt 6 defaults to UTF-8.
- **Modern Regex**: Migrated from `QRegExp` to `QRegularExpression`.
- **Dependency Updates**: Compatible with **Qwt 6.2+**, **FFTW3**, and **libuv**.

## Building

### Prerequisites
- **Qt 6.10+** (specifically tested with 6.10.2)
- **C++ Compiler**:
  - Windows: MSVC 2022 (Visual Studio Build Tools 2022)
  - Linux: GCC or Clang
- **Linux packages** (Fedora; equivalents exist on other distros):
  `qt6-qtbase-devel qt6-qtmultimedia-devel qt6-qtsvg-devel qt6-qttools-devel alsa-lib-devel`

### Windows Build Instructions
An automated build script is provided to simplify the process:

1.  **Build MIDI Dependency**:
    ```powershell
    .\build_qtmidi.bat
    ```
2.  **Build Main Project**:
    ```powershell
    .\build_project.bat
    ```

The compiled binaries will be located in the `bin/` directory.

### Linux Build Instructions
A `build.sh` wrapper handles the qtmidi dependency and the main project in one step:

```bash
./build.sh              # build
./build.sh -c           # clean, then build
./build.sh --clean-only # clean and exit
JOBS=4 ./build.sh       # override parallel jobs (default: nproc)
QMAKE=/path/qmake6 ./build.sh
```

The compiled binaries land in `bin/`. Run the app with:

```bash
./bin/entropypianotuner
```

#### Containerized build (optional)
`devenv/Containerfile` builds a Fedora 44 image with the full Qt 6 / build
toolchain preinstalled — useful if you'd rather not install Qt system-wide:

```bash
podman build -t ept-dev devenv/
podman run --rm -it -v "$PWD":/src -w /src ept-dev ./build.sh
```

### External Dependencies
This project uses several third-party libraries located in the `thirdparty/` directory:
- **Qwt**: Used for high-quality plotting.
- **FFTW3**: Used for Fast Fourier Transforms.
- **libuv**: Used for asynchronous I/O.
- **qtmidi**: MIDI support (maintained as a submodule/local copy).

## Deployment

### Windows
1. Ensure the project is built in Release mode.
2. Run the `windeployqt` tool on the generated `bin/entropypianotuner.exe`.
3. Manually include `core.dll`, `fftw3.dll`, and the `algorithms/` folder in your deployment directory.

### Linux
The build produces `bin/entropypianotuner` alongside the shared libraries it
links against (`libcore`, `libqwt`, `libfftw3`). The binary's RUNPATH includes
`$ORIGIN`, so it finds those siblings without any environment setup — just run
`bin/entropypianotuner`. For a redistributable bundle, ship the contents of
`bin/` together with the `algorithms/` folder.