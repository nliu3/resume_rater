/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Header of String Linked List
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#ifndef STRING_LINKED_LIST_H
#define STRING_LINKED_LIST_H

/**
 * String Linked list struct
 */
typedef struct string_linked_list {
    struct string_node* start;
    struct string_node* end;
    void (*add)(struct string_linked_list* linked_list, char* string);
    void (*destructor)(struct string_linked_list* linked_list);
} sll;

/**
 * String Node instance
 */
typedef struct string_node {
    struct string_node* previous;
    struct string_node* next;
    struct string_linked_list* list;
    struct string_node*  (*edit)(struct string_node*, char* new_str);
    void (*delete)(struct string_node*);
    char string[];
} string_node;

sll* create_string_linked_list();

void list_add_element(sll* linked_list, char* string);

void list_destructor(sll* me);

string_node* node_edit(string_node* origin_node, char* new_str);

void node_delete(string_node* current_node);

#endif
