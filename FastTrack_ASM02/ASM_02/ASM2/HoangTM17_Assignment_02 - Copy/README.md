# **ASSIGMENT 2 - GOOGLETEST FRAMEWORK** 

## Project directory structure

```bash
├───API/
│   ├───inlcude
│   └───src
├───build/
├───convertXMLtoHTML/
├───convertXMLtoXLSX/
├───report/
│   ├───ReportTest.xml
│   ├───ReportTest.json
│   ├───ReportTest.html
│   └───ReportTest.xlsx
├───TestSuite/
│   ├───inlcude
│   └───src
├───Makefile
├───README.md
```

- **build**: Contains all object files (.o) and generated executable files. This directory stores the compiled object files and the final executable used to run the application or tests.

- **include**: Directory for user-defined header files. These header files declare functions, classes, and other entities needed by the source code files.

- **report**: Stores test execution reports in various formats, including XML, JSON, HTML, and XLSX. These reports provide detailed results and summaries of the test runs.

- **src**: Contains the main source code files for the application. This directory includes the implementation files that define the core functionality of the application.

- **TestSuite**: Source code for test cases

- **README.md**: The main documentation file for the project, offering an overview, setup instructions, usage details, and other relevant information.

- **Makefile**: Script used by the make tool to automate various build and test commands. It facilitates compiling code, linking object files, running tests, and generating reports.

## Table of Contents README

1. [Prerequisites](#Prerequisites)
2. [Installation](#Installation)
3. [Usage](#Usage)
4. [Description of Test Scenarios](#description-of-test-scenarios)
 

## 1. Prerequisites

Before you begin, make sure you have completed the following steps:

- **Download and setup [Python >= 3.12](https://www.python.org)**:
  Ensure you have Python version 3.12 or higher installed on your system.

- **Download and setup [GoogleTest Framework](https://github.com/google/googletest)**
  Follow the instructions provided in the GoogleTest repository to set up the framework for unit testing.

- **Download and setup [MinGW](https://sourceforge.net/projects/mingw-w64/)**:
  Install MinGW to provide a development environment for compiling your code. Follow the installation instructions on the MingW website.
- Ensure that you have a build environment that supports the `make` command


## 2. Installation

### 2.1. *How to install GoogleTest Framework to run test your application*
#### Step 1:
- Download and setup libraries from [GoogleTest Framework](https://github.com/google/googletest).


#### Step 2:
- Download [libgtest.a](https://drive.google.com/file/d/1m6tSmOJx2Z13e6CllE42ovdaEFdbbn47/view) and [libgtest_main.a](https://drive.google.com/file/d/17lThhJxvt6w_Ns4WRNxvD0cQgzQXb8Rd/view).
- Copy both of these files into the `lib` folder of MingW.
- Example path: `C:\mingw64\x86_64-w64-mingw32\lib`.
#### Step 3:
- Extract the GoogleTest Framework file: [googletest-1.15.2.zip](https://github.com/google/googletest/releases/tag/v1.15.2).
- Navigate to: `googletest → include → gtest`.
- Example path: `C:\Users\tranp\Downloads\googletest-1.15.2\googletest-1.15.2\googletest\include\gtest`.
- Copy the `gtest` folder and paste it into `C:\mingw64\x86_64-w64-mingw32\include`.

### 2.2. How to Install Python to Work with GoogleTest Framework

#### Step 1:
- Download the latest version of Python from the official website: [Python Downloads](https://www.python.org/downloads/).
- Run the installer and ensure you check the option **"Add Python to PATH"** during installation.

#### Step 2:
- After installation, open a command prompt and verify the installation with the following command:
```bash
  python --version
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

## 3. Usage
### 3.1. GoogleTest Assertion Usage

GoogleTest provides two types of assertions: **fatal** and **non-fatal**. Fatal assertions (`ASSERT_*`) cause the test to abort if they fail, while non-fatal assertions (`EXPECT_*`) allow the test to continue after the failure.

#### Types of Assertions:

#### 3.1.1. Basic Assertions
These assertions check whether a condition is `true` or `false`. You can use either `EXPECT_` or `ASSERT_` macros depending on whether you want the test to continue or stop upon failure.

- `EXPECT_TRUE(condition)` – Verifies that the condition is true.
- `EXPECT_FALSE(condition)` – Verifies that the condition is false.

Example:
```cpp
EXPECT_TRUE(value > 0);
EXPECT_FALSE(value == -1);
```
#### 3.1.2. Binary Comparison Assertions
These assertions compare two values. If the comparison fails, the test will record a failure and display both values for debugging purposes. Non-fatal assertions (`EXPECT_*`) allow the test to continue even after a failure.

```cpp
- EXPECT_EQ(expected, actual) – Expects that expected == actual.
- EXPECT_NE(val1, val2) – Expects that val1 != val2.
- EXPECT_LT(val1, val2) – Expects that val1 < val2.
- EXPECT_LE(val1, val2) – Expects that val1 <= val2.
- EXPECT_GT(val1, val2) – Expects that val1 > val2.
- EXPECT_GE(val1, val2) – Expects that val1 >= val2.
```

### Example:
```cpp
int a = 5;
int b = 10;

EXPECT_EQ(a + b, 15);  // Passes, because a + b == 15
EXPECT_NE(a, b);       // Passes, because a != b
EXPECT_LT(a, b);       // Passes, because a < b
EXPECT_LE(a, b);       // Passes, because a <= b
EXPECT_GT(b, a);       // Passes, because b > a
EXPECT_GE(b, a);       // Passes, because b >= a
```
### 3.2. Build Usage

**Ensure you have completed the installation of GoogleTest as described in the previous steps.**

#### Step 1: Build the source and test code, and link objects to create the executable
- To compile your project and the corresponding tests, run the following command:
```bash
make Build
```

#### Step 2: Run automation test and export all file reports XML s/ JSON / HTML / XLSX
```bash
make report
```

_**Note**_: 
- If you are unable to build the code or export reports using the provided commands, ensure you have completed all the prerequisites listed in section `1. Prerequisites`.
- Make sure that the MinGW `bin` directory is added to your system PATH. This allows your command line to recognize `make` and other MinGW tools. For example, on Windows, the path might be `C:\msys64\usr\bin`. 

Additionally, install the required Python libraries for running the `.py` scripts using the following commands:

```bash
pip install pandas
pip install xml
pip install tqdm
pip install openpyxl
pip install os
pip install pprint
```
Alternatively, you can use:

```bash
python3 -m pip install pandas
python3 -m pip install xml
python3 -m pip install tqdm
python3 -m pip install openpyxl
python3 -m pip install os
python3 -m pip install pprint
```
### Extra Commands

In addition to building and generating comprehensive reports, you can use the following commands to manage your project and reports:

1. **Clean all files in the `report` folder and `build` folder**
```bash
   make clean
 ```

2. Export XML test report
```bash
   make xml
 ```

3. Export JSON test report
```bash
   make json
 ```

4. Export HTML test report
```bash
   make html
 ```

5. Export XLSX test report
```bash
   make xlsx
 ```
 
## 4. Description of Test Scenarios

### Test Scenario 1: String Reversal Check

- **Step 1:** Check if `strOriginal` is the reverse of `strReverse`. Compare each character in `strOriginal` with the corresponding character in `strReverse` from the end. Specifically, for each position `indexCount` in `strOriginal`, ensure it matches the character at position `originalStrLen - 1U - indexCount` in `strReverse`. If any character does not match, return `FALSE`. If all characters match, return `TRUE`.

- **Step 2:** Use the `EXPECT_TRUE()` assertion to validate the result of the reversal check function. The test case passes if the function returns `TRUE`, indicating that the strings are indeed reverses of each other. If the function returns `FALSE`, the test case fails.

### Test Scenario 2: Prime Number Verification

- **Step 1:** Generate a list of 10 random numbers within the range [0, 100].

- **Step 2:** For each number in this list, determine if it is a prime number. A number is considered prime if it is greater than 1 and has no divisors other than 1 and itself. Return `TRUE` if the number is prime. Otherwise, return `FALSE`.

- **Step 3:** Apply the `EXPECT_TRUE()` assertion to check the result of the prime number verification function. The test case will pass if the function correctly identifies prime numbers as `TRUE`. If the function fails to identify a prime number correctly, or incorrectly identifies a non-prime number as prime, the test case will fail.
