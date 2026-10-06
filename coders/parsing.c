/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:52:11 by mosimon          #+#    #+#              */
/*   Updated: 2026/08/24 13:52:11 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_scheduler(char *str, t_scheduler_type *scheduler_type)
{
	if (strcmp(str, "fifo") == 0)
	{
		*scheduler_type = FIFO_ORDER;
		return (0);
	}
	if (strcmp(str, "edf") == 0)
	{
		*scheduler_type = EDF_ORDER;
		return (0);
	}
	return (1);
}

static int	parse_int(char *str, long *result)
{
	int		i;
	long	number;

	number = 0;
	i = 0;
	if (!str[0])
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		if (number > (LONG_MAX - (str[i] - '0')) / 10)
			return (1);
		number = number * 10 + (str[i] - '0');
		i++;
	}
	*result = number;
	return (0);
}

static int	parse_config_int(char *str, int *result, int minimum)
{
	long	value;

	if (parse_int(str, &value) != 0)
		return (1);
	if (value < minimum || value > INT_MAX)
		return (1);
	*result = (int)value;
	return (0);
}

static int	parse_values(char **av, t_config *config)
{
	if (parse_config_int(av[1], &config->number_of_coders, 1) != 0)
		return (1);
	if (parse_config_int(av[2], &config->time_to_burnout, 0) != 0)
		return (1);
	if (parse_int(av[3], &config->time_to_compile) != 0)
		return (1);
	if (parse_int(av[4], &config->time_to_debug) != 0)
		return (1);
	if (parse_int(av[5], &config->time_to_refactor) != 0)
		return (1);
	if (parse_config_int(av[6],
			&config->number_of_compiles_required, 1) != 0)
		return (1);
	if (parse_int(av[7], &config->dongle_cooldown) != 0)
		return (1);
	return (0);
}

int	parsing(int ac, char **av, t_config *config)
{
	if (ac != 9)
	{
		fprintf(stderr, "Error: expected 8 arguments\n");
		return (1);
	}
	if (parse_values(av, config) != 0)
		return (1);
	if (parse_scheduler(av[8], &config->scheduler_type) != 0)
	{
		fprintf(stderr, "Error: scheduler must be fifo or edf\n");
		return (1);
	}
	return (0);
}
