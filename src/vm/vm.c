/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzhurba <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 16:42:00 by mzhurba           #+#    #+#             */
/*   Updated: 2019/09/20 18:08:46 by mzhurba          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "corewar_instructions.h"

int		main(int argc, char **argv)
{
	t_corewar	*cw;

	if (argc < 2)
	{
		display_usage();
		return (1);
	}
	parse_args(argc, argv, (cw = new_corewar()));
	create_start_data(cw);
	printf("%d %d\n", cw->debug_mode, cw->debug_cycles);
	display_start_message(cw->champs) && run_battle(cw);
	display_result(cw);
}
