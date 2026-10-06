/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   dongles.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:51:49 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:51:49 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	wait_for_cooldown(t_coder *coder, t_dongle *dongle)
{
	long	now;

	while (dongle->last_release >= 0 && !simulation_stopped(coder->sim))
	{
		now = get_time_ms();
		if (now == -1)
		{
			stop_simulation(coder->sim);
			return (1);
		}
		if (now - dongle->last_release >= coder->sim->config.dongle_cooldown)
			return (0);
		usleep(100);
	}
	return (simulation_stopped(coder->sim));
}

static int	lock_dongle(t_coder *coder, int index)
{
	t_dongle	*dongle;

	dongle = &coder->sim->dongles[index];
	pthread_mutex_lock(&dongle->dongles_mutex);
	if (wait_for_cooldown(coder, dongle) != 0)
	{
		pthread_mutex_unlock(&dongle->dongles_mutex);
		return (1);
	}
	log_coder(coder, "has taken a dongle");
	return (0);
}

static int	wait_with_one_dongle(t_coder *coder, int index)
{
	while (!simulation_stopped(coder->sim))
		usleep(100);
	pthread_mutex_unlock(&coder->sim->dongles[index].dongles_mutex);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	int	first;
	int	second;

	first = coder->right_dongle;
	second = coder->left_dongle;
	if (first < second)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	if (lock_dongle(coder, first) != 0)
		return (1);
	if (first == second)
		return (wait_with_one_dongle(coder, first));
	if (lock_dongle(coder, second) != 0)
	{
		pthread_mutex_unlock(&coder->sim->dongles[first].dongles_mutex);
		return (1);
	}
	return (0);
}

void	release_dongles(t_coder *coder)
{
	int	first;
	int	second;
	long	now;

	first = coder->right_dongle;
	second = coder->left_dongle;
	if (first < second)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	now = get_time_ms();
	if (now == -1)
		stop_simulation(coder->sim);
	else
	{
		coder->sim->dongles[second].last_release = now;
		coder->sim->dongles[first].last_release = now;
	}
	pthread_mutex_unlock(&coder->sim->dongles[second].dongles_mutex);
	pthread_mutex_unlock(&coder->sim->dongles[first].dongles_mutex);
}
