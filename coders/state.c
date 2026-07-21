#include "codexion.h"

int simulation_stopped(t_sim *sim)
{
    int stopped;

    pthread_mutex_lock(&sim->state_mutex);
    stopped = sim->stopped;
    pthread_mutex_unlock(&sim->state_mutex);
    return (stopped);
}

void stop_simulation(t_sim *sim)
{
    pthread_mutex_lock(&sim->state_mutex);
    sim->stopped = 1;
    pthread_mutex_unlock(&sim->state_mutex);
}

