#include "codexion.h"

static int  coder_burned_out(t_coder *coder)
{
    long now;

    now = get_time_ms();
    if (now == -1)
        return (1);
    if  (now - coder->last_compile_start >= coder->sim->config.time_to_burnout)
        return (1);
    return (0);
}

void    *monitor_routine(void *arg)
{
    t_sim   *sim;
    int     i;

    sim = (t_sim *) arg;
    while (!simulation_stopped(sim))
    {
        i = 0;
        while (i < sim->config.number_of_coders)
        {
            if (coder_burned_out(&sim->coders[i]))
            {
                log_coder(&sim->coders[i], "burned out");
                stop_simulation(sim);
                return (NULL);
            }
            i++;
        }
        usleep(500);
    }
    return (NULL);
}
