/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   coder_actions.c                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/08/24 13:51:43 by mosimon          #+#    #+#              */
/*   Updated: 2026/10/06 14:08:30 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	abort_compile(t_coder *coder)
{
	release_dongles(coder);
	notify_scheduler(coder->sim);
	return (1);
}

static int	begin_compile(t_coder *coder)
{
	long	now;

	if (request_permission(coder) != 0)
		return (1);
	if (take_dongles(coder) != 0)
	{
		notify_scheduler(coder->sim);
		return (1);
	}
	if (simulation_stopped(coder->sim))
		return (abort_compile(coder));
	now = get_time_ms();
	if (now == -1)
	{
		stop_simulation(coder->sim);
		return (abort_compile(coder));
	}
	set_last_compile_start(coder, now);
	log_coder(coder, "is compiling");
	notify_scheduler(coder->sim);
	return (0);
}

int	compile(t_coder *coder)
{
	if (begin_compile(coder) != 0)
		return (1);
	precise_sleep(coder->sim, coder->sim->config.time_to_compile);
	if (!simulation_stopped(coder->sim))
	{
		increment_compile_count(coder);
		if (coders_done(coder->sim))
			stop_simulation(coder->sim);
	}
	release_dongles(coder);
	if (simulation_stopped(coder->sim))
		return (1);
	return (0);
}

int	debug(t_coder *coder)
{
	log_coder(coder, "is debugging");
	precise_sleep(coder->sim, coder->sim->config.time_to_debug);
	if (simulation_stopped(coder->sim))
		return (1);
	return (0);
}

int	refactor(t_coder *coder)
{
	log_coder(coder, "is refactoring");
	precise_sleep(coder->sim, coder->sim->config.time_to_refactor);
	if (simulation_stopped(coder->sim))
		return (1);
	return (0);
}
