#include "codexion.h"

//used by the monitor
long    get_last_compile_start(t_coder *coder)
{
    long    value;
    pthread_mutex_lock(&coder->data_mutex);
    value = coder->last_compile_start;
    pthread_mutex_unlock(&coder->data_mutex);
    return (value);
}

//used by coder thread
void    set_last_compile_start(t_coder *coder, long value)
{
    pthread_mutex_lock(&coder->data_mutex);
    coder->last_compile_start = value;
    pthread_mutex_unlock(&coder->data_mutex);
}

//used by monitor
void    increment_compile_count(t_coder *coder)
{
    pthread_mutex_lock(&coder->data_mutex);
    coder->compile_count++;
    pthread_mutex_unlock(&coder->data_mutex);
}

//used by monitor
int     get_compile_count(t_coder *coder)
{
    int count;
    pthread_mutex_lock(&coder->data_mutex);
    count = coder->compile_count;
    pthread_mutex_unlock(&coder->data_mutex);
    return (count);
}

int	coders_done(t_sim *sim)
{
	int i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		if (get_compile_count(&sim->coders[i]) < sim->config.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}


