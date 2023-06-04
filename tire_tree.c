/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Implementation of Trie Search Tree
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#include <malloc.h>
#include <string.h>
#include "./tire_tree.h"
#include "util.h"

/**
 * Create a Trie node.
 * @return
 */
trie_node* create_trie_node() {
    trie_node * new_node = (trie_node*) malloc(sizeof(trie_node));
    new_node->is_end = 0;
    for (int i = 0; i < TIRE_MAX_CHILD_NODE; i++){
        new_node->next[i] = NULL;
    }

    return new_node;
}

/**
 * Get the index corresponding to a character
 * @param ch
 * @return
 */
int get_index(const char ch) {
    if (ch == 32) {
        return TIRE_SPACE_INDEX;
    } else {
        // By subtracting characters and using an index
        // to ensure that the same character is not inserted repeatedly at the same level.
        return ch - 'a';
    }
}

/**
 * Insert a word into this Trie node
 * @param trie
 * @param origin_str
 */
void insert_word_to_trie(trie_node* trie, const char *origin_str) {
    // lower case
    char str[strlen(origin_str) + 1];
    strcpy(str, origin_str);
    my_strlwr(str);

    // Check the insert word(s)
    int i = 0, j = 0, index;
    while (str[j] != '\0') {
        index = get_index(str[i]);
        if (index < 0 || index > TIRE_MAX_CHILD_NODE - 1) {
            return;
        }
        j++;
    }

    trie_node* node = trie;
    while (str[i]) {
        index = get_index(str[i]);
        if (!node->next[index]) {
            node->next[index] = create_trie_node();
        }

        node = node->next[index];
        i++;
    }

    node->is_end = 1;
}

enum SEARCH_RESULT search_word_in_trie(trie_node* trie, const char *origin_str) {
    // lower case
    char str[strlen(origin_str) + 1];
    strcpy(str, origin_str);
    my_strlwr(str);
    // check search words
    int i = 0, j = 0, index;
    while (str[j] != '\0') {
        index = get_index(str[j]);
        if (index < 0 || index > TIRE_MAX_CHILD_NODE - 1) {
            return 0;
        }
        j++;
    }

    trie_node* node = trie;
    while (str[i]) {
        // Compare each character in the string str until reaching the end of the character array
        // or encountering a node that does not exist in the Trie.
        index = get_index(str[i]);
        if (!node->next[index]) {
            break;
        }

        node = node->next[index];
        i++;
    }

    if (node->next[TIRE_SPACE_INDEX] && !str[i]) {
        return WORDS_PARTIAL;
    } else if (node->is_end && !str[i]) {
        return WORD_FOUND;
    } else {
        return NOT_FOUND;
    }
}

/**
 * Free the trie memory
 * @param trie
 */
void free_trie(trie_node *trie) {
    for (int i = 0; i < TIRE_MAX_CHILD_NODE; i++){
        if (NULL != trie->next[i]) {
            free_trie(trie->next[i]);
        }
    }

    free(trie);
}
