cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17) 
project( luavm )

include_directories( include )

aux_source_directory( ./src LUAVM_SRC)

add_executable( luavm ${LUAVM_SRC} )