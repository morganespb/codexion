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
#define RESET "\033[0m"

static char	*get_coder_color(int id)
{
	char	*colors[6];

	colors[0] = "\033[31m";
	colors[1] ="\033[32m";
	colors[2] =	"\033[33m";
	colors[3] =	"\033[34m";
	colors[4] =	"\033[35m";
	colors[5] =	"\033[36m";
	return (colors[(id - 1) % 6]);
}

void	log_coder(t_coder *coder, char *message)
{
	long	timestamp;
	char	*color;
	char	*reset;

	color = get_coder_color(coder->id);
	reset = RESET;
	timestamp = get_time_ms() - coder->sim->start_time;
	pthread_mutex_lock(&coder->sim->print_mutex);
	printf("%s%ldms - coder %d %s%s\n",
		color, timestamp, coder->id, message, reset);
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
