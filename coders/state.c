/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   state.c                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:52:14 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:52:14 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_stopped(t_sim *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->state_mutex);
	stopped = sim->stopped;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stopped);
}

int	stop_simulation(t_sim *sim)
{
	int	first;

	pthread_mutex_lock(&sim->state_mutex);
	first = (sim->stopped == 0);
	sim->stopped = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	if (first && sim->scheduler_initialized)
	{
		pthread_mutex_lock(&sim->scheduler.sched_mutex);
		pthread_cond_broadcast(&sim->scheduler.condition);
		pthread_mutex_unlock(&sim->scheduler.sched_mutex);
	}
	return (first);
}

int	wait_for_start(t_sim *sim)
{
	int	ready;
	int	stopped;

	ready = 0;
	while (!ready)
	{
		pthread_mutex_lock(&sim->state_mutex);
		ready = sim->started || sim->stopped;
		stopped = sim->stopped;
		pthread_mutex_unlock(&sim->state_mutex);
		if (!ready)
			usleep(100);
	}
	return (stopped);
}

int	begin_simulation(t_sim *sim)
{
	long	start;
	int		i;

	start = get_time_ms();
	if (start == -1)
		return (1);
	i = 0;
	while (i < sim->config.number_of_coders)
		sim->coders[i++].last_compile_start = start;
	pthread_mutex_lock(&sim->state_mutex);
	sim->start_time = start;
	sim->started = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	return (0);
}
