

#include "minishell.h"

int main(int argc, char **argv, char **envp) {
    t_list *env_list = parse_env(envp);
    const char *input = "echo hello world \"USER   $USER fdkjhbv $DISPLAY vsdf fg $SHELL\"dbsff\"USER   $USER fdkjhbv $DISPLAY vsdf fg $SHELL\"";
    t_list *tokens = tokenize(input, env_list);
    for (const t_list *token_node = tokens; token_node; token_node = token_node->next) {
        const t_token *token = token_node->data;
        print_token(token);
    }

    destroy_list(tokens, destroy_token);
    destroy_list(env_list, destroy_env);
}