## Requirements

Builds are tested with the latest Ubuntu with GCC and Clang. Adapt package names to your system if needed.

- g++ or clang
- cmake
- git

## Developing

When developing, consider installing some of the following tools that are included in the build process. They are optional, but help to maintain a good code quality and consistent formatting.

- clang-format (system package or https://github.com/llvm/llvm-project)
- clang-tidy (system package or https://github.com/llvm/llvm-project)
- Doxygen (system package or https://github.com/doxygen/doxygen)

## Building

First, checkout the code from the GitHub repository.

```shell
git clone https://github.com/Tasemo/dipp.git
cd dipp
```

When developing or testing, build the code using Debug mode.

```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --parallel $(nproc)
```

For deployments or benchmarks, remember to use the Release mode to enable compiler optimizations.

```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel $(nproc)
```
