/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:00:11 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:52:04 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_config	config;
	t_sim		sim;
	int			status;

	if (parsing(ac, av, &config) != 0)
		return (1);
	if (init_simulation(&sim, config) != 0)
		return (1);
	status = start_threads(&sim);
	if (status == 0)
		status = join_threads(&sim);
	cleanup_sim(&sim);
	return (status);
}
