/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   time.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:52:19 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:52:25 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return (time.tv_sec * 1000L + time.tv_usec / 1000L);
}

void	precise_sleep(t_sim *sim, long duration_ms)
{
	long	start;
	long	now;

	start = get_time_ms();
	if (start == -1)
		return ;
	while (!simulation_stopped(sim))
	{
		now = get_time_ms();
		if (now == -1)
			return ;
		if (now - start >= duration_ms)
			break ;
		usleep(100);
	}
}
