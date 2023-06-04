/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Main file
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <libgen.h>
#include "./string_linked_list.h"
#include "./text_file_parser.h"
#include "./tire_tree.h"
#include "./util.h"
#include "./read_dir.h"

#if defined(_WIN32) || defined(_WIN64)
#define KEYWORDS_FILE_PATH ".\\keywords.txt" // keywords file path
#define RESUME_DIR_PATH ".\\resumes" // resume file dir
#else
#define KEYWORDS_FILE_PATH "./keywords.txt"
#define RESUME_DIR_PATH "./resumes"
#endif

// Prototypes start.
trie_node* build_search_trie();

enum SEARCH_RESULT find_full_words(trie_node* trie, string_node* node);

int get_resume_rate(trie_node *trie, char* file_path);
// Prototypes end.

int main(void) {
    // Building a trie search tree.
    trie_node *search_trie = build_search_trie();

    //Read all files of type "txt" in a directory and return a linked list of file names.
    sll* file_path_linked_list = list_all_type_files(RESUME_DIR_PATH, "txt");

    string_node* resume_path_node = file_path_linked_list->start;
    puts("*** RESUME RATING SYSTEM ***\n");
    while (NULL != resume_path_node) {
        // Get the resume rate;
        int rate = get_resume_rate(search_trie, resume_path_node->string);
        printf("File name: %-30s     Resume Rating: %d\n", basename(resume_path_node->string), rate);
        resume_path_node = resume_path_node->next;
    }
    puts("\n----------- Thank for using! -----------");

    // Destruct the file path linked list.
    file_path_linked_list->destructor(file_path_linked_list);
    // Free the trie search tree.
    free_trie(search_trie);

    return 0;
} // end main

/**
 * Build a trie search tree by reading the text content from the KEYWORDS_FILE_PATH.
 * @return trie_node
 */
trie_node* build_search_trie() {
    trie_node *trie = create_trie_node();
    // Build a linked list of strings using the text from keywords.txt.
    sll* keywords_linked_list = text_file_to_linked_list(KEYWORDS_FILE_PATH, ',');
    string_node* keywords_string_node = keywords_linked_list->start;
    while (NULL != keywords_string_node) {
        char tmp_str[strlen(keywords_string_node->string) + 1];
        strcpy(tmp_str, keywords_string_node->string);
        // Convert the keyword to lowercase.
        my_strlwr(tmp_str);
        // Insert into the trie search tree.
        insert_word_to_trie(trie, trim(tmp_str));
        keywords_string_node = keywords_string_node->next;
    }

    //  Destruct the file keywords linked list.
    keywords_linked_list->destructor(keywords_linked_list);

    return trie;
}

/**
 * Searching for words that contain spaces.
 * Suppose we have a search tree that contains the phrase "Cross browser Compatibility" as a continuous string.
 * However, in the resume string node, the phrase is separated into individual words,
 * like "Cross", "browser", and "Compatibility".
 * In this case, we would need to iterate and check the resume string node and its next nodes for a complete match.
 * @param trie trie search tree
 * @param node resume string node
 * @return
 */
enum SEARCH_RESULT find_full_words(trie_node* trie, string_node* node) {
    char* c_str = node->string;
    int total_str_len = (int) strlen(c_str);
    // Due to the increasing length of the strings, dynamic memory allocation is required here.
    char* str_mem = (char*) malloc(total_str_len + 1);
    strcpy(str_mem, c_str);
    string_node* n_node = node->next;
    char flag = 0;
    enum SEARCH_RESULT ret = NOT_FOUND;
    int space_count = 1;
    while (!flag && NULL != n_node) {
        char* n_str = n_node->string;
        total_str_len = total_str_len + (int) strlen(n_str) + space_count;
        char* tmp_str_mem = (char *) realloc(str_mem, total_str_len + 1);
        if (NULL == tmp_str_mem) {
            exit(1);
        }

        // Try to combine with next word and search again.
        str_mem = tmp_str_mem;
        strcat(str_mem, " ");
        strcat(str_mem, n_str);
        switch (search_word_in_trie(trie, str_mem)) {
            case WORD_FOUND:
                ret = WORD_FOUND;
                flag = 1;
                break;
            case NOT_FOUND:
                ret = NOT_FOUND;
                flag = 1;
                break;
            case WORDS_PARTIAL:
                // If you still return WORDS_PARTIAL, the while loop will continue.
                break;
        }

        n_node = n_node->next;
        space_count++;
    }

    free(str_mem);
    return ret;
}

/**
 * Get the rate of a resume through trie search tree.
 * @param trie trie search tree
 * @param file_path
 * @return
 */
int get_resume_rate(trie_node *trie, char* file_path) {
    int rate = 0;
    sll* resume_words_linked_list = text_file_to_linked_list(file_path, ' ');
    string_node* resume_string_node = resume_words_linked_list->start;
    while (NULL != resume_string_node) {
        switch (search_word_in_trie(trie, resume_string_node->string)) {
            case WORD_FOUND:
                rate++;
                break;
            case WORDS_PARTIAL:
                if (find_full_words(trie, resume_string_node)) {
                    rate++;
                }
                break;
            case NOT_FOUND:
                break;
        }

        resume_string_node = resume_string_node->next;
    }

    resume_words_linked_list->destructor(resume_words_linked_list);
    return rate;
}
