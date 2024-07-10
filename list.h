//
// Created by kolterdyx on 7/9/24.
//

#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

typedef struct s_list {
    void *data;
    struct s_list *next;
    int size;
} t_list;

t_list *new_list(const void *data);

void destroy_list(t_list *list, void (*destroy_data)(void *));

t_list *append_list(const t_list *list, const void *data);

#endif //LIST_H
