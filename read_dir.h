/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Head of folder reader
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#ifndef READ_DIR_H
#define READ_DIR_H

sll* list_all_type_files(const char* folderPath, char* file_type);

#endif
