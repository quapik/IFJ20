cmake_minimum_required(VERSION 3.10.2)

project(projIFJ20)

add_executable(${PROJECT_NAME} main.c scanner.c parser.c exprBottomUp.c symtable.c codegen.c)
