/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   monitor.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:52:07 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:52:07 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_burned_out(t_coder *coder)
{
	long	now;
	long	last_compile;

	now = get_time_ms();
	if (now == -1)
		return (-1);
	last_compile = get_last_compile_start(coder);
	if (now - last_compile >= coder->sim->config.time_to_burnout)
		return (1);
	return (0);
}

static int	check_burnout(t_sim *sim)
{
	int	i;
	int	status;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		status = coder_burned_out(&sim->coders[i]);
		if (status != 0)
		{
			stop_simulation(sim);
			if (status > 0)
				log_burnout(&sim->coders[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *) arg;
	if (wait_for_start(sim))
		return (NULL);
	while (!simulation_stopped(sim))
	{
		if (check_burnout(sim))
			return (NULL);
		if (coders_done(sim))
		{
			stop_simulation(sim);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
