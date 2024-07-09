#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "list.h"

typedef enum e_token_type {
    ARG,
    VARIABLE,
    PIPE,
    SEMICOLON,
    REDIRECTION_STDOUT,
    REDIRECTION_STDIN,
} token_type;

typedef struct s_token {
    token_type type;
    char *value;
} t_token;


typedef struct s_command {
    t_token *tokens;
    struct s_command *next;
} t_command;

t_token *new_token(const token_type type, char *value) {
    t_token *token = malloc(sizeof(t_token));
    token->type = type;
    token->value = value;
    return token;
}

void destroy_token(t_token *token) {
    free(token->value);
    free(token);
}

t_token *env_var(const char *input, size_t *i) {
    size_t j = *i + 1;
    while (input[j] != ' ' && input[j] != '\0') {
        j++;
    }
    char *value = malloc(j - *i);
    strncpy(value, input + *i + 1, j - *i - 1);
    *i = j;
    return new_token(VARIABLE, value);
}

t_list *tokenize(const char *input) {
    size_t i = 0;
    const size_t len = strlen(input);
    t_list *list = NULL;
    while (i < len) {
        switch (input[i]) {
            case ' ':
                i++;
                continue;
            case '|':
                list = append_list(list, new_token(PIPE, NULL));
                i++;
                continue;
            case ';':
                list = append_list(list, new_token(SEMICOLON, NULL));
                i++;
                continue;
            case '>':
                list = append_list(list, new_token(REDIRECTION_STDOUT, NULL));
                i++;
                continue;
            case '<':
                list = append_list(list, new_token(REDIRECTION_STDIN, NULL));
                i++;
                continue;
            case '$':
                list = append_list(list, env_var(input, &i));
                continue;
            default:
                size_t k = i;
                while (input[k] != ' ' && input[k] != '\0') {
                    k++;
                }
                char *value = malloc(k - i);
                strncpy(value, input + i, k - i);
                list = append_list(list, new_token(ARG, value));
                i = k;
        }
    }
    return list;
}




int main() {
    const char *input = "ls -l | grep foo";
    t_list *tokens = tokenize(input);
    printf("size: %i\n", tokens->size);
    for (t_list *node = tokens; node != NULL; node = node->next) {
        t_token *token = node->data;
        printf("%s\n", token->value);
    }
    destroy_list(tokens, destroy_token);
}