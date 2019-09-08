/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ass_to_bytecode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhurba <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 01:58:09 by mzhurba           #+#    #+#             */
/*   Updated: 2019/09/08 18:48:36 by mzhurba          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ass_to_bytecode.h"

void	insert_ints_instead_mentions(t_pars *pars)
{
	t_mention	*mention;
	t_label		*label;

	label = pars->labels;
	while (label)
	{
		if (label->op_pos != -1)
		{
			mention = label->mentions;
			while (mention)
			{
				write_to_bytecode(pars->code, mention->pos,
						(label->op_pos - mention->op_pos), mention->size);
				mention = mention->next;
			}
		}
		else
			terminate_label(label);
		label = label->next;
	}
}

void	read_file(t_pars *pars)
{
	char	*line;
	int		res;

	pars->end = new_entity(pars, END);
	while ((res = read_next_line(pars->fd, &line)) > 0
		&& !(pars->col = 0)
		&& (++pars->row))
		while (line[pars->col])
			skip_whitespaces(&(pars->col), line)
			&& skip_comment(&(pars->col), line)
			&& (line[pars->col] != '\0')
			&& get_entities(pars, &line);
	add_entity(&(pars->entities), pars->end);
	res == -1 ? terminate("An error occurred while opening file") : 0;
}

int		reverse_file_fd(char *file)
{
	int		fd;
	char	*new;
	char	*ext;
	char	*old;

	ext = ft_strrchr(file, '.');
	old = ft_strsub(file, 0, ext - file);
	new = ft_strjoin(old, ft_strequ(ext, ".s") ? ".cor" : ".s");
	ft_strdel(&old);
	fd = open(new, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		terminate("Can not create file =(");
	ft_printf("Writing output program to %s\n", new);
	ft_strdel(&new);
	return (fd);
}

void	write_code_to_bytecode(t_pars *pars, int fd)
{
	char	*bcode;
	int		len;

	len = 4 + PROG_NAME_LENGTH + 4 + 4 + COMMENT_LENGTH + 4 + pars->pos;
	bcode = ft_memalloc(sizeof(char) * (len + 1));
	write_to_bytecode(bcode, 0, COREWAR_EXEC_MAGIC, 4);
	ft_memcpy(&(bcode[4]), pars->name, ft_strlen(pars->name));
	write_to_bytecode(bcode, 4 + PROG_NAME_LENGTH + 4, pars->pos, 4);
	ft_memcpy(&(bcode[4 + PROG_NAME_LENGTH + 4 + 4]), pars->comment,
									ft_strlen(pars->comment));
	ft_memcpy(&(bcode[4 + PROG_NAME_LENGTH + 4 + 4 + COMMENT_LENGTH + 4]),
			pars->code, pars->pos);
	write(fd, bcode, len);
	ft_strdel(&bcode);
}

int		ass_to_bytecode(char *file)
{
	t_entity	*curr;
	t_pars		*pars;
	int			fd;

	(fd = open(file, O_RDONLY)) < 0 ? terminate("Invalid file") : 0;
	pars = init_pars(fd);
	read_file(pars);
	curr = pars->entities;
	get_champ_bio(pars, &curr);
	read_and_proc_entities(pars, &curr);
	fd = reverse_file_fd(file);
	write_code_to_bytecode(pars, fd);
	return (1);
}
