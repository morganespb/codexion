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

void	take_dongles(t_coder *coder)
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
	pthread_mutex_lock(&coder->sim->dongles[first].dongles_mutex);
	log_coder(coder, "has taken a first dongle");
	pthread_mutex_unlock(&coder->sim->dongles[first].dongles_mutex);
	pthread_mutex_lock(&coder->sim->dongles[second].dongles_mutex);
	log_coder(coder, "has taken a second dongle");
	pthread_mutex_unlock(&coder->sim->dongles[second].dongles_mutex);
}

void	release_dongles(t_coder *coder)
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
	pthread_mutex_lock(&coder->sim->dongles[first].dongles_mutex);
	log_coder(coder, "has released a first dongle");
	pthread_mutex_unlock(&coder->sim->dongles[first].dongles_mutex);
	pthread_mutex_lock(&coder->sim->dongles[second].dongles_mutex);
	log_coder(coder, "has released a second dongle");
	pthread_mutex_unlock(&coder->sim->dongles[second].dongles_mutex);
}
