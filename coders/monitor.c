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
		return (1);
	last_compile = get_last_compile_start(coder);
	if (now - last_compile >= coder->sim->config.time_to_burnout)
		return (1);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *) arg;
	while (!simulation_stopped(sim))
	{
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			if (coder_burned_out(&sim->coders[i]))
			{
				log_coder(&sim->coders[i], "burned out");
				stop_simulation(sim);
				return (NULL);
			}
			if (coders_done(sim))
			{
				stop_simulation(sim);
				return (NULL);
			}
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
