# Building BDE tests with Circle

The latest public executable of Circle [build 98](http://circle-lang.org/linux/build_98.tgz) compiles and runs all tests in BDE on this branch (https://github.com/seanbaxter/bde branch circle_dev). This patch set adds a couple of Circle-specific additions and workarounds. The only extensive change is a unique code path to support the `NameOf` operator, which requires compiler-specific logic.

To build and run the tests:

1. **Clone the [Circle bde-tools fork](https://github.com/seanbaxter/bde-tools/tree/circle_dev) and add its /bin to your path**

The fork is only 4 additional lines in bde_ufid.cmake to declare the compiler.

2. **Add Circle as a recognized BDE C++ compiler by inserting this text into ~/.bdecompilerconfig**

```
[
    {
        "uplid": "unix-linux-",
        "compilers": [
            {
                "type":      "circle",
                "c_path":    "/usr/bin/clang-8",
                "cxx_path":  "/usr/bin/circle",
                "version":   "1",
                "toolchain": "circle-default"
            }
        ]
    }
]
```

3. **Install the Circle branch for CMake support**

https://github.com/seanbaxter/cmake/tree/circle-compiler

```
$ git clone https://github.com/seanbaxter/cmake/tree/circle-compiler ~/cmake
$ cd ~/cmake
$ ./bootstrap && make && sudo make install
```

4. **Choose a UFID build environment**

```
$ eval `bde_build_env.py -t dbg_exc_mt_64`
Using user configuration: /home/sean/.bdecompilerconfig
Using compiler: circle-1
Using ufid: dbg_exc_mt_64
Using install directory: /home/sean/projects/bde2/_install
```

5. **Configure the BDE build**

```
$ cd ~/bde
$ cmake_build.py -G Ninja configure --clean
Configuration cmd:
cmake /home/sean/projects/bde2 -G Ninja -DCMAKE_MODULE_PATH:PATH=/home/sean/projects/bde-tools/cmake -DUFID:STRING=dbg_exc_mt_64 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBDE_LOG_LEVEL=QUIET -DBUILD_BITNESS=64 -DBDE_USE_WAFSTYLEOUT=OFF -DCMAKE_INSTALL_PREFIX=/opt/bb -DCMAKE_INSTALL_LIBDIR=lib64 -DBDE_TEST_REGEX= -DCMAKE_TOOLCHAIN_FILE=/home/sean/projects/bde-tools/cmake/toolchains/linux/circle-default.cmake -DCMAKE_PREFIX_PATH:PATH=///opt/bb
-- The C compiler identification is Clang 8.0.1
-- The CXX compiler identification is Circle 1.0.98
-- Check for working C compiler: /usr/bin/clang-8
-- Check for working C compiler: /usr/bin/clang-8 - works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/circle
-- Check for working CXX compiler: /usr/bin/circle - works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found PkgConfig: /usr/bin/pkg-config (found version "0.29.1") 
-- Found PythonInterp: /usr/bin/python (found version "2.7.15") 
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE  
-- Check if the system is big endian
-- Searching 16 bit integer
-- Looking for sys/types.h
-- Looking for sys/types.h - found
-- Looking for stdint.h
-- Looking for stdint.h - found
-- Looking for stddef.h
-- Looking for stddef.h - found
-- Check size of unsigned short
-- Check size of unsigned short - done
-- Searching 16 bit integer - Using unsigned short
-- Check if the system is big endian - little endian
-- Configuring done
-- Generating done
CMake Warning:
  Manually-specified variables were not used by the project:

    BUILD_BITNESS


-- Build files have been written to: /home/sean/projects/bde2/_build/unix-linux-x86_64-4.15.0-circle-1-dbg_exc_mt_64
```

6. **Compile and run all tests**

```
$ cmake_build.py --timeout=150 -j6 --tests=run build
[3002/3002] Linking CXX executable bdlt_packedcalendar.t
Test project /home/sean/projects/bde2/_build/unix-linux-x86_64-4.15.0-circle-1-dbg_exc_mt_64
        Start   1: balb_controlmanager.t
        Start   2: balb_filecleanerconfiguration.t
  ...
882/883 Test #824: bslstl_vector_test1.t ..............................   Passed    8.86 sec
883/883 Test #763: bslstl_hashtable.t .................................   Passed   42.52 sec

100% tests passed, 0 tests failed out of 883

Total Test time (real) = 193.05 sec
```
