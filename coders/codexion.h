/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.h                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: mosimon <morgane.simon@learner.42.tech>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/20 16:01:08 by mosimon          #+#    #+#              */
/*   Updated: 2026/10/06 14:26:41 by mosimon         ###   ########.fr        */
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

typedef enum e_scheduler_type
{
	FIFO_ORDER,
	EDF_ORDER
}	t_scheduler_type;

typedef struct s_config
{
	int					number_of_coders;
	int					time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	int					number_of_compiles_required;
	long				dongle_cooldown;
	t_scheduler_type	scheduler_type;
}	t_config;

typedef struct s_coder
{
	int				id;
	int				left_dongle;
	int				right_dongle;
	int				compile_count;
	long			last_compile_start;
	long			request_order;
	int				permission_granted;
	int				waiting;
	pthread_t		thread;
	pthread_mutex_t	data_mutex;
	t_sim			*sim;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	long			last_release;
	pthread_mutex_t	dongles_mutex;
}	t_dongle;

typedef struct s_scheduler
{
	t_coder			**heap;
	int				size;
	int				capacity;
	long			next_order;
	int				currently_taking_dongles;
	pthread_cond_t	condition;
	pthread_mutex_t	sched_mutex;
}	t_scheduler;

typedef struct s_sim
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	t_scheduler		scheduler;
	long			start_time;
	int				stopped;
	int				started;
	int				coders_initialized;
	int				dongles_initialized;
	int				scheduler_initialized;
	pthread_t		monitor;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
}	t_sim;

/* parsing */
int		parsing(int ac, char **av, t_config *config);

/* initialization */
int		init_simulation(t_sim *sim, t_config config);

/* threads */
int		start_threads(t_sim *sim);
int		join_threads(t_sim *sim);

/* shared data */
long	get_time_ms(void);
void	precise_sleep(t_sim *sim, long duration_ms);
void	log_coder(t_coder *coder, char *message);
void	log_burnout(t_coder *coder);
int		simulation_stopped(t_sim *sim);
int		stop_simulation(t_sim *sim);
int		wait_for_start(t_sim *sim);
int		begin_simulation(t_sim *sim);
void	*monitor_routine(void *arg);
long	get_last_compile_start(t_coder *coder);
void	set_last_compile_start(t_coder *coder, long value);
void	increment_compile_count(t_coder *coder);
int		get_compile_count(t_coder *coder);

/* actions */
int		compile(t_coder *coder);
int		debug(t_coder *coder);
int		refactor(t_coder *coder);
int		coders_done(t_sim *sim);

/* cleanup */
void	cleanup_sim(t_sim *sim);
void	destroy_dongles_mutexes(t_sim *sim, int count);
void	destroy_coders_mutexes(t_sim *sim, int count);
void	destroy_scheduler(t_sim *sim);
int		take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);

/* scheduler */
int		higher_priority(t_sim *sim, t_coder *first, t_coder *second);
void	heap_insert(t_sim *sim, t_coder *coder);
t_coder	*heap_pop(t_sim *sim);
int		request_permission(t_coder *coder);
void	notify_scheduler(t_sim *sim);

#endif
