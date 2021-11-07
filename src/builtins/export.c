/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emadriga <emadriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 20:42:55 by emadriga          #+#    #+#             */
/*   Updated: 2021/11/07 13:44:06 by emadriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define EXPORT_WRONG_ID "export: `%c': not a valid identifier\n"

/**
 * * This should recreate the bash funtion "env".
 * * Returms enviroment variables list
 * @param env_list	enviroment list
*/
void static	ft_print_env_without_last_cmd(t_str **env_list)
{
	t_str	*aux;

	aux = *env_list;
	while (aux != NULL)
	{
		if (!ft_strncmp(aux->str, "_=/", 3))
			printf("%s\n", aux->str);
		aux = aux->next;
	}
}

/**
 * * This should recreate the bash funtion "export".
 * * Returns ENV alphabetically-sorted  
 * * Add records (through argumment vector) to enviroment variables list
 * @param env_list	enviroment list
 * @param argv	vector of arguments containing records to add
*/
void	ft_export(t_str **env_list, char **argv)
{
	int		i;
	char	*env_desc;
	char	*strchr_n;

	i = 1;
	env_desc = NULL;
	if (argv[i] == NULL)
		ft_print_env_without_last_cmd(env_list);
	while (argv[i] != NULL)
	{
		if (!ft_isalpha(argv[i][0]))
			printf(EXPORT_WRONG_ID, argv[i][0]);
		strchr_n = ft_strchr(argv[i], '=');
		if (strchr_n)
			env_desc = ft_substr(argv[i], 0, strchr_n - argv[i] + 1);
		else
			env_desc = ft_strdup(argv[i]);
		ft_lst_str_delete(env_list, env_desc);
		ft_lst_str_add_sorted(env_list, argv[i]);
		free(env_desc);
		i++;
	}
}
