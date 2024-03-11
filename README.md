# BB4fhd
This repository contains the open-source code for **A Branch-&-Bound Algorithm for Fractional Hypertree
Decomposition**

### 1. Setup

#### 1.1 Install GLPK library

```bash
wget http://ftp.gnu.org/gnu/glpk/glpk-4.55.tar.gz
tar -zxvf glpk-4.55.tar.gz
cd glpk-4.55
./configure --prefix=/path/to/install
make
make install
```

### 1.2 Build
edit the flags in [Makefile](./src/C++/Makefile):
```Makefile
LDFLAGS:=-L/path/to/install/lib/
INCLUDE:= -I/path/to/install/include/
# if use GNU
CC:=g++ 
# if use Clang
CC:=clang++
```

build project:

```bash
cd src
export LD_LIBRARY_PATH=/path/to/install/lib
make exe
```

### 2. How to Run
#### 2.1 Simple test
```bash
./main
```

The program will solve the instance in the folder sample/, and the result will be saved in result/

#### 2.2 Custom data test
Change the line in global_setting.h:
```C++
const char * const result_prefix = "../result";
...
const char * const data_dir = "../sample";
```
re-build the project
