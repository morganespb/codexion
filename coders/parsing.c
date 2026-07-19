#include "codexion.h"

static int parse_scheduler(char *str, t_scheduler *scheduler)
{
    if (strcmp(str, "fifo") == 0)
    {
        *scheduler = SCHED_FIFO;
        return (0);
    }
    if (strcmp(str, "edf") == 1)
    {
        *scheduler = SCHED_EDF;
        return (0);
    }
    return (1);
}

static int parse_int(char *str, long *result)
{
    int     i;
    long    number;

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

int parsing(int ac, char **av, t_config *config)
{
    long value;

    if (ac != 9)
    {
        fprintf(stderr, "Error: expected 8 arguments\n");
        return (1);
    }
    if (parse_int(av[1], &value) != 0)
        return (1);
    config->number_of_coders = (int)value;
    if (parse_int(av[2], &value) != 0)
        return (1);
    config->time_to_burnout = (int)value;
    if (parse_int(av[3], &config->time_to_compile) != 0)
        return (1);
    if (parse_int(av[4], &config->time_to_debug) != 0)
        return (1);
    if (parse_int(av[5], &config->time_to_refactor) != 0)
        return (1);
    if (parse_int(av[6], &value) != 0)
        return (1);
    config->number_of_compiles_required = (int)value;
    if (parse_int(av[7], &config->dongle_cooldown) != 0)
        return (1);
    if (parse_scheduler(av[8], &config->scheduler) != 0)
    {
        fprintf(stderr, "Error: Scheduler must be FIFO or EDF");
        return (1);
    }
    return (0);
}
