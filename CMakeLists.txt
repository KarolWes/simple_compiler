cmake_minimum_required(VERSION 3.24)
project(simple_compiler)

set(CMAKE_CXX_STANDARD 17)


FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

BISON_TARGET(
        Parser # Just a name
        parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp # path to save output
)

FLEX_TARGET(
        Lexer
        count.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


ADD_EXECUTABLE(main ${BISON_Parser_OUTPUTS} ${FLEX_Lexer_OUTPUTS}
        main.cpp lexer.cpp)
