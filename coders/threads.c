/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   threads.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:03:49 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:52:16 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	if (wait_for_start(coder->sim))
		return (NULL);
	while (!simulation_stopped(coder->sim))
	{
		if (compile(coder) != 0)
			break ;
		if (debug(coder) != 0)
			break ;
		if (refactor(coder) != 0)
			break ;
	}
	return (NULL);
}

static int	rollback_threads(t_sim *sim, int count, int monitor_started)
{
	stop_simulation(sim);
	if (monitor_started)
		pthread_join(sim->monitor, NULL);
	while (count > 0)
	{
		count--;
		pthread_join(sim->coders[count].thread, NULL);
	}
	return (1);
}

int	start_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread,
				NULL, coder_routine, &sim->coders[i]) != 0)
			return (rollback_threads(sim, i, 0));
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
		return (rollback_threads(sim, i, 0));
	if (begin_simulation(sim) != 0)
		return (rollback_threads(sim, i, 1));
	return (0);
}

int	join_threads(t_sim *sim)
{
	int	i;
	int	error;

	error = 0;
	if (pthread_join(sim->monitor, NULL) != 0)
	{
		stop_simulation(sim);
		error = 1;
	}
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
			error = 1;
		i++;
	}
	return (error);
}
