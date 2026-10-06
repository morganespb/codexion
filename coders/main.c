/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:00:11 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:52:04 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_config	config;
	t_sim		sim;

	if (parsing(ac, av, &config) != 0)
		return (1);
	if (init_simulation(&sim, config) != 0)
		return (1);
	sim.start_time = get_time_ms();
	if (sim.start_time == -1)
	{
		free(sim.coders);
		free(sim.dongles);
		return (1);
	}
	init_coder_times(&sim);
	if (start_threads(&sim) != 0 || join_threads(&sim) != 0)
	{
		free(sim.coders);
		free(sim.dongles);
		return (1);
	}
	cleanup_sim(&sim, sim.coders_initialized, sim.dongles_initialized);
	return (0);
}
