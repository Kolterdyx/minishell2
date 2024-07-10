//
// Created by kolterdyx on 7/10/24.
//

#ifndef MINISHELL_H
#define MINISHELL_H

#include "list.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

typedef struct s_env {
    char *key;
    char *value;
} t_env;

t_token *new_token(token_type type, char *value);
t_token *copy_token(const t_token *token);
void destroy_token(t_token *token);
void print_token(const t_token *token);
char *token_type_str(token_type type);

int ft_isalpha(int c);
int ft_isdigit(int c);
int ft_isalnum(int c);
char *ft_strjoin(char *s1, char *s2);

t_list *parse_env(char **envp);
char *get_env_value(const t_list *env_list, const char *key);
char *find_varname(char *str);
char *replace_str(char *str, t_list *env_list);
void print_env(const t_list *env_list);
void destroy_env(t_env *var);

t_token *parse_env_var(const char *input, t_list *env_list, size_t *i);
t_token *parse_string_var(const char *input, t_list *env_list, size_t *i);
t_token *parse_arg(const char *input, size_t *i);
t_list *tokenize(const char *input, t_list *env_list);
t_list *parse(t_list *tokens);

#endif //MINISHELL_H
