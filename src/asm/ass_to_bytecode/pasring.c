/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pasring.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhurba <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 14:40:00 by mzhurba           #+#    #+#             */
/*   Updated: 2019/09/01 14:40:00 by mzhurba          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ass_to_bytecode.h"

void	read_file(t_pars *pars)
{
	char	*line;
	int		res;

	while (!(pars->col = 0)
		   && (++pars->row)
		   && (res = read_next_line(pars->fd, &line)) > 0)
		while (line[pars->col])
			skip_whitespaces(&(pars->col), line)
			&& skip_comment(&(pars->col), line)
			&& (line[pars->col] != '\0')
			&& get_entities(pars, &line);
	res == -1 ? terminate("An error occurred while opening file") : 0;
}

int		get_entities(t_pars *pars, char **row)
{
	if ((*row)[pars->col] == '\n' && (++(pars->col)))
		add_entity(&(pars->entities), new_entity(pars, NEW_LINE));
	else if ((*row)[pars->col] == SEPARATOR_CHAR && (++(pars->col)))
		add_entity(&(pars->entities), new_entity(pars, SEPARATOR));
	else if ((*row)[pars->col] == '.')
		parse_chars(pars, *row, ++(pars->col), new_entity(pars, COMMAND));
	else if ((*row)[pars->col] == DIRECT_CHAR && (++(pars->col)))
		(*row)[pars->col] == LABEL_CHAR && (++(pars->col)) ?
		parse_chars(pars, *row, pars->col - 2, new_entity(pars, DIRECT_LABEL)) :
		parse_int(pars, *row, pars->col - 1, new_entity(pars, DIRECT));
	else if ((*row)[pars->col] == '"')
		parse_str(pars, row, pars->col, new_entity(pars, STRING));
	else if ((*row)[pars->col] == LABEL_CHAR)
		parse_chars(pars, *row, ++(pars->col),
					new_entity(pars, INDIRECT_LABEL));
	else
		parse_int(pars, *row, pars->col, new_entity(pars, INDIRECT));
	return (1);
}

void	parse_chars(t_pars *pars, char *row, int start, t_entity *entity)
{
	size_t	col;

	entity->col = start;
	col = pars->col;
	while (row[pars->col] != '\0' && ft_strchr(LABEL_CHARS, row[pars->col]))
		++(pars->col);
	if ((pars->col - col) && row[pars->col] == LABEL_CHAR && (++(pars->col)))
	{
		entity->content =  ft_strsub(row, start, pars->col - start);
		entity->class = LABEL;
		add_entity(&(pars->entities), entity);
	}
	else if ((pars->col - col) && is_delimiter(row[pars->col]))
	{
		entity->content = ft_strsub(row, start, pars->col - start);
		if (entity->class == INDIRECT)
			entity->class = class_is_register(entity->content) ?
							REGISTER : OPERATOR;
		add_entity(&(pars->entities), entity);
	}
	else
		terminate_lexical(pars->row, pars->col);
}

void	parse_int(t_pars *pars, char *row, int start, t_entity *entity)
{
	size_t	col;

	entity->col = start;
	row[pars->col] == '-' ? ++(pars->col) : 0;
	col = pars->col;
	while (ft_isdigit(row[pars->col]))
		++(pars->col);
	if ((pars->col - col)
		&& (entity->class == DIRECT || is_delimiter(row[pars->col])))
	{
		entity->content = ft_strsub(row, start, pars->col - start);
		add_entity(&(pars->entities), entity);
	}
	else if (entity->class != DIRECT)
	{
		pars->col = start;
		parse_chars(pars, row, start, entity);
	}
	else
		terminate_lexical(pars->row, pars->col);
}

void	parse_str(t_pars *pars, char **row, int start, t_entity *entity)
{
	long long	size;
	char		*str;
	char		*end;

	entity->col = start;
	size = 1;
	while (!(end = ft_strchr(*row + start + 1, '"'))
		   && (size = read_next_line(pars->fd, &str)))
		*row = join_str(row, &str);
	upd_pars_row_and_col(pars, *row);
	if (!size)
		terminate_lexical(pars->row, pars->col);
	entity->content = ft_strsub(*row, start, end + 1 - (*row) - start);
	(end - pars->col != *row) ? upd_row(row, end - pars->col) : 0;
	++(pars->col);
	add_entity(&(pars->entities), entity);
}
