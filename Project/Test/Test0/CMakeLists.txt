set(ModuleName Test0)

file(GLOB_RECURSE SrcAll *.cpp *.h)

add_executable(${ModuleName} ${SrcAll})

target_link_libraries(${ModuleName} PRIVATE Niflect)

include(../Licensed_googletest.cmake)
add_test(NAME ${ModuleName} COMMAND ${ModuleName})