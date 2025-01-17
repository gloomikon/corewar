/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxiliary.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaliuta <msaliuta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 14:27:00 by mzhurba           #+#    #+#             */
/*   Updated: 2019/09/15 17:56:17 by msaliuta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

bool	is_delimiter(char c)
{
	return (ft_isspace(c) || c == '"' || c == '\0'
			|| c == DIRECT_CHAR
			|| c == SEPARATOR_CHAR
			|| c == COMMENT_CHAR || c == ANOTHER_COMMENT_CHAR);
}

int		divide_str(char **str, char **row)
{
	char *div;
	char *new;

	div = ft_strchr(*str, '\n') + 1;
	if (!(*row = ft_strsub(*str, 0, div - *str)))
		terminate(MEMORY_ALLOCATION);
	if (!ft_strlen(div))
		ft_strdel(str);
	else
	{
		if (!(new = ft_strdup(div)))
			terminate(MEMORY_ALLOCATION);
		ft_strdel(str);
		*str = new;
	}
	return (1);
}

int		read_next_line(int fd, char **row)
{
	char		*tmp;
	char		buff[BUFF_SIZE + 1];
	static char	*str = NULL;
	int			size;

	if (read(fd, buff, 0) < 0)
		return (-1);
	while (!ft_strchr(str, '\n'))
	{
		if ((size = read(fd, buff, BUFF_SIZE)) == 0)
		{
			if ((*row = str) == NULL)
				return (0);
			str = NULL;
			return (1);
		}
		buff[size] = '\0';
		tmp = str;
		if (!(str = ft_strjoin(str, buff)))
			terminate(MEMORY_ALLOCATION);
		ft_strdel(&tmp);
	}
	return (divide_str(&str, row));
}

char	*join_str(char **s1, char **s2)
{
	char	*res;

	if (!(res = ft_strjoin(*s1, *s2)))
		terminate(MEMORY_ALLOCATION);
	ft_strdel(s1);
	ft_strdel(s2);
	return (res);
}

bool	class_is_register(char *str)
{
	int i;
	int len;

	len = ft_strlen(str);
	if ((len == 2 || len == 3) && str[0] == 'r')
	{
		i = 1;
		while (ft_isdigit(str[i]))
			i++;
		if (str[i] == '\0' && ft_atoi(&str[1]) > 0)
			return (true);
	}
	return (false);
}
