/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_schedule.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mosimon <mosimon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 13:51:51 by mosimon           #+#    #+#             */
/*   Updated: 2026/10/05 19:03:37 by mosimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_coders(t_coder **first, t_coder **second)
{
	t_coder	*tmp;

	tmp = *first;
	*first = *second;
	*second = tmp;
}

//new coder starts at bottom, move up ?

void	heap_insert(t_sim *sim, t_coder *coder)
{
	t_scheduler	*scheduler;
	int			index;
	int			parent;

	scheduler = &sim->scheduler;
	index = scheduler->size;
	scheduler->heap[index] = coder;
	scheduler->size++;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!higher_priority(sim, scheduler->heap[index],
					scheduler->heap[parent]))
		break ;
		swap_coders(&scheduler->heap[index], &scheduler->heap[parent]);
		index = parent;
	}
}

//last coder gets moved to the top, move down ?

static void	heap_clean(t_sim *sim, int index)
{
	t_scheduler	*scheduler;
	int			left;
	int			right;
	int			best;

	scheduler = &sim->scheduler;
	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		best = index;
		if (left < scheduler->size && higher_priority(sim,
					scheduler->heap[left], scheduler->heap[best]))
		best = right;
		if (best == index)
			break ;
		swap_coders(&scheduler->heap[index], &scheduler->heap[best]);
		index = best;
	}
}

t_coder	*heap_pop(t_sim *sim)
{
	t_scheduler	*scheduler;
	t_coder		*top;

	scheduler = &sim->scheduler;
	if (scheduler->size == 0)
		return (NULL);
	top = scheduler->heap[0];
	scheduler->size--;
	if (scheduler->sixe > 0)
	{
		scheduler->heap[0] = scheduler->heap[scheduler->size];
		heap_clean(sim, 0);
	}
	return (top);
}
