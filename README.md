# cpptrace_th: Customizable terminate handler register for [cpptrace](https://github.com/jeremy-rifkin/cpptrace)

## Features

- Automatically register a terminate handler that prints a stack trace (provided by cpptrace) when a program terminates unexpectedly.
- Customizable stack trace output format.
  - Option to print code snippets around the stack trace. (Default: ON)
  - Option to skip stackframes from the top of the stack trace by regex matching. (Default: skip till `std::terminate`)
- Option to disable zstd prerequisite in cpptrace. (Default: ON)

## Usage

```cmake
FetchContent_Declare(
  cpptrace_th
  GIT_REPOSITORY "https://github.com/Lei-workshop/cpptrace_th"
  GIT_TAG "main"
  GIT_SHALLOW ON
  GIT_PROGRESS TRUE
  SYSTEM)
FetchContent_MakeAvailable(cpptrace_th)

target_link_libraries(your_target PRIVATE cpptrace_th::object)
# OR
target_link_libraries(your_target PRIVATE cpptrace_th::shared)
```

## Customization

cpptrace_th can be customized by cmake variables (compile-time) or by setting environment variables (run-time). Run-time environment variables take precedence over compile-time cmake variables.

| variable                  | explanation                                            | default            | cmake | environment |
| ------------------------- | ------------------------------------------------------ | ------------------ | ----- | ----------- |
| CPPTRACE_TH_BUILD_EXAMPLE | Print code snippets in trace                           | ON                 | ✓     | ✓           |
| CPPTRACE_TH_ENABLE_SKIP   | Enable skip regex                                      | ON                 | ✓     | ✓           |
| CPPTRACE_TH_SKIP_REGEX    | Frames before matched regex (included) will be omitted | \bstd::terminate\b | ✓     | ✓           |
| CPPTRACE_TH_NO_ZSTD       | Remove ZSTD support (speedup build)                    | ON                 | ✓     |             |
