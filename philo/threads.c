/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:48:11 by marvin            #+#    #+#             */
/*   Updated: 2025/05/27 22:48:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_threads(t_data *data)
{
	int	error;
	int	i;
	int	nb_philos;

	i = 0;
	nb_philos = data->input.nb_philo;
	while (i < nb_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < data->input.nb_philo)
	{
		error = pthread_create(&data->philos[i].thread_id, NULL, &philo_routine,
				(void *)&data->philos[i]);
		if (error != 0)
			error_exit("Thread could not be created", error);
		i++;
	}
}

void	join_threads(t_data *data)
{
	int	i;
	int	error;
	int	nb_philos;

	nb_philos = data->input.nb_philo;
	i = 0;
	while (i < nb_philos)
	{
		error = pthread_join(data->philos[i].thread_id, NULL);
		if (error != 0)
			printf("Thread of philosopher number %i could not be joined", i);
		i++;
	}
	pthread_join(data->monitor, NULL);
}

void	destroy_mutexes(t_data *data)
{
	int			i;
	int			error;
	t_philos	*philo;

	philo = data->philos;
	i = 0;
	usleep(1000);
	while (i < data->input.nb_philo)
	{
		error = pthread_mutex_destroy(&data->forks[i]);
		if (error != 0)
			printf("Fork number %i could not be destroyed: error %d\n", i,
				error);
		i++;
	}
	(void)philo;
	pthread_mutex_destroy(&data->alive);
	if (error != 0)
		printf("Alive mutex could not be destroyed\n");
	pthread_mutex_destroy(&data->access);
	if (error != 0)
		printf("Access mutex could not be destroyed\n");
	error = pthread_mutex_destroy(&data->output);
	if (error != 0)
		printf("Output mutex could not be destroyed\n");
}
