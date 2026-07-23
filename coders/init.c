/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:01:14 by mosimon          #+#    #+#              */
/*   Updated: 2026/07/20 16:01:14 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int  init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
    sim->dongles_initialized = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->dongles[i].id = i;
        if (pthread_mutex_init(&sim->dongles[i].dongles_mutex, NULL) != 0)
        {
            destroy_dongles_mutexes(sim, i);
            return (1);
        }
		i++;
        sim->dongles_initialized++;
	}
    return (0);
}

static int	init_coders(t_sim *sim)
{
	int	i;
	int	count;

	i = 0;
	count = sim->config.number_of_coders;
    sim->coders_initialized = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].left_dongle = i;
		sim->coders[i].right_dongle = (i + 1) % count;
		sim->coders[i].compile_count = 0;
		sim->coders[i].sim = sim;
        if (pthread_mutex_init(&sim->coders[i].data_mutex, NULL) != 0)
        {
            destroy_coders_mutexes(sim, i);
            return (1);
        }
        sim->coders_initialized++;
        i++;
	}
    return (0);
}

void    init_coder_times(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        sim->coders[i].last_compile_start = sim->start_time;
        i++;
    }
}

int	init_simulation(t_sim *sim, t_config config)
{
	sim->config = config;
    sim->stopped = 0;
	sim->coders = malloc(sizeof(t_coder) * config.number_of_coders);
	if (!sim->coders)
		return (1);
	sim->dongles = malloc(sizeof(t_dongle) * config.number_of_coders);
	if (!sim->dongles)
		return (1);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		free(sim->coders);
		free(sim->dongles);
		return (1);
	}
    //printf("INIT print: %p\n", (void *)&sim->print_mutex);
    if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
    {
	    free(sim->coders);
		free(sim->dongles);
		return (1);
	}
	if (init_coders(sim) != 0 || init_dongles(sim) != 0)
    {
        cleanup_sim(sim, sim->coders_initialized, sim->dongles_initialized); 
        return (1);
    }
	return (0);
}
