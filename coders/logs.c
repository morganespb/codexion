/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   logs.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:01:23 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:51:56 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_coder(t_coder *coder, char *message)
{
	long	timestamp;

	pthread_mutex_lock(&coder->sim->print_mutex);
	if (!simulation_stopped(coder->sim))
	{
		timestamp = get_time_ms() - coder->sim->start_time;
		printf("%ld %d %s\n", timestamp, coder->id, message);
	}
	pthread_mutex_unlock(&coder->sim->print_mutex);
}

void	log_burnout(t_coder *coder)
{
	long	timestamp;

	pthread_mutex_lock(&coder->sim->print_mutex);
	timestamp = get_time_ms() - coder->sim->start_time;
	printf("%ld %d burned out\n", timestamp, coder->id);
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
