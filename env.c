//
// Created by kolterdyx on 7/10/24.
//
#include "minishell.h"

char *get_env_value(const t_list *env_list, const char *key) {
    for (const t_list *node = env_list; node; node = node->next) {
        t_env *var = node->data;
        if (strcmp(var->key, key) == 0) {
            return var->value;
        }
    }
    return NULL;
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
    char *varname;
    char *start;
    char *end;
    char *tmp2;
    char *tmp;

    varname = find_varname(str); // $VAR
    while (varname) {
        start = strstr(str, varname);  // find where the var starts: "hello $USER test" -> "$USER test"
        if (start)
            end = start + strlen(varname);  // find where the var ends: "$USER test" -> "USER test"
        else
            return NULL;  // Panic, we should never reach this point
        tmp = strndup(str, start - str);  // left side of the var: "hello $USER test" -> "hello "
        tmp2 = ft_strjoin(tmp, get_env_value(env_list, varname+1));  // "hello " + "user" -> "hello user"
        free(tmp);
        free(varname);
        tmp = ft_strjoin(tmp2, end); // "hello user" + " test" -> "hello user test"
        free(tmp2);
        free(str);
        str = tmp;  // "hello user test"
        varname = find_varname(str);  // find next var
    }
    return str;
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

void print_env(const t_list *env_list) {
    for (const t_list *node = env_list; node; node = node->next) {
        t_env *var = node->data;
        printf("%s=%s\n", var->key, var->value);
    }
}

void destroy_env(t_env *var) {
    free(var->key);
    free(var->value);
    free(var);
}