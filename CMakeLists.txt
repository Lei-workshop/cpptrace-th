cmake_minimum_required(VERSION 3.25)
project(cpptrace_th CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---------- options ----------
option(CPPTRACE_TH_BUILD_EXAMPLE "Build examples" OFF)
option(CPPTRACE_TH_PRINT_SNIPPETS "Print code snippets in trace" ON)
option(CPPTRACE_TH_ENABLE_SKIP "Enable skip regex" ON)
set(CPPTRACE_TH_SKIP_REGEX
    [=[
\bstd::terminate\b]=]
    CACHE STRING "Frames before matched regex (included) will be omitted")
option(CPPTRACE_TH_NO_ZSTD "Remove ZSTD support (speedup build)" ON)

# ---------- libdwarf and cpptrace ----------
include(FetchContent)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW) # allow setting options with `set`

if(CPPTRACE_TH_NO_ZSTD)
  set(ENABLE_DECOMPRESSION FALSE)
  set(BUILD_DWARFDUMP FALSE)
  FetchContent_Declare(
    libdwarf
    GIT_REPOSITORY https://github.com/jeremy-rifkin/libdwarf-lite.git
    GIT_TAG v0.11.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    OVERRIDE_FIND_PACKAGE TRUE SYSTEM)
  FetchContent_MakeAvailable(libdwarf)

  set(CPPTRACE_USE_EXTERNAL_LIBDWARF TRUE)
endif()
FetchContent_Declare(
  cpptrace
  GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
  GIT_TAG v0.7.1
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
  SYSTEM)
FetchContent_MakeAvailable(cpptrace)

# ---------- build ----------
add_library(cpptrace_th_options INTERFACE)
target_compile_definitions(
  cpptrace_th_options
  INTERFACE CPPTRACE_TH_PRINT_SNIPPETS=$<BOOL:${CPPTRACE_TH_PRINT_SNIPPETS}>
            CPPTRACE_TH_ENABLE_SKIP=$<BOOL:${CPPTRACE_TH_ENABLE_SKIP}>
            CPPTRACE_TH_SKIP_REGEX=${CPPTRACE_TH_SKIP_REGEX})
target_link_libraries(cpptrace_th_options INTERFACE cpptrace::cpptrace)

add_library(cpptrace_th_static STATIC register.cpp)
target_link_libraries(cpptrace_th_static PRIVATE cpptrace_th_options)
add_library(cpptrace_th_static_whole_archive INTERFACE)
target_link_libraries(
  cpptrace_th_static_whole_archive
  INTERFACE -Wl,--whole-archive cpptrace_th_static -Wl,--no-whole-archive)

add_library(cpptrace_th_shared SHARED register.cpp)
target_link_libraries(cpptrace_th_shared PRIVATE cpptrace_th_options)
add_library(cpptrace_th_shared_no_as_needed INTERFACE)
target_link_libraries(
  cpptrace_th_shared_no_as_needed INTERFACE -Wl,--no-as-needed
                                            cpptrace_th_shared -Wl,--as-needed)

if(CPPTRACE_TH_BUILD_EXAMPLE)
  add_subdirectory(examples)
endif()

# ---------- export ----------
add_library(cpptrace_th::static ALIAS cpptrace_th_static_whole_archive)
add_library(cpptrace_th::shared ALIAS cpptrace_th_shared_no_as_needed)
