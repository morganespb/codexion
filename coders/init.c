#include "codexion.h"

static  void init_dongles(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        sim->dongles[i].id = i;
        i++;
    }
}

static void init_coders(t_sim *sim)
{
    int i;
    int count;

    i = 0;
    count = sim->config.number_of_coders;
    while (i < count)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].left_dongle = i;
        sim->coders[i].right_dongle = (i + 1) % count;
        sim->coders[i].compile_count = 0;
        i++;
    }
}

int init_simulation(t_sim *sim, t_config config)
{
    sim->config = config;
    sim->coders = malloc(sizeof(t_coder) * config.number_of_coders);
    if (!sim->coders)
        return (1);
    sim->dongles = malloc(sizeof(t_dongle) * config.number_of_coders);
    if (!sim->dongles)
        return (1);
    init_dongles(sim);
    init_coders(sim);
    return (0);
}

