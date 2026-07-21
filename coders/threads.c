/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   threads.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:03:49 by mosimon          #+#    #+#              */
/*   Updated: 2026/07/20 16:03:49 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*coder_routine(void *arg)
{
	t_coder	*coder;

	//long    timestamp;
	//long    now;
	//long    elapsed;
	coder = (t_coder *) arg;
	log_coder(coder, "is running");
	precise_sleep(coder->sim, 200);
    if (!simulation_stopped(coder->sim))
        usleep(100);
    return (NULL);
}

int	start_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine, &sim->coders[i]) != 0)
			return (1);
		i++;
	}
    if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
        return (1);
    return (0);
}

int	join_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
			return (1);
		i++;
	}
    if (pthread_join(sim->monitor, NULL) != 0)
        return (1);
    return (0);
}
