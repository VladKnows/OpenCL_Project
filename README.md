# OpenCL Modular Framework

## 📖 Overview
This project presents a **modular C++ framework** designed to simplify the development and execution of **OpenCL** applications. It provides an accessible solution for running computational kernels across different hardware architectures.

Developed as part of a Bachelor's Thesis, this framework aims to streamline OpenCL usage in large-scale projects where direct interaction with the raw API can be complex and repetitive.

### Key Objectives
* **Unified System:** Manages all necessary OpenCL components (platform/device selection, source code compilation, memory allocation, and kernel execution).
* **Minimal Configuration:** Allows defining and running chains of kernels with minimal user setup.
* **Abstraction:** Uses extensible abstract classes to handle platforms, programs, buffers, and kernel arguments.
* **Portability:** Tested on diverse hardware including NVIDIA RTX 4060, Intel UHD Graphics, and Mali-G610.

---

## 🚀 Algorithms Implemented
The `main` entry point includes default test cases for 5 parallel algorithms to demonstrate performance and compatibility:

1.  **Vector Addition**
2.  **Vector Reduction** (Sum of elements)
3.  **Matrix Multiplication**
4.  **FIR Filter** (Finite Impulse Response)
5.  **DCT4** (Discrete Cosine Transform)

---

## 🛠️ Prerequisites

Before running the project, ensure you have the following installed:
* **C++ Compiler:** Clang.
* **Build System:** CMake.
* **Package Manager:** [vcpkg](https://github.com/microsoft/vcpkg).
* **IDE:** Visual Studio Code (with C++ and CMake Tools extensions).

---

## ⚙️ Configuration & Installation

### 1. Windows Setup

1.  **Environment Check:** Ensure `vcpkg` and `LLVM (Clang)` are installed.
2.  **VS Code Configuration:**
    * Open `.vscode/c_cpp_properties.json`.
    * Update `includePath` to point to your local vcpkg folder (e.g., `C:/tools/vcpkg/installed/x64-windows/include`).
    * Update `compilerPath` to your Clang executable (e.g., `C:/Program Files/LLVM/bin/clang++.exe`).
3.  **CMake Configuration (Critical):**
    * Open VS Code Settings (`Ctrl + ,`).
    * Search for **"CMake: Configure Args"**.
    * Add the following item to fix the Resource Compiler path:
        ```text
        -DCMAKE_RC_COMPILER=C:/Program Files/LLVM/bin/llvm-rc.exe
        ```
        *(Adjust the path if you installed LLVM in a different location).*

### 2. Linux Setup

1.  **Environment Check:** Ensure `vcpkg` and `clang` are installed via your package manager.
2.  **VS Code Configuration:**
    * Open `.vscode/c_cpp_properties.json`.
    * Update `includePath` to your vcpkg installation (default usually `/opt/vcpkg/installed/arm64-linux/include` or `x64-linux`).
    * Update `compilerPath` to your Clang executable (e.g., `"/usr/bin/clang++"`).

---

## ▶️ How to Run

The project is configured to run via **Visual Studio Code** using the CMake Tools extension.

1.  Open the project folder in VS Code.
2.  Select your Build Kit (Clang) from the status bar.
3.  **Build:** Press the **Build** button (or `F7`) to compile the project.
4.  **Run:** Press the **Run** button (Play icon) or `Ctrl + F5`.

The application will execute the 5 default algorithms and output the results/performance metrics in the terminal.
