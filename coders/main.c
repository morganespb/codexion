#include "codexion.h"

static void print_sim(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        printf("Coder %d: left=%d, right=%d, compiles=%d\n", sim->coders[i].id, sim->coders[i].left_dongle, 
int main(int ac, char **av)
{
    t_config config;

    if (parsing(ac, av, &config) != 0)
        return (1);

        return (0);
}
