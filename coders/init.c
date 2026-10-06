/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:01:14 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:51:55 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].last_release = -1;
		if (pthread_mutex_init(&sim->dongles[i].dongles_mutex, NULL) != 0)
			return (1);
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
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].left_dongle = i;
		sim->coders[i].right_dongle = (i + 1) % count;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].request_order = 0;
		sim->coders[i].permission_granted = 0;
		sim->coders[i].waiting = 0;
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].data_mutex, NULL) != 0)
			return (1);
		sim->coders_initialized++;
		i++;
	}
	return (0);
}

static int	init_sched(t_sim *sim)
{
	t_scheduler	*scheduler;

	scheduler = &sim->scheduler;
	scheduler->capacity = sim->config.number_of_coders;
	scheduler->size = 0;
	scheduler->next_order = 0;
	scheduler->currently_taking_dongles = 0;
	scheduler->heap = malloc(sizeof(*scheduler->heap)
			* (size_t)scheduler->capacity);
	if (scheduler->heap == NULL)
		return (1);
	if (pthread_mutex_init(&scheduler->sched_mutex, NULL) != 0)
	{
		free(scheduler->heap);
		return (1);
	}
	if (pthread_cond_init(&scheduler->condition, NULL) != 0)
	{
		pthread_mutex_destroy(&scheduler->sched_mutex);
		free(scheduler->heap);
		return (1);
	}
	sim->scheduler_initialized = 1;
	return (0);
}

static int	init_storage(t_sim *sim)
{
	sim->coders = malloc((size_t)sim->config.number_of_coders
			* sizeof(*sim->coders));
	sim->dongles = malloc((size_t)sim->config.number_of_coders
			* sizeof(*sim->dongles));
	if (!sim->coders || !sim->dongles)
	{
		free(sim->coders);
		free(sim->dongles);
		return (1);
	}
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		free(sim->coders);
		free(sim->dongles);
		return (1);
	}
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_mutex);
		free(sim->coders);
		free(sim->dongles);
		return (1);
	}
	return (0);
}

int	init_simulation(t_sim *sim, t_config config)
{
	sim->config = config;
	sim->stopped = 0;
	sim->started = 0;
	sim->coders_initialized = 0;
	sim->dongles_initialized = 0;
	sim->scheduler_initialized = 0;
	if (config.number_of_coders <= 0 || init_storage(sim) != 0)
		return (1);
	if (init_coders(sim) != 0 || init_dongles(sim) != 0
		|| init_sched(sim) != 0)
	{
		cleanup_sim(sim);
		return (1);
	}
	return (0);
}
