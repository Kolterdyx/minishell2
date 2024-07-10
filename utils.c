//
// Created by kolterdyx on 7/10/24.
//

#include "minishell.h"

char *token_type_str(const token_type type) {
    switch (type) {
        case ARG:
            return "ARG";
        case VARIABLE:
            return "VAR";
        case PIPE:
            return "PIPE";
        case SEMICOLON:
            return "SEMICOLON";
        case REDIRECTION_STDOUT:
            return "REDIRECTION_STDOUT";
        case REDIRECTION_STDIN:
            return "REDIRECTION_STDIN";
    }
    return NULL;
}

int ft_isalpha(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int ft_isdigit(int c) {
    return c >= '0' && c <= '9';
}

int ft_isalnum(int c) {
    return ft_isdigit(c) || ft_isalpha(c);
}


char *ft_strjoin(char *s1, char *s2) {
    char *new_str;
    int i;
    int j;

    if (!s1)
        s1 = "";

    if (!s2)
        s2 = "";

    new_str = malloc(strlen(s1) + strlen(s2) + 1);
    i = 0;
    j = 0;
    while (s1[i]) {
        new_str[j] = s1[i];
        i++;
        j++;
    }
    i = 0;
    while (s2[i]) {
        new_str[j] = s2[i];
        i++;
        j++;
    }
    new_str[j] = '\0';
    return new_str;
}
