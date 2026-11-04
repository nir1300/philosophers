/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:48:11 by marvin            #+#    #+#             */
/*   Updated: 2025/06/16 00:40:25 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_mutexes(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&data->output, NULL) != 0)
		return (exit_sim(data, "Mutex could not be created\n"));
	data->mutex_init++;
	if (pthread_mutex_init(&data->access, NULL) != 0)
		return (exit_sim(data, "Mutex could not be created\n"));
	data->mutex_init++;
	if (pthread_mutex_init(&data->alive, NULL) != 0)
		return (exit_sim(data, "Mutex could not be created\n"));
	data->mutex_init++;
	pthread_mutex_lock(&data->alive);
	while (i < data->input.nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			data->is_alive = 0;
			pthread_mutex_unlock(&data->alive);
			return (exit_sim(data, "Mutex could not be created\n"));
		}
		data->mutex_init++;
		i++;
	}
	return (0);
}

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->input.nb_philo)
	{
		if (pthread_create(&data->philos[i].thread_id, NULL, &philo_routine,
				(void *)&data->philos[i]) != 0)
		{
			data->is_alive = 0;
			pthread_mutex_unlock(&data->alive);
			return (exit_sim(data, "Philo thread could not be created\n"));
		}
		data->thread_init++;
		i++;
	}
	return (0);
}

int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->input.nb_philo && i < data->thread_init)
	{
		if (pthread_join(data->philos[i].thread_id, NULL) != 0)
			ft_putendl("Philo thread could not be joined\n");
		i++;
	}
	if (data->thread_init -1 == data->input.nb_philo && \
	pthread_join(data->monitor, NULL) != 0)
		ft_putendl("Monitoring thread could not be joined\n");
	return (0);
}

int	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	usleep(1000);
	if (data->mutex_init > 0 && pthread_mutex_destroy(&data->output) != 0)
		ft_putendl("Access mutex could not be destroyed\n");
	if (data->mutex_init > 1 && pthread_mutex_destroy(&data->access) != 0)
		ft_putendl("Output mutex could not be destroyed\n");
	if (data->mutex_init > 2 && pthread_mutex_destroy(&data->alive) != 0)
		ft_putendl("Alive mutex could not be destroyed\n");
	data->mutex_init -= 3;
	while (i < data->mutex_init)
	{
		if (pthread_mutex_destroy(&data->forks[i]) != 0)
			ft_putendl("Fork mutex could not be destroyed\n");
		i++;
	}
	return (0);
}
