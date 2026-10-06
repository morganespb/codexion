/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   scheduler.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:52:12 by mosimon          #+#    #+#              */
/*   Updated: 2026/10/06 12:09:07 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
//For fifo: smaller request_order = higher priority
//for edf: smaller deadline = higher priority

//PRIORITY

int	higher_priority(t_sim *sim, t_coder *first, t_coder *second)
{
	long	deadline_1;
	long	deadline_2;

	if (sim->config.scheduler_type == FIFO)
		return (first->request_order < second->request_order);
	deadline_1 = get_last_compile_start(first) + sim->config.time_to_burnout;
	deadline_2 = get_last_compile_start(second) + sim->config.time_to_burnout;
	return (deadline_1 < deadline_2);
}

//SCHEDULER

void	request_permission(t_coder *coder)
{
	t_scheduler	*scheduler;

	scheduler = &coder->sim->scheduler;
	pthread_mutex_lock(&scheduler->sched_mutex);
	coder->waiting = 1;
	coder->permission_granted = 0;
	coder->request_order = scheduler->next_order++;
	heap_insert(coder->sim, coder);
	while (!coder->permission_granted && !simulation_stopped(coder->sim))
		pthread_cond_wait(&scheduler->condition, &scheduler->sched_mutex);
	coder->waiting = 0;
	pthread_mutex_unlock(&scheduler->sched_mutex);
	if (simulation_stopped(coder->sim))
		return (1);
	return (0);
}

static void	grant_access(t_sim *sim)
{
	t_coder	*next;

	if (sim->scheduler.size == 0)
		return ;
	next = heap_pop(sim);
	if (next == NULL)
		return ;
	next->permission_granted = 1;
	phthread_cond_broadcast(&sim->scheduler.condition);
}

static void	notify_scheduler(t_sim *sim)
{
	pthread_mutex_lock(&sim->scheduler.mutex);
	grant_next(sim);
	phthread_cond_broadcast(&sim->scheduler.conditon);
	pthread_mutex_unlock(&sim->scheduler.mutex);
}
