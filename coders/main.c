/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:00:11 by mosimon          #+#    #+#              */
/*   Updated: 2026/07/20 16:00:11 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//static void print_sim(t_sim *sim)
//{
//    int i;
//
//    i = 0;
//    while (i < sim->config.number_of_coders)
//    {
//        printf("Coder %d: left=%d, right=%d, compiles=%d\n",
//            sim->coders[i].id,
//            sim->coders[i].left_dongle,
//            sim->coders[i].right_dongle,
//            sim->coders[i].compile_count);
//        i++;
//    }
//}

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
	if (start_threads(&sim) != 0)
	{
		free(sim.coders);
		free(sim.dongles);
		return (1);
	}
	if (join_threads(&sim) != 0)
	{
		free(sim.coders);
		free(sim.dongles);
		return (1);
	}
	//print_sim(&sim);
	pthread_mutex_destroy(&sim.print_mutex);
	pthread_mutex_destroy(&sim.state_mutex);
    free(sim.coders);
	free(sim.dongles);
	return (0);
}
