# 🖥️ CSE314 Operating System Sessional

[![Course](https://img.shields.io/badge/Course-CSE314-blue)](https://github.com/ahtasham67/CSE314-Operating-System-Sessional)
[![Language](https://img.shields.io/badge/Language-C%2B%2B%2FC%2FShell-green)](C)
[![Semester](https://img.shields.io/badge/Semester-Jan%202025-orange)](xv6)

> A comprehensive collection of operating system concepts implementation and assignments for CSE314 - Operating System Sessional course.

## 📋 Table of Contents

- [🖥️ CSE314 Operating System Sessional](#️-cse314-operating-system-sessional)
  - [📋 Table of Contents](#-table-of-contents)
  - [📁 Project Structure](#-project-structure)
  - [🎯 Assignments Overview](#-assignments-overview)
    - [🌅 Bash Online Assignment - Photo Curator](#-bash-online-assignment---photo-curator)
    - [📊 Offline 1 - Shell Scripting \& Program Analysis](#-offline-1---shell-scripting--program-analysis)
    - [🔧 Offline 2 - xv6 Operating System Enhancement](#-offline-2---xv6-operating-system-enhancement)
    - [🧵 Offline 3 - Inter-Process Communication (IPC)](#-offline-3---inter-process-communication-ipc)
  - [🛠️ Technologies Used](#️-technologies-used)
  - [⚡ Quick Start](#-quick-start)
    - [Prerequisites](#prerequisites)
    - [Running Assignments](#running-assignments)
    - [Bash Online - Travel Photo Curator](#bash-online---travel-photo-curator)
    - [Offline 1 - Automated Program Evaluation System](#offline-1---automated-program-evaluation-system)
    - [Offline 2 - xv6 System Call Enhancement](#offline-2---xv6-system-call-enhancement)
    - [Offline 3 - Multi-threaded Synchronization](#offline-3---multi-threaded-synchronization)
  - [🎨 Key Learning Outcomes](#-key-learning-outcomes)
  - [👨‍💻 Author](#-author)

## 📁 Project Structure

```
os/
├── 🌐 bash_online/                    # Online shell scripting assignment
│   ├── 2105067.sh                     # Photo categorization script
│   └── Travel Photo Curator/          # Assignment resources
│       ├── files.zip
│       └── Specification.pdf
│
├── 📝 offline1/                       # Shell scripting & program analysis
│   ├── CSE314_Jan_25_Shell_Offline_Specification.pdf
│   ├── Match/                         # Grading system results
│   │   └── targets/                   # Student submissions analysis
│   │       ├── result.csv             # Final grading results
│   │       ├── C/                     # C language submissions
│   │       ├── C++/                   # C++ language submissions
│   │       ├── Java/                  # Java language submissions
│   │       └── Python/                # Python language submissions
│   └── Workspace/                     # Main working directory
│       ├── organize.sh                # Automated grading script
│       ├── submissions/               # Student submission files
│       ├── tests/                     # Test cases
│       └── answers/                   # Expected outputs
│
├── 🔧 offline2/                       # xv6 Operating System Enhancement
│   ├── 2105067.patch                  # System modifications patch
│   └── Jan25_CSE314_Assignment2_Spec.pdf
│
└── 🧵 offline3/                       # Inter-Process Communication
    ├── main.cpp                       # Main simulation program
    ├── class.hpp                      # Class definitions
    ├── header.hpp                     # Header declarations
    ├── run.sh                         # Compilation & execution script
    └── CSE314_Jan_25_IPC_Offline.pdf
```

## 🎯 Assignments Overview

### 🌅 Bash Online Assignment - Photo Curator

**Objective**: Implement a shell script to automatically categorize travel photos based on timestamp

- **Technology**: Bash scripting
- **Complexity**: ⭐⭐
- **Key Skills**: File manipulation, string processing, conditional logic

### 📊 Offline 1 - Shell Scripting & Program Analysis

**Objective**: Create an automated system for evaluating and grading programming assignments

- **Technology**: Advanced Bash scripting
- **Complexity**: ⭐⭐⭐⭐
- **Key Skills**: Multi-language support, automated testing, report generation

### 🔧 Offline 2 - xv6 Operating System Enhancement

**Objective**: Extend the xv6 operating system with system call tracking and process management

- **Technology**: C, xv6 kernel programming
- **Complexity**: ⭐⭐⭐⭐⭐
- **Key Skills**: Kernel programming, system calls, process scheduling

### 🧵 Offline 3 - Inter-Process Communication (IPC)

**Objective**: Implement a multi-threaded simulation with complex synchronization

- **Technology**: C++, pthreads
- **Complexity**: ⭐⭐⭐⭐
- **Key Skills**: Multi-threading, synchronization, deadlock prevention

## 🛠️ Technologies Used

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Shell](https://img.shields.io/badge/Shell_Script-121011?style=for-the-badge&logo=gnu-bash&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

- **Programming Languages**: C++17, C, Bash
- **Operating Systems**: Linux, xv6
- **Concepts**: Multi-threading, IPC, System calls, Process management
- **Tools**: GCC, Make, Git, xv6 kernel

## ⚡ Quick Start

### Running Assignments

**Bash Online Assignment:**

```bash
cd bash_online
chmod +x 2105067.sh
./2105067.sh
```

**Offline 1 - Grading System:**

```bash
cd offline1/Workspace
chmod +x organize.sh
./organize.sh submissions targets tests answers [options]
```

**Offline 2 - xv6 Enhancement:**

```bash
cd offline2
# Apply patch to xv6 source code
git apply 2105067.patch
make qemu
```

**Offline 3 - IPC Simulation:**

```bash
cd offline3
chmod +x run.sh
./run.sh
```

## 🎨 Key Learning Outcomes

| Assignment      | Core Concepts                              | Skills Developed                            |
| --------------- | ------------------------------------------ | ------------------------------------------- |
| **Bash Online** | Shell scripting, File processing           | Text manipulation, Logic implementation     |
| **Offline 1**   | Advanced scripting, Multi-language support | System automation, Report generation        |
| **Offline 2**   | Xv-6 Kernel programming, System calls      | Low-level programming, OS internals         |
| **Offline 3**   | Multi-threading, Synchronization           | Concurrent programming, Resource management |

## 👨‍💻 Author

**Ahtashamul Haque**

- 🎓 Student ID: 2105067
- 🏫 Department: Computer Science and Engineering, BUET
- 🐙 LEVEL-3 Term-1

---
