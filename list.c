//
// Created by kolterdyx on 7/9/24.
//

#include "list.h"

#include <stdio.h>

t_list *new_list(void *data) {
    t_list *list = calloc(1, sizeof(t_list));
    list->data = data;
    list->next = NULL;
    list->size = 1;
    return list;
}

void destroy_list(t_list *list, void (*destroy_data)(void *)) {
    if (list == NULL) {
        fprintf(stderr, "destroy_list: list is NULL\n");
        return;
    }
    if (list->next) {
        destroy_list(list->next, destroy_data);
    }
    destroy_data(list->data);
    free(list);
}

t_list *append_list(t_list *list, void *data) {
    if (list == NULL) {
        printf("create %p %p\n", list, data);
        return new_list(data);
    }
    printf("append %p %p\n", list, data);
    t_list *new = new_list(data);
    while (list->next) {
        list = list->next;
    }
    list->next = new;
    list->size++;
    return list;
}