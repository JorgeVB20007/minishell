#include "minishell.h"

/*
 * This should recreate the bash funtion "echo".
*/
void	ft_echo(char **list, int *idx)
{
	int		n_flag;
	char	*assist;

	n_flag = 0;
	(*idx)++;
	if (list[*idx] == NULL)
	{
		write(1, "\n", 1);
		return ;
	}
	assist = adv_qm_rem(list[*idx]);
	if (!strncmp(assist, "-n\0", 3))
	{
		n_flag = 1;
		(*idx)++;
	}
	while (list[*idx] && list[*idx][0] != '|')
	{
		if (list[*idx][0] == '>' || list[*idx][0] == '<')
			(*idx) += 2;
		free(assist);
		if (!list[*idx] || list[*idx][0] != '|')
			break ;
		assist = adv_qm_rem(list[(*idx)++]);
		ft_putstr_fd(assist, 1);
		if (list[*idx] && list[*idx][0] != '|')
		{
			if (list[*idx][0] == '>')
			write(1, " ", 1);
		}
			
	}
	if (!n_flag)
		write(1, "\n", 1);
	free(assist);
}
