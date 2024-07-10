#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ucontext.h>

#include "list.h"

typedef enum e_token_type {
    ARG,
    VARIABLE,
    PIPE,
    SEMICOLON,
    REDIRECTION_STDOUT,
    REDIRECTION_STDIN,
} token_type;

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

typedef struct s_token {
    token_type type;
    char *value;
} t_token;


typedef struct s_env {
    char *key;
    char *value;
} t_env;


typedef struct s_command {
    t_list *tokens;
} t_command;

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

char *get_env_value(const t_list *env_list, const char *key) {
    printf("key: %s\n", key);
    for (const t_list *node = env_list; node; node = node->next) {
        t_env *var = node->data;
        if (strcmp(var->key, key) == 0) {
            return var->value;
        }
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

char *find_varname(char *str) {
    int start;
    int started_var;

    start = 0;
    started_var = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '$' && (ft_isalpha(str[i+1]) || str[i+1] == '_')) {
            start = i+1;
            started_var = 1;
        }
        if (i > start && started_var && !(ft_isalnum(str[i+1]) || str[i+1] == '_'))
            return strndup(str + start - 1, (i - start) + 2);
    }
    return NULL;
}


char *replace_str(char *str, t_list *env_list) {
    char *var;
    char *start;
    char *end;
    char *tmp2;
    char *tmp;

    var = find_varname(str);
    while (var) {
        start = strstr(str, var);
        if (start)
            end = start + strlen(var);
        tmp = strndup(str, start - str);
        tmp2 = ft_strjoin(tmp, get_env_value(env_list, var+1));
        free(tmp);
        tmp = ft_strjoin(tmp2, end);
        free(tmp2);
        free(str);
        str = tmp;
        printf("%s\n", str);
        var = find_varname(str);
    }
    return str;
}

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
    char *value = malloc(j - *i);
    strncpy(value, input + *i + 1, j - *i - 1);
    *i = j;
    value = replace_str(value, env_list);
    return new_token(ARG, value);
}

t_token *parse_arg(const char *input, size_t *i) {
    size_t k = *i;
    while (input[k] != ' ' && input[k] != '\0') {
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

t_list *parse_env(char **envp) {
    int i = 0;
    t_list *env_list = NULL;
    t_env *var = NULL;
    while (envp[i]) {
        var = calloc(1, sizeof(t_env));
        var->key = strndup(envp[i], strchr(envp[i], '=') - envp[i]);
        var->value = strdup(strchr(envp[i], '=') + 1);
        env_list = append_list(env_list, var);
        i++;
    }
    return env_list;
}



int main(int argc, char **argv, char **envp) {
    t_list *env_list = parse_env(envp);
    const char *input = "echo hello world \"USER   $USER fdkjhbv $DISPLAY vsdf fg $SHELL\"";
    t_list *tokens = tokenize(input, env_list);
    for (const t_list *token_node = tokens; token_node; token_node = token_node->next) {
        const t_token *token = token_node->data;
        print_token(token);
    }

    destroy_list(tokens, destroy_token);
}