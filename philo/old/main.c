/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:14 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/03/03 18:02:34 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


int meals_finished(t_philos *philo)
{
    int i = 0;
    pthread_mutex_lock(&philo->data->output);  // Lock before accessing shared data
    while (i < philo->data->input.nb_philo)
    {
        if (philo->data->philos[i].meals_eaten <= philo->data->input.nb_meals)
        {
            pthread_mutex_unlock(&philo->data->output);  // Unlock after reading
            return (0);
        }
        i++;
    }
    pthread_mutex_unlock(&philo->data->output);  // Unlock after reading
    return (1);
}


void take_fork(t_philos *new)
{
	pthread_mutex_lock(&new->left_fork);
	print_states(new, 1);
}

void eat(t_philos *new)
{
	print_states(new, 2);
	pthread_mutex_lock(&new->data->output);
	new->meals_eaten++;
	pthread_mutex_unlock(&new->data->output);
	usleep(new->input.time_to_eat * 1000);
}

void sleepy(t_philos *new)
{
	usleep(300);
	print_states(new, 3);
	usleep(new->input.time_to_sleep * 1000);
	// set time since last eaten?
}

void	eat_sleep(t_philos *new, int even, int odd)
{
	int number;
	while (1)
	{
		pthread_mutex_lock(&new->output);
		if (meals_finished(new))
		{
			pthread_mutex_unlock(&new->output);
			break;
		}
		pthread_mutex_unlock(&new->output);
		pthread_mutex_lock(&new->left_fork);
		pthread_mutex_unlock(&new->left_fork);
		if (odd)
			take_fork(new);
		else
		{
			pthread_mutex_lock(&new->output);
			number = new->number;
			if (number == new->data->input.nb_philo)
				number = 0;
			pthread_mutex_lock(&new->data->philos[number].left_fork);
			pthread_mutex_unlock(&new->output);
	
		}
		if (odd)
			eat(new);
		pthread_mutex_unlock(&new->left_fork);
		sleepy(new);
		if (even)
			take_fork(new);
		else
			pthread_mutex_lock(&new->left_fork);
		if (even)
			eat(new);
		pthread_mutex_unlock(&new->left_fork);
	}
}

// somehow only works without valgrind


void	*even_routine(void *data)
{
	t_philos *new;
    
	new = (t_philos *)data;
	pthread_mutex_lock(&new->data->output);
	pthread_mutex_unlock(&new->data->output);
	if (new->number % 2 != 0)
		eat_sleep(new, 0, 1);
	else
		eat_sleep(new, 1, 0);
	return (NULL);
}


void    *odd_routine(void *data)
{
    (void)data;
	printf("hello\n");
	return (NULL);
}

void	create_threads(t_data *data)
{
	int	error;
	int	i;

	i = 0;
	data->threads_finished = 0;
	
	while (i < data->input.nb_philo)
	{
		data->philos[i].i = i;
		pthread_mutex_init(&data->philos[i].left_fork, NULL);

		if (data->input.nb_philo % 2 == 0)
			error = pthread_create(&data->philos[i].thread_id, NULL, \
            &even_routine, (void *)&data->philos[i]);
		else
			error = pthread_create(&data->philos[i].thread_id, NULL, \
            &odd_routine, (void *)data);
		if (error != 0)
			error_exit("Thread could not be created", error);
		pthread_mutex_lock(&data->output);
		data->philos[i].number = i + 1;
		data->philos[i].state = 0;
		data->philos[i].meals_eaten = 0;
		data->philos[i].locked = 1;
		data->philos[i].start_time = data->start_time;
		pthread_mutex_unlock(&data->output);
		i++;
	}
	data->threads_finished = 1;
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
	pthread_mutex_init(&data.output, NULL);
	error_handling(argc, argv);
	initialize(argc, argv, &data);
	create_threads(&data);
	
	join_threads(&data);
	destroy_mutexes(&data);
	if (!data.alive)
		exit(0);
	free_data(&data);
	pthread_mutex_lock(&data.output);
	printf("\nafter threads: %lu\n", get_time(data.start_time));
	pthread_mutex_unlock(&data.output);
	
}
