#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>

#include <cpptrace/cpptrace.hpp>

namespace {

namespace config {

auto option_is_on(std::string option) -> bool {
  std::transform(option.begin(), option.end(), option.begin(),
                 [](unsigned char ch) { return std::tolower(ch); });
  return !(option == "0" || option == "false" || option == "off" ||
           option == "no" || option == "n");
}

auto get_env_var(const char* env_var,
                 const char* default_value) -> std::string {
  const char* value = std::getenv(env_var);
  return value != nullptr ? value : default_value;
}

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define CPPTRACE_TH_EXPAND(x) x
#define CPPTRACE_TH_GET_CONFIG(name) \
  get_env_var("CPPTRACE_TH_" #name, CPPTRACE_TH_EXPAND(CPPTRACE_TH_##name))
// NOLINTEND(cppcoreguidelines-macro-usage)

auto print_snippets() -> bool {
  static const auto kPrintSnippets = CPPTRACE_TH_GET_CONFIG(PRINT_SNIPPETS);
  return option_is_on(kPrintSnippets);
}

auto enable_skip_regex() -> bool {
  static const auto kEnableSkipRegex = CPPTRACE_TH_GET_CONFIG(ENABLE_SKIP);
  return option_is_on(kEnableSkipRegex);
}

auto skip_regex() -> std::string {
  static const auto kSkipRegex = CPPTRACE_TH_GET_CONFIG(SKIP_REGEX);
  return kSkipRegex;
}

}  // namespace config

auto skip_stacktrace_frames(cpptrace::stacktrace& trace) -> void {
  static const std::regex kSkipRegex(config::skip_regex());
  auto pos = std::find_if(trace.frames.begin(), trace.frames.end(),
                          [](const cpptrace::stacktrace_frame& frame) {
                            return std::regex_search(frame.symbol, kSkipRegex);
                          });
  if (pos != trace.frames.end()) {
    trace.frames.erase(trace.frames.begin(), pos + 1);
  }
}

auto terminate_handler() -> void {
  try {
    std::ostringstream oss;

    try {
      std::exception_ptr eptr = std::current_exception();
      if (eptr) {
        std::rethrow_exception(eptr);
      }
      oss << "terminate called without an active exception\n";
    } catch (const std::exception& e) {
      oss << "terminate called after throwing an instance of '"
          << cpptrace::demangle(typeid(e).name()) << "'\n";
      oss << "  what(): " << e.what() << '\n';
    } catch (...) {
      oss << "terminate called after throwing an instance of unknown type\n";
    }

    auto trace = cpptrace::generate_trace(/*skip=*/1);
    if (config::enable_skip_regex()) {
      skip_stacktrace_frames(trace);
    }
    if (config::print_snippets()) {
      trace.print_with_snippets(oss);
    } else {
      trace.print(oss);
    }

    std::cerr << oss.str() << std::flush;
  } catch (...) {
    std::fputs("exception raised in terminate handler\n", stderr);
  }
  std::abort();
}

const auto kSetTerminateInitializer = []() {
  std::set_terminate(terminate_handler);
  return 0;
}();

}  // namespace
