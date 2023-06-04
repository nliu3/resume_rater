/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Convert a text file into a string linked list.
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include "./string_linked_list.h"
#include "./text_file_parser.h"
#define BUF_SIZE 100
#define HYPHEN "-"

/**
 * Parse the file given its .txt address and return the phrases as data in the form of a string linked list structure.
 * @param file_path
 * @param delimiter_char
 * @return
 */
sll* text_file_to_linked_list(char* file_path, char delimiter_char) {
    char delimiter_str[2] = {delimiter_char, '\0'};
    sll *linked_list = create_string_linked_list();
    FILE *f;
    if ((f = fopen(file_path, "rb")) == NULL) {
        printf("Open failed: %s!!\n", file_path);
        exit(1);
    }

    // Buffer size for each read operation.
    // +1 is for '\0';
    char buffer[BUF_SIZE + 1];
    size_t block_size = sizeof(char);
    size_t elements_read;
    memset(buffer, 0, BUF_SIZE + 1);
    while ((elements_read = fread(buffer, block_size, BUF_SIZE / block_size, f)) > 0) {
        // Since it is reading in binary mode, you need to manually add the '\0' character.
        buffer[elements_read * block_size + 1] = '\0';
        // Replace any non-alphanumeric characters, excluding the delimiter character, with a space.
        replace_non_alphanumeric(buffer, delimiter_char);

        int str_len = (int) strlen(buffer);
        // 3 = left hyphen, a right hyphen, and  '\0'
        int new_str_len = str_len + 3;
        char new_str[new_str_len];

        // Handle the case where a word at the beginning or end of the buffer is cut off.
        build_hyphen_between_buffers(buffer, new_str, new_str_len, delimiter_char);

        int in = 0, j;
        char *p[BUF_SIZE];
        char *buf = new_str;
        char *ptr = NULL;
        // Split the buffer into an array using the delimiter char.
        while((p[in] = strtok_r(buf, delimiter_str, &ptr)) != NULL) {
            buf = NULL;
            in++;
        }

        for(j = 0; j < in; j++) {
            // Add the results to the linked list in a loop.
            linked_list->add(linked_list, p[j]);
        }

        memset(buffer, 0, BUF_SIZE + 1);
    }

    fclose(f);

    // Link the broken words of the previous and next nodes using a hyphen.
    handle_hyphen(linked_list);

    return linked_list;
}

/**
   Since the buffer may capture the middle of a word, resulting in the word being cut off,
   if the beginning or end of the buffer contains a potential incomplete word, add a hyphen to the beginning or end.
 * @param origin_str
 * @param dest_str
 * @param len
 * @param delimiter_char
 */
void build_hyphen_between_buffers(char* origin_str, char* dest_str, int len, char delimiter_char) {
    memset(dest_str, 0, len);
    char last_char = origin_str[strlen(origin_str) - 1];
    char first_char = origin_str[0];

    // check head
    if (first_char != delimiter_char && first_char != 10) {
        strcat(dest_str, HYPHEN);
    }

    strcat(dest_str, origin_str);
    // check end
    if (last_char != delimiter_char && last_char != 10) {
        strcat(dest_str, HYPHEN);
    }
}

/**
 * Link the broken words of the previous and next nodes using a hyphen.
 * Example:
 * [Node 1] -> [Node 2]
 * Node 1 string: "diff-"
 * Node 2 string: "-icult"
 * Result : Node 1 will be deleted.
 *          Node 2 string will be edited to "difficult".
 * @param node
 * @return
 */
string_node* connect_next_by_hyphen(string_node* node) {
    char flag = 1;
    while (flag) {
        if (NULL != node->next) {
            string_node* n_node = node->next;
            char* c_node_string = node->string;
            int c_node_string_len = (int) strlen(c_node_string);
            char c_last_char = c_node_string[c_node_string_len - 1];
            char* n_node_string = n_node->string;
            int n_node_string_len =  (int) strlen(n_node_string);
            char n_first_char = n_node_string[0];
            // If the next node starts with a hyphen and the current node ends with a hyphen, concatenate the two strings.
            if (n_first_char == HYPHEN[0] && c_last_char == HYPHEN[0]) {
                char new_str[n_node_string_len + c_node_string_len];
                memset(new_str, 0, n_node_string_len + c_node_string_len);
                strncat(new_str, c_node_string, c_node_string_len - 1);
                strncat(new_str, n_node_string + 1, n_node_string_len - 1);
                // Change the next node string to new connected string.
                n_node = node->next->edit(n_node, new_str);
                // Delete the current node
                node->delete(node);
                // Set current node variable to next node.
                node = n_node;
            } else {
                flag = 0;
            }
        } else {
            flag = 0;
        }
    }

    return node;
}

/**
 * Filter out all leading hyphens from the strings in all nodes.
 * @param node
 * @return
 */
string_node* filter_start_hyphen(string_node* node) {
    char* c_node_string = node->string;
    int c_node_string_len = (int) strlen(c_node_string);
    char c_first_char = c_node_string[0];
    if (c_first_char == HYPHEN[0]) {
        char new_str[c_node_string_len];
        memset(new_str, 0, c_node_string_len);
        strncpy(new_str, c_node_string + 1, c_node_string_len - 1);
        node = node->edit(node, new_str);
    }

    return node;
}

/**
 * Filter out all trailing hyphens from the strings in all nodes.
 * @param node
 * @return
 */
string_node* filter_end_hyphen(string_node* node) {
    char* c_node_string = node->string;
    int c_node_string_len = (int) strlen(c_node_string);
    char c_last_char = c_node_string[c_node_string_len - 1];
    if (c_last_char == HYPHEN[0]) {
        char new_str[c_node_string_len];
        memset(new_str, 0, c_node_string_len);
        strncat(new_str, c_node_string, c_node_string_len - 1);
        node = node->edit(node, new_str);
    }

    return node;
}

/**
 * Handle hyphens within words in all nodes.
 * @param linked_list
 */
void handle_hyphen(sll* linked_list) {
    string_node *node = linked_list->start;
    while (NULL != node) {
        node = connect_next_by_hyphen(node);
        // At this point, all nodes that need to be connected with hyphens have been rearranged.
        // Next, remove the extra hyphens that not need to be linked in all nodes.
        node = filter_start_hyphen(node);
        node = filter_end_hyphen(node);
        node = node->next;
    }
}

/**
 * Replace any non-alphanumeric characters,
 * excluding the delimiter character, with a space.
 * @param str
 * @param delimiter_char
 */
void replace_non_alphanumeric(char *str, char delimiter_char) {
    while (*str) {
        if (!isalnum(*str) && *str != delimiter_char) {
            *str = ' ';
        }
        str++;
    }
}
