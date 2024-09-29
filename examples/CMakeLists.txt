# Add an example to the build
function(add_example name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} PRIVATE cpptrace_th::object)
endfunction()

add_example(call_terminate)
add_example(throw_exception)
add_example(throw_int)
