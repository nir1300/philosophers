/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:14 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/02/20 18:27:33 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*even_routine(void *data)
{
	t_data *new;
    
	new = (t_data *)data;
    int i = new->i;
	while (new->threads_finished)
		;
	printf("hello, philo nb %i\n", i);

    // if (new->philos[i].locked)
    // {
    //     pthread_mutex_unlock(&new->philos[i].left_fork);
    // }
	// printf("hello, philo nb %i\n", i);
	return (NULL);
}

void    *odd_routine(void *data)
{
    (void)data;
	printf("hello\n");
	return (NULL);
}

void    create_output_thread(t_data *data)
{
    //int error;
    pthread_mutex_init(&data->output, NULL);
    // error = pthread_mutex_lock(&data->output);
    // if (error != 0)
    //     error_exit("Output mutex could not be locked", error);
    // if (data->input.nb_philo % 2 == 0)
    //     error = pthread_create(&data->output_id, NULL, \
    //     &even_routine, (void *)&data);
    // else
    //     error = pthread_create(&data->output_id, NULL, \
    //     &odd_routine, (void *)&data);
    // if (error != 0)
    //     error_exit("Output thread could not be created", error);
}

void	create_threads(t_data *data)
{
	int	error;
	int	i;

	i = 0;
	data->threads_finished = 0;
	while (i < data->input.nb_philo)
	{
        data->i = i;
		pthread_mutex_init(&data->philos[i].left_fork, NULL);
        error = pthread_mutex_lock(&data->philos[i].left_fork);
        if (error != 0)
			error_exit("Fork mutex could not be locked", error);
		if (data->input.nb_philo % 2 == 0)
			error = pthread_create(&data->philos[i].thread_id, NULL, \
            &even_routine, (void *)&data);
		else
			error = pthread_create(&data->philos[i].thread_id, NULL, \
            &odd_routine, (void *)&data);
		if (i == data->input.nb_philo - 1)
			data->threads_finished = 1;
		if (error != 0)
			error_exit("Thread could not be created", error);
		data->philos[i].number = i;
		data->philos[i].state = 0;
		data->philos[i].meals_eaten = 0;
		data->philos[i].locked = 1;
		i++;
	}
}

void	join_threads(t_data *data)
{
	int	i;
	int	error;

	i = 0;
	while (i < data->input.nb_philo)
	{
		error = pthread_join(data->philos[i].thread_id, NULL);
		if (error != 0)
			printf("Thread of philosopher number %i could not be joined", i);
		i++;
	}
    // error = pthread_join(data->output_id, NULL);
    // if (error != 0)
    //     printf("Output thread could not be joined");
}

void	destroy_mutexes(t_data *data)
{
	int	i;
	int	error;

	i = 0;
	while (i < data->input.nb_philo)
	{
		if (!data->philos[i].locked)
		{
			error = pthread_mutex_destroy(&data->philos[i].left_fork);
			if (error != 0)
				printf("Left fork mutex of philosopher number \
				%i could not be destroyed", i);
		}
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_data	data;

	data.start_time = get_starttime();
	printf("time: %lu\n", get_time(data.start_time));
	usleep(2000);
	error_handling(argc, argv);
	initialize(argc, argv, &data);
    //create_output_thread(&data);
	create_threads(&data);
	// join_threads(&data);
	// destroy_mutexes(&data);
	// if (!data.alive)
	// 	exit(0);
	// free_data(&data);
	// printf("\nafter threads: %lu\n", get_time(data.start_time));
}
