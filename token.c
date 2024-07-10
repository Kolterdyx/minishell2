//
// Created by kolterdyx on 7/10/24.
//

#include "minishell.h"

t_token *new_token(const token_type type, char *value) {
    t_token *token = malloc(sizeof(t_token));
    token->type = type;
    token->value = value;
    return token;
}


t_token *copy_token(const t_token *token) {
    return new_token(token->type, token->value ? strdup(token->value) : NULL);
}


void destroy_token(t_token *token) {
    free(token->value);
    free(token);
}

void print_token(const t_token *token) {
    if (token->value) {
        printf("%s '%s'\n", token_type_str(token->type), token->value);
    } else {
        printf("%s\n", token_type_str(token->type));
    }
}
