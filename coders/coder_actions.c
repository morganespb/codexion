#include "codexion.h"

int compile(t_coder *coder)
{
    long    now;

    now = get_time_ms();
    take_dongles(coder);
    if (now == -1)
    {
        release_dongles(coder);
        return (1);
    }
    set_last_compile_start(coder, now);
    log_coder(coder, "is compiling");
    precise_sleep(coder->sim, coder->sim->config.time_to_compile);
    if (!simulation_stopped(coder->sim))
    {
        increment_compile_count(coder);
        precise_sleep(coder->sim, coder->sim->config.dongle_cooldown);
    }
    release_dongles(coder);
    if (simulation_stopped(coder->sim))
        return (1);
    return (0);
}

int debug(t_coder *coder)
{
    log_coder(coder, "is debugging");
    precise_sleep(coder->sim, coder->sim->config.time_to_debug);
    if (simulation_stopped(coder->sim))
        return (1);
    return (0);
}

int refactor(t_coder *coder)
{
    log_coder(coder, "is refactoring");
    precise_sleep(coder->sim, coder->sim->config.time_to_refactor);
    if (simulation_stopped(coder->sim))
        return (1);
    return (0);
}

