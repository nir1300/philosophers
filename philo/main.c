/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:14 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/02/17 17:25:42 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time()
{
    struct timeval tv;
    int error = gettimeofday(&tv, NULL);
    if (error == -1)
        return (-1);
    return (tv.tv_sec * 1000L);
}

void    print_states(t_data *data, int state, int philo_id)
{
    (void)data;
    if (state == 1)
        printf("%lu %i has taken a fork\n", get_time(), philo_id);
    if (state == 2)
        printf("%lu %i is eating\n", get_time() , philo_id);
    if (state == 3)
        printf("%lu %i is sleeping\n", get_time(), philo_id);
    if (state == 4)
        printf("%lu %i is thinking\n", get_time(), philo_id);
    if (state == 5)
        printf("%lu %i died\n", get_time(), philo_id);
}

void    *routine()
{
    printf("hello");
    return NULL;
}

void    create_threads(t_data *data)
{
    int error;
    int i = 0;
    
    while (i < data->input.nb_philo)
    {
        pthread_mutex_init(&data->philos[i].left_fork, NULL);
        error = pthread_create(&data->philos[i].thread_id, NULL, &routine, NULL);
        if (error != 0)
            error_exit("Thread could not be created", error);
        data->philos[i].number = i;
        data->philos[i].state = 0;
        data->philos[i].meals_eaten = 0;
        data->philos[i].locked = 0;
        i++;
    }
}

void	error_exit(char *error_msg, int errnum)
{
	printf("%s\n", error_msg);
	exit(errnum);
}

void	error_handling(int argc, char *argv[])
{
	if (argc < 5)
		error_exit("Argument number invalid", 1);
	if (argv[1] <= 0 || argv[2] <= 0 || argv[3] <= 0 || argv[4] <= 0 || (argc == 6 && argv[5] <= 0))
		error_exit("Input variables must be positive", 1);
}

void    free_data(t_data *data)
{
    free(data->philos);
}

void	initialize(int argc, char *argv[], t_data *data)
{
	data->input.nb_philo = ft_atoi(argv[1]);
	data->input.time_to_die = ft_atoi(argv[2]);
	data->input.time_to_eat = ft_atoi(argv[3]);
	data->input.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->input.nb_meals = ft_atoi(argv[5]);
	else
		data->input.nb_meals = -1;
    data->alive = 1;
    // lock any mutexes here?
    data->philos = ft_calloc(data->input.nb_philo, sizeof(t_philos));
    if (!data->philos)
        error_exit("Allocation failed", 1);
}

void    join_threads(t_data *data)
{
    int i = 0;
    int error;
    while (i < data->input.nb_philo)
    {
        error = pthread_join(data->philos[i].thread_id, NULL);
        if (error != 0)
            printf("Thread of philosopher number %i could not be joined", i);
        i++;
    }
}

void    destroy_mutexes(t_data *data)
{
    //pthread_mutex_unlock();
    int i = 0;
    int error;
    while (i < data->input.nb_philo)
    {
        error = pthread_mutex_destroy(&data->philos[i].left_fork);
        if (error != 0)
            printf("Left fork mutex of philosopher number %i could not be destroyed", i);
        i++;
    }
}

int	main(int argc, char *argv[])
{
	t_data data;
    printf("time: %lu\n", get_time());
	error_handling(argc, argv);
	initialize(argc, argv, &data);
    
	create_threads(&data);
    join_threads(&data);
    destroy_mutexes(&data);
    if (!data.alive)
        exit(0);
    free_data(&data);
    get_time();

}
