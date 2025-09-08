/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:00:00 by rwrobles          #+#    #+#             */
/*   Updated: 2025/09/08 21:14:24 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

/* Forward declaration */
typedef struct s_shell t_shell;

typedef struct s_expander {
        char            *input;
        char            *result;
        size_t          input_pos;
        size_t          result_pos;
        size_t          result_capacity;
        t_quote_state   quote_state;
        t_shell         *shell;
        int             error;
} t_expander;

// Core expansion functions
char            *expand_string(const char *input, t_shell *shell,
                        t_quote_state state);
int             expand_token(t_token *token, t_shell *shell);
int             expand_command(t_cmd *cmd, t_shell *shell);
int             expand_command_list(t_cmd *cmd_list, t_shell *shell);

// Expander lifecycle
t_expander      *expander_init(const char *input, t_shell *shell,
                        t_quote_state state);
t_expander      *expander_alloc(const char *input, t_shell *shell,
                        t_quote_state state);
void            expander_destroy(t_expander *expander);
int             expander_expand(t_expander *expander);
void            expander_set_error(t_expander *expander);

// Variable resolution
char            *resolve_variable(const char *var_name, t_shell *shell);
char            *resolve_exit_status(t_shell *shell);
char            *get_env_var(const char *name, t_shell *shell);

// Quote handling
t_quote_state   update_quote_state(t_quote_state current, char c);
int             is_quote_char(char c);
int             should_expand_in_context(t_quote_state state);

// String building
int             expander_append_char(t_expander *expander, char c);
int             expander_append_string(t_expander *expander, const char *str);
int             expander_resize_result(t_expander *expander);

// Variable parsing
char            *parse_variable_name(const char *input, size_t *pos);
int             is_valid_var_char(char c);
int             expander_handle_variable(t_expander *expander);
char            *var_name_alloc(const char *input, size_t start, size_t len);
int             handle_braced_var(t_expander *expander);

// Character processing
int             expander_process_char(t_expander *expander, char c);
int             handle_quoted_char(t_expander *expander, char c);
int             handle_regular_char(t_expander *expander, char c);

// Escape handling
int             handle_backslash_escape(t_expander *expander);
int             handle_double_quote_escape(t_expander *expander, char next_char);
int             handle_normal_escape(t_expander *expander, char next_char);

#endif

