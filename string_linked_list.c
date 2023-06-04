/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Implementation of String Linked List
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./string_linked_list.h"

/**
 * Create a string node list.
 * @return
 */
sll* create_string_linked_list() {
    sll* linked_list = (sll *)malloc(sizeof(sll));
    if (linked_list == NULL) exit (1);
    linked_list->start = NULL;
    linked_list->end = NULL;
    linked_list->add = &list_add_element;
    linked_list->destructor = &list_destructor;

    return linked_list;
}

/**
 * Add an element in the end of the list
 * @param linked_list
 * @param string
 */
void list_add_element(sll* linked_list, char* string) {
    string_node* new_node = (string_node*) malloc(sizeof(string_node) + strlen(string) + 1);
    if (new_node == NULL) {
        exit (1);
    }

    new_node->next = NULL;
    new_node->previous = NULL;
    new_node->list = linked_list;
    new_node->edit = &node_edit;
    new_node->delete = &node_delete;
    strcpy(new_node->string,string);

    if (NULL == linked_list->start) {
        // If there is no start node.
        linked_list->start = new_node;
        linked_list->start->next = NULL;
    } else {
        // Set the previous pointer of the new node to point to the end of the list.
        new_node->previous = linked_list->end;
        // Set the next pointer of the original end node of the list to point to the new node.
        linked_list->end->next = new_node;
    }

    // Set the end of the list to point to the new node.
    linked_list->end = new_node;
}

/**
 * Free the memory of the node list
 * @param linked_list
 */
void list_destructor(sll* linked_list) {
    string_node* node = linked_list->start;
    while (NULL != node) {
        string_node* temp_node = node;
        node = node->next;
        free(temp_node);
    }

    free(linked_list);
}

/**
 * Modify the string in a string node.
 * @param origin_node
 * @param new_str
 * @return
 */
string_node* node_edit(string_node* origin_node, char* new_str) {
    int origin_len = (int) strlen(origin_node->string);
    int new_len = (int) strlen(new_str);
    string_node* new_node;
    // If the length has changed, reassigning memory.
    if (new_len != origin_len) {
        new_node = (string_node*) malloc(sizeof(string_node) + new_len + 1);
        if (new_node == NULL) {
            exit(1);
        }

        // Copy the memory of the original node directly to a new memory.
        memcpy(new_node, origin_node, sizeof(string_node));
        // Set the new string in the new node.
        strcpy(new_node->string,new_str);

        if (NULL != origin_node->previous) {
            // Connect the next pointer of the previous node of the original node to the new node.
            origin_node->previous->next = new_node;
        }

        if (NULL != origin_node->next) {
            // Connect the previous pointer of the original node's next node to the new node.
            origin_node->next->previous = new_node;
        }

        if (origin_node->list->start == origin_node) {
            // If the original node is the start of the list, replace it with the new node.
            origin_node->list->start = new_node;
        }

        if (origin_node->list->end == origin_node) {
            // If the original node is the end of the list, replace it with the new node.
            origin_node->list->end = new_node;
        }

        free(origin_node);
        origin_node = new_node;
    }

    strcpy(origin_node->string, new_str);
    return origin_node;
}

/**
 * Delete this node.
 * @param current_node
 */
void node_delete(string_node* current_node) {
    if (NULL == current_node->previous && NULL == current_node->next) {
        // If the current node does not have a previous or next node, reset the start and end of the linked list.
        current_node->list->start = NULL;
        current_node->list->end = NULL;
    } else if (NULL == current_node->next) {
        // If the current node does not have a next node (current is the last of the list),
        // its previous node's next pointer will become NULL.
        current_node->previous->next = NULL;
        // Update the end of the linked list to the previous node.
        current_node->list->end = current_node->previous;
    } else if (NULL == current_node->previous) {
        // If the current node does not have a previous node (current at start of the list),
        // its next node's next previous will become NULL.
        current_node->next->previous = NULL;
        // Update the start of the linked list to the next node.
        current_node->list->start = current_node->next;
    } else {
        //
        // Set the previous pointer of the next node of the current node to point to the current node's next node.
        current_node->previous->next = current_node->next;
        // Set the next pointer of the previous node of the current node to point to the current node's previous node.
        current_node->next->previous = current_node->previous;
        // The current_node has been detached from the linked list
    }

    // Free the current_node memory
    free(current_node);
}
