/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.h                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:01:08 by mosimon          #+#    #+#              */
/*   Updated: 2026/07/20 16:01:08 by mosimon         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_sim	t_sim;

typedef enum e_scheduler
{
	FIFO_ORDER,
	EDF_ORDER
}	t_scheduler;

typedef struct s_config
{
	int			number_of_coders;
	int			time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	int			number_of_compiles_required;
	long		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_coder
{
	int			id;
	int			left_dongle;
	int			right_dongle;
	int			compile_count;
    long        last_compile_start;
	pthread_t	thread;
	t_sim		*sim;
}	t_coder;

typedef struct s_dongle
{
	int	id;
}	t_dongle;

typedef struct s_sim
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	long			start_time;
    int             stopped;
    pthread_t       monitor;
	pthread_mutex_t	print_mutex;
    pthread_mutex_t state_mutex;
}	t_sim;

int		parsing(int ac, char **av, t_config *config);
int		init_simulation(t_sim *sim, t_config config);
int		start_threads(t_sim *sim);
int		join_threads(t_sim *sim);
long	get_time_ms(void);
void	precise_sleep(t_sim *sim, long duration_ms);
void	log_coder(t_coder *coder, char *message);
int     simulation_stopped(t_sim *sim);
void    stop_simulation(t_sim *sim);
void    *monitor_routine(void *arg);
void    init_coder_times(t_sim *sim);

#endif
