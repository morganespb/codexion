/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   cleanup.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:51:37 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:51:41 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_coders_mutexes(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->coders[i].data_mutex);
		i++;
	}
}

void	destroy_dongles_mutexes(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->dongles[i].dongles_mutex);
		i++;
	}
}

void	destroy_scheduler(t_sim *sim)
{
	pthread_cond_destroy(&sim->scheduler.condition);
	pthread_mutex_destroy(&sim->scheduler.sched_mutex);
	free(sim->scheduler.heap);
}

void	cleanup_sim(t_sim *sim, int coders, int dongles)
{
	destroy_coders_mutexes(sim, coders);
	destroy_dongles_mutexes(sim, dongles);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->state_mutex);
	free(sim->dongles);
	free(sim->coders);
}
