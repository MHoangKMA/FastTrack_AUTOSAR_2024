# **ASSIGMENT 3 - Makefile** 

## Project directory structure

```bash
C:.
├───Assignment_03_Makefile       
│   ├───include
│   ├───output
│   │   ├───depend
│   │   ├───object
│   │   ├───S32K144_Demo.elf
│   │   └───S32K144_Demo.map
│   ├───Project_Settings
│   │   ├───Linker_Files
│   │   └───Startup_Code
│   └───src
│       ├───App
│       │   └───LED
│       ├───Drivers
│       │   ├───CAN
│       │   ├───Clock
│       │   ├───GPIO
│       │   ├───Interrupt
│       │   ├───PIT
│       │   ├───PMC
│       │   ├───RTC
│       │   └───WDog
│       └───System
│           ├───Init
│           ├───Sch
│           └───Task
│   ├───build_log_FLASH.txt
│   ├───build_log_RAM.txt
│   ├───Makefile
│   └───README.md
└───TestCase
```


### include
This folder contains the header files for the S32K144 board. These header files are crucial for defining the board's interfaces and functionalities.

### output
The `output` folder is where files generated from the compilation process are stored. It consists of two subdirectories:
- **depend**: This subdirectory contains dependency files with a `.d` extension, which are used by the make system to track file dependencies during the build process.
- **object**: This subdirectory contains object files with a `.c` extension, which are intermediate files generated after the source code has been compiled.

### Project setting
This folder includes project-specific settings, particularly for the linker and startup code. It contains two subfolders:
- **Linker_files**: This folder contains two linker scripts with the `.ld` extension. These scripts are essential for generating the `.elf` file, which is used to flash the program onto the board. There are two modes supported:
  - **RAM**: Used to flash the code onto RAM for quick debugging and testing.
  - **FLASH**: Used to flash the code onto the board’s FLASH memory for final deployment.
- **Startup_Code**: This folder contains the startup code for the S32K144 board. The startup code initializes the microcontroller’s system before executing the main application.

### src
The `src` directory holds the main source code for the project and is organized into several subdirectories:
- **APP**: This folder contains the application-specific source code, such as the code controlling the LED on the S32K144 board.
- **Drivers**: This directory is divided into subdirectories, each containing driver code for different peripherals:
  - **CAN**: Code for the Controller Area Network driver.
  - **Clock**: Code for clock configuration and management.
  - **GPIO**: Code for General-Purpose Input/Output control.
  - **Interrupt**: Code for managing interrupts.
  - **PIT**: Code for the Periodic Interrupt Timer driver.
  - **PMC**: Code for Power Management Controller.
  - **RTC**: Code for Real-Time Clock driver.
  - **WDog**: Code for the Watchdog timer.
- **System**: This folder contains system-related code and is divided into the following subdirectories:
  - **Init**: Code responsible for system initialization.
  - **Sch**: Code for the system scheduler.
  - **Task**: Code for task management.
- **main.c**: The main entry point of the project. It contains the core logic that ties together the drivers, system, and application code.

### Testcase
This folder contains test suites for both **RAM** and **FLASH** modes. These test suites are used to validate the functionality of the project in both deployment scenarios:
- **RAM** test suite: Focuses on testing the code running in RAM, often used for debugging purposes.
- **FLASH** test suite: Focuses on testing the code running from the board's FLASH memory, used for final product testing and validation.

## Objective
The primary goal of this project is to create a Makefile that automates the build process and generates the following:
- A flashing file in the `.elf` format, which can be used to upload the compiled program to the S32K144 board.
- A `.map` file, which provides detailed memory usage information.

The Makefile should support two different modes for flashing the program onto the S32K144 board:
- **RAM mode**: This mode allows for the quick loading of the program into RAM for debugging and testing purposes.
- **FLASH mode**: This mode compiles and prepares the program for final deployment onto the board’s FLASH memory.

---

This README provides a detailed overview of the project structure and objectives. The Makefile will serve as the main tool for building and deploying the project in both development and production environments.


## Table of Contents README
Setting compiler environment variable
1. [Prerequisites](#1-prerequisites)
2. [Installation](#2-installation)
3. [Setup Compiler Environment](#3-setup-environment-compiler)
4. [Check build log ](#4-check-build-log)
 
## 1. Prerequisites

Before you begin, make sure you have completed the following steps:

- **Download and setup [Cygwin](https://www.cygwin.com/install.html)**:
  Ensure you installed Cygwin on your system.

- **Download and setup [S32 Design Studio for S32 Platform](https://www.nxp.com/design/design-center/software/automotive-software-and-tools/s32-design-studio-ide/s32-design-studio-for-s32-platform:S32DS-S32PLATFORM)**
If you have downloaded S32DS, you can also use the GCC version included in the S32DS installer
  + C:/NXP/S32DS.3.5/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi

- **Download and set up [GCC11-4_eARMv7](https://cache.nxp.com/secured/updates/S32DS/NXP_GCC11.4_eARMv7_gf703eb2.exe?fileExt=.exe)**:
  If you prefer not to use the GCC Compiler provided by S32 Design Studio (S32DS), you can install the standalone GCC11-4_eARMv7 toolchain using the this link.

- **Download and setup [MinGW](https://sourceforge.net/projects/mingw-w64/)**:
  Install MinGW to provide a development environment for compiling your code. Follow the installation instructions on the MingW website.
- Ensure that you have a build environment that supports the `make` command


## 2. Installation

### 2.1. *How to install S32 Design Studio for S32 Platform*
#### Step 1:
- Download from [S32 Design Studio for S32 Platform](https://www.nxp.com/design/design-center/software/automotive-software-and-tools/s32-design-studio-ide/s32-design-studio-for-s32-platform:S32DS-S32PLATFORM).


#### Step 2:
- Follow the on-screen instructions provided by the installer to complete the setup process.

#### Step 3:
- In the License step, you can retrieve the License Key from the NXP website for the version you downloaded in Step 1. During installation, enter the License Key and select the "Activate Online" option.


### 2.2. How to Install GCC11-4_eARMv7 toolchain 

#### Step 1:
- Download the version of GCC11-4_eARMv7 from the website: [NXP](https://cache.nxp.com/secured/updates/S32DS/NXP_GCC11.4_eARMv7_gf703eb2.exe?fileExt=.exe).
- Download and follow the installation prompts to complete the setup.

#### Step 2:
- After installation, open a command prompt and verify the installation with the following command:
```bash
  gcc --version
```
### 2.3. How to Install `make` command 
  To use the `make` command, follow these instructions based on your operating system:

  ### For Windows:
  1. **Install [MSYS2](https://www.msys2.org/)**
     - Download and run the MSYS2 installer from the official website.
     - Follow the installation instructions and update the package database with the following commands:
       ```bash
       pacman -Syu
       ```
       After the initial update, close the terminal and reopen it to continue updating:
       ```bash
       pacman -Su
       ```

  2. **Install `make`**
     - Open the MSYS2 terminal and run:
       ```bash
       pacman -S make
       ```

  3. **Add MSYS2 to your system PATH**
     - Go to System Properties → Environment Variables.
     - Edit the `Path` variable and add the path to the MSYS2 `bin` directory (e.g., `C:\msys64\usr\bin`).

  ### For macOS:
  1. **Install Xcode Command Line Tools**
     - Open Terminal and run:
       ```bash
       xcode-select --install
       ```

  2. **Install `make` (if not already installed)**
     - `make` is included with Xcode Command Line Tools, so this step is typically not required. However, you can install GNU `make` using Homebrew if needed:
       ```bash
       brew install make
       ```

  ### For Linux:
  1. **Install `make` using your package manager**
     - On Debian-based systems (e.g., Ubuntu), run:
       ```bash
       sudo apt-get update
       sudo apt-get install make
       ```

     - On Red Hat-based systems (e.g., Fedora), run:
       ```bash
       sudo dnf install make
       ```

  After installing `make`, ensure it is accessible from your command line by running:
  ```bash
  make --version
  ```

_**Note**_: 
- When installing Cygwin, In the interface select the package you can to select install `make`. 

### 2.1. *How to install Cygwin*
#### Step 1:
- Download the version of Cygwin from the website: [Cygwin](https://www.cygwin.com/install.html).
- Download and follow the installation prompts to complete the setup.

#### Step 2:
Select `All` to show all packages and then choose the necessary packages for this project, which include:

- `bash`
- `m4`
- `make`
- `mintty`
- `nc`
- `which`

#### Step 3:
Verify the installation of the above packages by running the following command:

```bash
cygcheck --check-setup
```


## 3. Setup Environment Compiler
### 3.1. Setup Environment Compiler with S32 Design Studio for S32 Platform

This assignment will provide instructions on how to configure the `GCC ARM version 11.4 compiler path` in your environment.

#### Step 1: Open the directory that contains the entire project.

**Example:**
```bash
$ C:\Users\tranp\Downloads\FastTrack_24\Assignment\ASM03
```
#### Step 2: Open Git bash and type
```bash
$ export GCC_DIR=YOUR_COMPILER_PATH
```
**Example:**
```bash
$ export GCC_DIR=/c/NXP/S32DS.3.5/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi
```
**You can check through:** `make print-GCC_DIR`
```bash
$ make print-GCC_DIR
C:/NXP/S32DS.3.4/S32DS/build_tools/gcc_v11.4/gcc-11.4-arm32-eabi
```

### 3.2. Setup Environment Compiler with SGCC11-4_eARMv7 toolchain

This assignment will provide instructions on how to configure the `GCC ARM version 11.4 compiler path` in your environment.

#### Step 1: Open the directory that contains the entire project.

**Example:**
```bash
$ C:\Users\tranp\Downloads\FastTrack_24\Assignment\ASM03
```
#### Step 2: Open Git bash and type
```bash
$ export GCC_DIR=YOUR_COMPILER_PATH
```
**Example:**
```bash
$ export GCC_DIR=/d/GCC/arm-none-eabi
```
**You can check through:** `make print-GCC_DIR`
```bash
$ make print-GCC_DIR
D:/GCC/arm-none-eabi
```
## 4. Build Usage

**Ensure you have completed the installation of Cygwin or MinGW as described in the previous steps.**

#### Step 1: Setup Environment Compiler
  - To successfully build the project, we need to complete Step 3: Setup Environment Compiler as outlined in the first step.


#### Step 2: To generate the `.elf` flashing file for the board and the `.map` file when programming in RAM, you need to enter the following command:

```bash
make build LOAD_TO=RAM
```

**Example when using `make build LOAD_TO=RAM`:**
```bash
$ make clean
===================================================================
==> Cleaning Fail! No output directory found.

tranp@DESKTOP-BAHVMEH MINGW64 ~/Downloads/FastTrack_24/Assignment/ASM03/Assignment_03_Makefile
$ make build LOAD_TO=RAM
===================================================================
COMPILING C FILE:
Compiling.... ./Project_Settings/Startup_Code/startup.c
COMPILING Completed! : ./output/object/startup.o
===================================================================
COMPILING C FILE:
Compiling.... ./Project_Settings/Startup_Code/system_S32K144.c
COMPILING Completed! : ./output/object/system_S32K144.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/main.c
COMPILING Completed! : ./output/object/main.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/App/LED/LED.c
COMPILING Completed! : ./output/object/LED.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/CAN/Can.c
COMPILING Completed! : ./output/object/Can.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/Clock/Clock.c
COMPILING Completed! : ./output/object/Clock.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/GPIO/GPIO.c
COMPILING Completed! : ./output/object/GPIO.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/Interrupt/interrupt_manager.c
COMPILING Completed! : ./output/object/interrupt_manager.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/PIT/LPIT.c
COMPILING Completed! : ./output/object/LPIT.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/PMC/PMC.c
COMPILING Completed! : ./output/object/PMC.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/RTC/RTC.c
COMPILING Completed! : ./output/object/RTC.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/WDog/WDog.c
COMPILING Completed! : ./output/object/WDog.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/System/Init/Init.c
COMPILING Completed! : ./output/object/Init.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/System/Sch/Sch.c
COMPILING Completed! : ./output/object/Sch.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/System/Task/Task.c
COMPILING Completed! : ./output/object/Task.o
===================================================================
COMPILING ASSEMBLER FILE:
Compiling.... ./Project_Settings/Startup_Code/startup_S32K144.S
COMPILING Completed! : ./output/object/startup_S32K144.o
===================================================================
Invoking the Standard S32DS C Linker...
Linking the object files to generate the executable at: ./output/S32K144_Demo.elf

```

#### Step 3: To generate the `.elf` flashing file for the board and the `.map` file when programming in FLASH, you need to enter the following command:

```bash
make build LOAD_TO=FLASH
```
**Example when using `make build LOAD_TO=FLASH`:**
```bash
$ make build LOAD_TO=FLASH
===================================================================
COMPILING C FILE:
Compiling.... ./Project_Settings/Startup_Code/startup.c
COMPILING Completed! : ./output/object/startup.o
===================================================================
COMPILING C FILE:
Compiling.... ./Project_Settings/Startup_Code/system_S32K144.c
COMPILING Completed! : ./output/object/system_S32K144.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/main.c
COMPILING Completed! : ./output/object/main.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/App/LED/LED.c
COMPILING Completed! : ./output/object/LED.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/CAN/Can.c
COMPILING Completed! : ./output/object/Can.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/Clock/Clock.c
COMPILING Completed! : ./output/object/Clock.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/GPIO/GPIO.c
COMPILING Completed! : ./output/object/GPIO.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/Interrupt/interrupt_manager.c
COMPILING Completed! : ./output/object/interrupt_manager.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/PIT/LPIT.c
COMPILING Completed! : ./output/object/LPIT.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/PMC/PMC.c
COMPILING Completed! : ./output/object/PMC.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/RTC/RTC.c
COMPILING Completed! : ./output/object/RTC.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/Drivers/WDog/WDog.c
COMPILING Completed! : ./output/object/WDog.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/System/Init/Init.c
COMPILING Completed! : ./output/object/Init.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/System/Sch/Sch.c
COMPILING Completed! : ./output/object/Sch.o
===================================================================
COMPILING C FILE:
Compiling.... ./src/System/Task/Task.c
COMPILING Completed! : ./output/object/Task.o
===================================================================
COMPILING ASSEMBLER FILE:
Compiling.... ./Project_Settings/Startup_Code/startup_S32K144.S
COMPILING Completed! : ./output/object/startup_S32K144.o
===================================================================
Invoking the Standard S32DS C Linker...
Linking the object files to generate the executable at: ./output/S32K144_Demo.elf
```
_**Note**_: 
- If you are unable to build the code or export reports using the provided commands, ensure you have completed all the prerequisites listed in section `1. Prerequisites`.
- Make sure that the MinGW `bin` directory is added to your system PATH. This allows your command line to recognize `make` and other MinGW tools. For example, on Windows, the path might be `C:\msys64\usr\bin`. 
- Make sure that the Cygwin is installed.
- Make sure installed `make`.

### Extra Commands

In addition to building and generating comprehensive reports, you can use the following commands to manage your project and reports:

**Clean all files in the `output` folder and delete folder**
```bash
   make clean
 ```
**Example when using `make clean`:**
```bash
$ make clean
===================================================================
==> Cleaning the following files and directories:
./output
./output/depend
./output/depend/Can.d
./output/depend/Clock.d
./output/depend/GPIO.d
./output/depend/Init.d
./output/depend/interrupt_manager.d
./output/depend/LED.d
./output/depend/LPIT.d
./output/depend/main.d
./output/depend/PMC.d
./output/depend/RTC.d
./output/depend/Sch.d
./output/depend/startup.d
./output/depend/startup_S32K144.d
./output/depend/system_S32K144.d
./output/depend/Task.d
./output/depend/WDog.d
./output/object
./output/object/Can.o
./output/object/Clock.o
./output/object/GPIO.o
./output/object/Init.o
./output/object/interrupt_manager.o
./output/object/LED.o
./output/object/LPIT.o
./output/object/main.o
./output/object/PMC.o
./output/object/RTC.o
./output/object/Sch.o
./output/object/startup.o
./output/object/startup_S32K144.o
./output/object/system_S32K144.o
./output/object/Task.o
./output/object/WDog.o
./output/S32K144_Demo.elf
./output/S32K144_Demo.map
==> Cleaning... Clean ALL Completed!
```

- If `output` folder empty or `output` folder not exist, nothing to delete. It will be displayed:

```bash
$ make clean
===================================================================
==> Cleaning Fail! No output directory found.
```

_**Note**_: 
- It is recommended to run the `make clean` command before building the `.elf` flashing file in both RAM and FLASH modes to remove any old files before the new build.

## 4. Check build log 

### 4.1. Check build log load to FLASH

- **Step 1:** Open the file named `build_log_FLASH.txt`, which is generated after each run of the `make build LOAD_TO=FLASH` command.

- **Step 2:** Check the build log to verify which `.c` files were successfully compiled and identify any missing files. Review this information in the `log_build_FLASH.txt` file.
  - You can also check whether the `.elf` and `.map` files were successfully built in this log.

```bash
COMPILING Completed! : ./output/object/startup.o
COMPILING Completed! : ./output/object/system_S32K144.o
COMPILING Completed! : ./output/object/main.o
COMPILING Completed! : ./output/object/LED.o
COMPILING Completed! : ./output/object/Can.o
COMPILING Completed! : ./output/object/Clock.o
COMPILING Completed! : ./output/object/GPIO.o
COMPILING Completed! : ./output/object/interrupt_manager.o
COMPILING Completed! : ./output/object/LPIT.o
COMPILING Completed! : ./output/object/PMC.o
COMPILING Completed! : ./output/object/RTC.o
COMPILING Completed! : ./output/object/WDog.o
COMPILING Completed! : ./output/object/Init.o
COMPILING Completed! : ./output/object/Sch.o
COMPILING Completed! : ./output/object/Task.o
COMPILING Completed! : ./output/object/startup_S32K144.o
==> BUILD on FLASH Complete!
Generated ELF file: ./output/S32K144_Demo.elf
Generated MAP file: ./output/S32K144_Demo.map
```

### 4.1. Check build log load to RAM

- **Step 1:** Open the file named `build_log_RAM.txt`, which is generated after each run of the `make build LOAD_TO=RAM` command.

- **Step 2:** Check the build log to verify which `.c` files were successfully compiled and identify any missing files. Review this information in the `log_build_RAM.txt` file.
  - You can also check whether the `.elf` and `.map` files were successfully built in this log.

```bash
COMPILING Completed! : ./output/object/startup.o
COMPILING Completed! : ./output/object/system_S32K144.o
COMPILING Completed! : ./output/object/main.o
COMPILING Completed! : ./output/object/LED.o
COMPILING Completed! : ./output/object/Can.o
COMPILING Completed! : ./output/object/Clock.o
COMPILING Completed! : ./output/object/GPIO.o
COMPILING Completed! : ./output/object/interrupt_manager.o
COMPILING Completed! : ./output/object/LPIT.o
COMPILING Completed! : ./output/object/PMC.o
COMPILING Completed! : ./output/object/RTC.o
COMPILING Completed! : ./output/object/WDog.o
COMPILING Completed! : ./output/object/Init.o
COMPILING Completed! : ./output/object/Sch.o
COMPILING Completed! : ./output/object/Task.o
COMPILING Completed! : ./output/object/startup_S32K144.o
==> BUILD on RAM Complete!
Generated ELF file: ./output/S32K144_Demo.elf
Generated MAP file: ./output/S32K144_Demo.map
```

