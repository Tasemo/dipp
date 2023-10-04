## Requirements

Builds are regularly tested with the latest Ubuntu with GCC and Clang. Adapt package names to your system if needed. The following tools are needed for building.

- g++ or clang
- cmake
- git

Some dynamic libraries are also required at build and runtime.

- libcurl4-openssl-dev
- libuv1-dev
- zlib1g-dev
- libjpeg-turbo8-dev (recommended) or libjpeg-dev

## Developing

When developing, consider installing some of the following tools that are included in the build process. They are optional, but help to maintain a good code quality and consistent formatting.

- clang-format (system package or https://github.com/llvm/llvm-project)
- clang-tidy (system package or https://github.com/llvm/llvm-project)
- include-what-you-see (https://github.com/include-what-you-use/include-what-you-use)
- Doxygen (system package or https://github.com/doxygen/doxygen)

## Building

To get started, checkout the code from the GitHub repository.

```shell
git clone https://github.com/Tasemo/dipp.git
cd dipp
```

When developing or testing, build the code using Debug mode. You should also consider enabling either the address, memory or thread sanitizer for runtime error detection. The memory sanitizer is only available when compiling with Clang. The undefined sanitizer is disabled because of false positives with thrill. Since the buid takes a long time, a parallel build with all available cores is recommended.

```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DSANITIZE=Address
cmake --build build --config Debug --parallel $(nproc)
```

For deployments or benchmarks, remember to use the Release mode to enable compiler optimizations.

```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel $(nproc)
```

## Running

To control the behaviour of the application (e.g. the amount of workers per host), you have to use environment variables. A default configuration with an explanation for all values is [provided](configure.default.sh). Copy the file, make your changes and remember to source the file every time you make a new change.

```shell
cp configure.default.sh configure.sh
source configure.sh
```

Executables are created in the ./bin directory. It also contains a /tools subdirectory for profiling tools, a /deploy subdirectory for deployment scripts and a /tests directory for unit tests.

```shell
./bin/main
```
