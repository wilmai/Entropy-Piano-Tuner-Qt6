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
- **C++ Compiler**: MSVC 2022 (Windows)
- **Visual Studio Build Tools 2022**

### Windows Build Instructions
A automated build script is provided to simplify the process:

1.  **Build MIDI Dependency**:
    ```powershell
    .\build_qtmidi.bat
    ```
2.  **Build Main Project**:
    ```powershell
    .\build_project.bat
    ```

The compiled binaries will be located in the `bin/` directory.

### External Dependencies
This project uses several third-party libraries located in the `thirdparty/` directory:
- **Qwt**: Used for high-quality plotting.
- **FFTW3**: Used for Fast Fourier Transforms.
- **libuv**: Used for asynchronous I/O.
- **qtmidi**: MIDI support (maintained as a submodule/local copy).

## Deployment
To create a standalone deployment of the application:
1. Ensure the project is built in Release mode.
2. Run the `windeployqt` tool on the generated `bin/entropypianotuner.exe`.
3. Manually include `core.dll`, `fftw3.dll`, and the `algorithms/` folder in your deployment directory.