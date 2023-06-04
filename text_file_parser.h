/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Header of convert a text file into a string linked list.
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#ifndef TEXT_FILE_PARSER_H
#define TEXT_FILE_PARSER_H

sll* text_file_to_linked_list(char* file_path, char delimiter_char);

void build_hyphen_between_buffers(char* origin_str, char* dest_str, int len, char delimiter_char);

string_node* connect_next_by_hyphen(string_node* node);

string_node* filter_end_hyphen(string_node* node);

string_node* filter_start_hyphen(string_node* node);

void handle_hyphen(sll* linked_list);

void replace_non_alphanumeric(char *str, char delimiter_char);

#endif
