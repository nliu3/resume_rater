/***************************************************
 * Written by...: Ning Liu
 * Date Written.: May 27, 2023
 * Purpose......: Folder reader
 * Assignments..: Resume Rater
 * Compilation..: gcc -o out main.c string_linked_list.c text_file_parser.c tire_tree.c util.c read_dir.c
 * How to run...: `./out.exe` or `./out`.
 ***************************************************/

#include <stdio.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define PATH_SEPARATOR "\\"
#else
#include <dirent.h>
#define PATH_SEPARATOR "/"
#define MAX_PATH 6255
#endif

#if defined(_WIN32) || defined(_WIN64)
#define MY_SPRINT_F sprintf_s
#else
#define MY_SPRINT_F snprintf
#endif

#include "./string_linked_list.h"

/**
 * Read all paths of a given type from the specified directory and return them as a string linked list.
 * @param folderPath
 * @param file_type
 * @return
 */
sll* list_all_type_files(const char* folder_path, char* file_type) {
    sll *linked_list = create_string_linked_list();
#ifdef _WIN32
    WIN32_FIND_DATA findData;
    HANDLE h_find;
    char searchPath[MAX_PATH];
    char ext_pattern[10];
    MY_SPRINT_F(ext_pattern, 10, "\\*.%s", file_type);
    strcpy(searchPath, folder_path);
    strcat(searchPath, ext_pattern);

    h_find = FindFirstFile(searchPath, &findData);
    if (h_find != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                char tmp_path[MAX_PATH];
                MY_SPRINT_F(tmp_path, MAX_PATH, "%s%s%s", folder_path, PATH_SEPARATOR, findData.cFileName);
                linked_list->add(linked_list, tmp_path);
            }
        } while (FindNextFile(h_find, &findData));

        FindClose(h_find);
    }
#else
    DIR* dir;
    struct dirent* entry;
    dir = opendir(folder_path);

    char ext_pattern[10];
    MY_SPRINT_F(ext_pattern, 10, ".%s", file_type);

    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ext_pattern) != NULL) {
                char tmp_path[MAX_PATH];
                MY_SPRINT_F(tmp_path, MAX_PATH, "%s%s%s", folder_path, PATH_SEPARATOR, entry->d_name);
                linked_list->add(linked_list, tmp_path);
            }
        }
        closedir(dir);
    }
#endif

    return linked_list;
}
