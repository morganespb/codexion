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

static const char	*get_coder_color(int id)
{
	const char	*colors[6];

	colors[0] = "\033[31m";
	colors[1] = "\033[32m";
	colors[2] = "\033[33m";
	colors[3] = "\033[34m";
	colors[4] = "\033[35m";
	colors[5] = "\033[36m";
	return (colors[(id - 1) % 6]);
}

void	log_coder(t_coder *coder, const char *message)
{
	long	timestamp;

	pthread_mutex_lock(&coder->sim->print_mutex);
	if (!simulation_stopped(coder->sim))
	{
		timestamp = get_time_ms() - coder->sim->start_time;
		printf("%s%ld %d %s\033[0m\n", get_coder_color(coder->id),
			timestamp, coder->id, message);
	}
	pthread_mutex_unlock(&coder->sim->print_mutex);
}

void	log_burnout(t_coder *coder)
{
	long	timestamp;

	pthread_mutex_lock(&coder->sim->print_mutex);
	timestamp = get_time_ms() - coder->sim->start_time;
	printf("%s%ld %d burned out\033[0m\n", get_coder_color(coder->id),
		timestamp, coder->id);
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
