/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   logs.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:01:23 by mosimon          #+#    #+#              */
/*   Updated: 2026/07/20 16:06:58 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_coder(t_coder *coder, char *message)
{
	long	timestamp;

	timestamp = get_time_ms() - coder->sim->start_time;
    pthread_mutex_lock(&coder->sim->print_mutex);
	printf("%ldms - coder %d %s\n", timestamp, coder->id, message);
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
