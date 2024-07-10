//
// Created by kolterdyx on 7/10/24.
//
#include "minishell.h"

t_token *parse_env_var(const char *input, t_list *env_list, size_t *i) {
    size_t j = *i + 1;
    while (input[j] != ' ' && input[j] != '$' && input[j] != '"' && input[j] != '\'' && input[j] != '\0') {
        j++;
    }
    char *value = malloc(j - *i);
    strncpy(value, input + *i + 1, j - *i - 1);
    *i = j;
    return new_token(ARG, get_env_value(env_list, value));
}

t_token *parse_string_var(const char *input, t_list *env_list, size_t *i) {
    size_t j = *i + 1;

    while (input[j] != '"' && input[j] != '\0') {
        j++;
    }
    char *value = calloc(j - *i, sizeof(char));
    strncpy(value, input + *i + 1, j - *i - 1);
    *i = j;
    value = replace_str(value, env_list);
    return new_token(ARG, value);
}

t_token *parse_arg(const char *input, size_t *i) {
    size_t k = *i;
    while (input[k] != ' ' && input[k] != '\0' && input[k] != '$' && input[k] != '"') {
        k++;
    }
    char *value = calloc(k - *i, sizeof(char));
    strncpy(value, input + *i, k - *i);
    *i = k;
    return new_token(ARG, value);
}

t_list *tokenize(const char *input, t_list *env_list) {
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
                list = append_list(list, parse_env_var(input, env_list, &i));
                continue;
            case '"':
                list = append_list(list, parse_string_var(input, env_list, &i));
                i++;
                continue;
            default:
                list = append_list(list, parse_arg(input, &i));
        }
    }
    return list;
}

t_list *parse(t_list *tokens) {
    t_list *cmd_list = NULL;
    for (t_list *node = tokens; node; node = node->next) {
        t_list *cmd_tokens = NULL;
        t_token *token = node->data;
        if (token->type == SEMICOLON || token->type == PIPE) {
            cmd_list = append_list(cmd_list, cmd_tokens);
        } else {
            cmd_tokens = append_list(cmd_tokens, copy_token(token));
        }
    }
    return cmd_list;
}
