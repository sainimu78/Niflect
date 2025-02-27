set(ModuleName Test1)

file(GLOB_RECURSE SrcAll *.cpp *.h)

add_executable(${ModuleName} ${SrcAll})

target_link_libraries(${ModuleName} PRIVATE Niflect)
target_link_libraries(${ModuleName} PRIVATE NiflectGen)

include(../Licensed_googletest.cmake)
add_test(NAME ${ModuleName} COMMAND ${ModuleName})