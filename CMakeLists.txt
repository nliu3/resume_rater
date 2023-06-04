cmake_minimum_required(VERSION 3.25)
project(Ctest C)

set(CMAKE_C_STANDARD 11)

add_executable(Ctest main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c)
