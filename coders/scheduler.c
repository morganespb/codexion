/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   scheduler.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:52:12 by mosimon          #+#    #+#              */
/*   Updated: 2026/10/06 14:24:14 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	higher_priority(t_sim *sim, t_coder *first, t_coder *second)
{
	long	last_compile_1;
	long	last_compile_2;

	if (sim->config.scheduler_type == FIFO_ORDER)
		return (first->request_order < second->request_order);
	last_compile_1 = get_last_compile_start(first);
	last_compile_2 = get_last_compile_start(second);
	if (last_compile_1 == last_compile_2)
		return (first->request_order < second->request_order);
	return (last_compile_1 < last_compile_2);
}

static void	grant_access(t_sim *sim)
{
	t_coder	*next;

	if (sim->scheduler.size == 0
		|| sim->scheduler.acquisition_in_flight)
		return ;
	next = heap_pop(sim);
	if (next == NULL)
		return ;
	next->permission_granted = 1;
	sim->scheduler.acquisition_in_flight = 1;
	pthread_cond_broadcast(&sim->scheduler.condition);
}

static int	finish_request(t_coder *coder, t_scheduler *scheduler)
{
	coder->waiting = 0;
	if (simulation_stopped(coder->sim))
	{
		if (coder->permission_granted)
			scheduler->acquisition_in_flight = 0;
		pthread_mutex_unlock(&scheduler->sched_mutex);
		return (1);
	}
	pthread_mutex_unlock(&scheduler->sched_mutex);
	return (0);
}

int	request_permission(t_coder *coder)
{
	t_scheduler	*scheduler;

	scheduler = &coder->sim->scheduler;
	pthread_mutex_lock(&scheduler->sched_mutex);
	if (simulation_stopped(coder->sim))
	{
		pthread_mutex_unlock(&scheduler->sched_mutex);
		return (1);
	}
	coder->waiting = 1;
	coder->permission_granted = 0;
	coder->request_order = scheduler->next_order++;
	heap_insert(coder->sim, coder);
	grant_access(coder->sim);
	while (!coder->permission_granted && !simulation_stopped(coder->sim))
		pthread_cond_wait(&scheduler->condition, &scheduler->sched_mutex);
	return (finish_request(coder, scheduler));
}

void	notify_scheduler(t_sim *sim)
{
	pthread_mutex_lock(&sim->scheduler.sched_mutex);
	sim->scheduler.acquisition_in_flight = 0;
	if (!simulation_stopped(sim))
		grant_access(sim);
	pthread_cond_broadcast(&sim->scheduler.condition);
	pthread_mutex_unlock(&sim->scheduler.sched_mutex);
}
