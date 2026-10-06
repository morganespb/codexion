/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   coder_data.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:51:45 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:51:45 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_last_compile_start(t_coder *coder)
{
	long	value;

	pthread_mutex_lock(&coder->data_mutex);
	value = coder->last_compile_start;
	pthread_mutex_unlock(&coder->data_mutex);
	return (value);
}

void	set_last_compile_start(t_coder *coder, long value)
{
	pthread_mutex_lock(&coder->data_mutex);
	coder->last_compile_start = value;
	pthread_mutex_unlock(&coder->data_mutex);
}

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->data_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->data_mutex);
}

int	get_compile_count(t_coder *coder)
{
	int	count;

	pthread_mutex_lock(&coder->data_mutex);
	count = coder->compile_count;
	pthread_mutex_unlock(&coder->data_mutex);
	return (count);
}

int	coders_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (get_compile_count(&sim->coders[i])
			< sim->config.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}
