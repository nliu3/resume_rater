/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Header of Trie Search Tree
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#ifndef TIRE_TREE_H
#define TIRE_TREE_H

#define TIRE_MAX_CHILD_NODE 27 // a-z + space
#define TIRE_SPACE_INDEX 26 // Index corresponding to the space character

/**
 * Trie node instance.
 */
typedef struct trie_node {
    char is_end;
    struct trie_node *next[TIRE_MAX_CHILD_NODE];
} trie_node;

enum SEARCH_RESULT { NOT_FOUND, WORDS_PARTIAL, WORD_FOUND };

trie_node* create_trie_node();

int get_index(char ch);

void insert_word_to_trie(trie_node *trie, const char *str);

enum SEARCH_RESULT search_word_in_trie(trie_node* trie, const char *origin_str);

void free_trie(trie_node *trie);

#endif
