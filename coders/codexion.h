#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef enum e_scheduler
{
    SCHED_FIFO,
    SCHED_EDF
}   t_scheduler;

typedef struct s_config
{
    int             number_of_coders;
    int             time_to_burnout;
    long            time_to_compile;
    long            time_to_debug;
    long            time_to_refactor;
    int             number_of_compiles_required;
    long            dongle_cooldown;
    t_scheduler     scheduler;
}   t_config;

typedef struct s_coder
{
    int     id;
    int     left_dongle;
    int     right_dongle;
    int     compile_count;
}   t_coder;

typedef struct s_dongle
{
    int     id;
}   t_dongle;

typedef struct s_sim
{
    t_config    config;
    t_coder     *coder;
    t_dongle    *dongle;
}   t_sim;

int parsing(int ac, char **av, t_config *config);

#endif

