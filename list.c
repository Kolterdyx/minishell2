//
// Created by kolterdyx on 7/9/24.
//

#include "list.h"

#include <stdio.h>

t_list *new_list(const void *data) {
    t_list *list = calloc(1, sizeof(t_list));
    list->data = (void *)data;
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

t_list *append_list(const t_list *list, const void *data) {
    if (list == NULL) {
        return new_list(data);
    }
    t_list *head = (t_list *)list;
    t_list *node = (t_list *)list;
    t_list *new = new_list(data);
    while (node->next) {
        node = node->next;
    }
    node->next = new;
    head->size++;
    return head;
}
