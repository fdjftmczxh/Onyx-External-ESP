# Onyx External ESP

[![Android](https://img.shields.io/badge/Platform-Android-green.svg)](https://developer.android.com/)
[![Arch](https://img.shields.io/badge/Arch-x86__64-orange.svg)](#)
[![Target](https://img.shields.io/badge/Version-1.60.0.f3153-red.svg)](#)

---

## Target Specification

This project has been built and tested for a specific emulation environment:
* **Architecture:** Android x86_64
* **Emulator:** MuMu Player (global)

**Note:** Support for physical Android devices (ARM architectures) is currently lacking. However, bringing compatibility to real, rooted physical devices is on the roadmap and will be worked on in future updates.

---

## Project Status

This project serves as an educational foundation rather than a finished product. Due to the game's obfuscation, several features, such as player names and team identification, are currently missing. The primary goal is to provide a clean, accessible starting point that is easy to understand, study, and modify.

At present, the project includes a core set of ESP features:

* Skeleton
* Snaplines
* Bounding Boxes (2D & 3D)
* Health
* Distance
* Off-screen Indicators

[Preview of ESP (streamable)](https://streamable.com/lkwjmm)

For any questions, joing my Discord server: https://discord.gg/q6cra6wUQA

---

## Build & Execution Instructions

This project includes automated batch scripts (`build.bat` and `push.bat`) to handle building and deploying the executable on Windows.

### Prerequisites
Before building, make sure you have the following ready on your Windows machine:
1. **[Android NDK](https://developer.android.com/ndk/downloads)** - Required for cross-compiling C++ for Android.
2. **ADB (Android Debug Bridge)** - Required to push the compiled binary to your device.
3. **Emulator Configuration** - Ensure your MuMu Player (or equivalent emulator) is running, rooted, and connected to ADB.

### 1. Initial Setup & Cloning
First, clone the repository:
```cmd
git clone https://github.com/fdjftmczxh/Onyx-External-ESP.git
cd Onyx-External-ESP
```

### 2. Compiling the Project (`build.bat`)
Before running the build script, you must point it to your Android NDK installation:
1. Open `build.bat` in a text editor.
2. Locate the configuration section at the top and edit the `ANDROID_NDK_HOME` variable to match your system (e.g., `C:\Users\YOUR_NAME\AppData\Local\Android\Sdk\ndk\30.0.x`).
3. Save and close the file.

To compile the project, simply double-click `build.bat` or run it from your terminal:
```cmd
build.bat
```
*(Note: If you encounter errors or need to rebuild from scratch, run `build.bat clean` to wipe out the old CMake cache).*

### 3. Pushing & Running the Project (`push.bat`)
Once the build is successful, you need to configure the push script:
1. Open `push.bat` in a text editor.
2. Edit the `ADB_PATH` variable to match the location of your Android SDK `platform-tools` folder.
3. Save and close the file.

Then run:
```cmd
push.bat
```
This script will automatically:
* Verify root access on your emulator.
* Safely terminate any previously running instances of the program.
* Push the newly compiled `onyx_external` binary to `/data/local/tmp`.
* Grant the necessary execute permissions.
* Launch the project as root.

---

## Credits

This project uses several community projects:

- **[KittyMemoryEx](https://github.com/MJx0/KittyMemoryEx)** by [MJx0](https://github.com/MJx0) — Memory scanning and pattern finding for Android.
- **[AImGUI](https://github.com/Bzi-Han/AndroidSurfaceImgui)** by [Bzi-Han](https://github.com/Bzi-Han) — External Android rendering framework.

---
