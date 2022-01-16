/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piped_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 11:08:30 by emadriga          #+#    #+#             */
/*   Updated: 2022/01/16 13:44:03 by emadriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"minishell.h"

/**
 * * Return size of argv's process to malloc
 * @param tokens	current tokens list
*/
static int	get_process_argv_len(char **tokens)
{
	int		i;
	int		type_redir;

	i = 0;
	while (*tokens != NULL && ft_strcmp(*tokens, "|"))
	{
		type_redir = eval_token_redir(*tokens);
		i += (type_redir == NONE);
		if (type_redir != NONE)
			tokens++;
		tokens++;
	}
	return (i);
}

/**
 * * Add redirection info to current process
 * @param token			current token to read from
 * @param process		current process to initialize
 * @param type_redir	redirection type
*/
static void	add_redir_to_process(char *token, t_pp	*process, int type_redir)
{
	t_redir	*new;

	if (token != NULL)
	{
		new = lst_redir_new();
		new->type = type_redir;
		if (type_redir != HEREDOC)
			new->go_to = ft_strdup(token);
		else
		{
			new->go_to = get_heredoc_pipedfork(token);
			if (!ft_strcmp(new->go_to, token))
				g_var.last_cmd_status = 130;
			if (!ft_strcmp(new->go_to, ""))
			{
				free(new->go_to);
				new->go_to = NULL;
			}
		}
		lst_redir_add_back(&process->redir, new);
	}
}

/**
 * * Init process with needed info to execute later
 * @param tokens	current tokens list
 * @param process	current process to initialize
*/
static void	init_piped_process(char **tokens, t_pp	*process)
{
	int		i;
	int		type_redir;

	process->argv = malloc(sizeof(char *) * (get_process_argv_len(tokens) + 1));
	process->argv[0] = adv_qm_rem(*tokens++, NOT_FREE);
	if (process->is_cmd)
		process->pathname = new_getpath(process->argv[0], &g_var.env);
	i = 1;
	while (*tokens != NULL && ft_strcmp(*tokens, "|"))
	{
		type_redir = eval_token_redir(*tokens);
		if (type_redir != NONE)
			add_redir_to_process(*(++tokens), process, type_redir);
		else
			process->argv[i++] = ft_strdup(*tokens);
		if (g_var.last_cmd_status != NONE)
			break ;
		tokens++;
	}
	process->argv[i] = NULL;
}

/**
 * * Returns processes to handle with pipes
 * * Every process has needed info to execute later
 * @param tokens	tokens list
 * @param processes	list of process to return
*/
void	get_piped_processes(char **tokens, t_pp **processes)
{
	t_pp	*process;
	int		type_token;

	g_var.last_cmd_status = NONE;
	while (*tokens != NULL && !g_var.last_cmd_status)
	{
		if (!ft_strcmp(*tokens, "|"))
			tokens++;
		type_token = eval_token(*tokens);
		if (type_token != BUILTIN && type_token != COMMAND)
			break ;
		process = NULL;
		process = lst_process_new();
		process->is_cmd = type_token == COMMAND;
		init_piped_process(tokens, process);
		lst_process_add_back(processes, process);
		while (*tokens != NULL && ft_strcmp(*tokens, "|"))
			tokens++;
	}
	if (g_var.last_cmd_status != NONE)
		lst_process_free(processes);
}
