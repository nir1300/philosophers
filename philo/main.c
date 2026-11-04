/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:14 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/06/16 00:30:38 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_states(t_philos *philos, t_data *data, int state)
{
	pthread_mutex_lock(&data->output);
	pthread_mutex_lock(&data->alive);
	if (data->is_alive)
	{
		if (state == 1)
			printf("%lu %i has taken a fork\n", get_time(data->start_time),
				philos->number);
		if (state == 2)
			printf("%lu %i is eating\n", get_time(data->start_time),
				philos->number);
		if (state == 3)
			printf("%lu %i is sleeping\n", get_time(data->start_time),
				philos->number);
		if (state == 4)
			printf("%lu %i is thinking\n", get_time(data->start_time),
				philos->number);
	}
	pthread_mutex_unlock(&data->alive);
	if (state == 5)
		printf("%lu %i died\n", get_time(data->start_time), philos->number);
	pthread_mutex_unlock(&data->output);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	data.start_time = get_starttime();
	if (error_handling(argc, argv))
		return (1);
	if (initialize(argc, argv, &data))
		return (1);
	if (create_mutexes(&data))
		return (1);
	if (create_threads(&data))
		return (1);
	if (pthread_create(&data.monitor, NULL, &monitoring, (void *)&data) == 0)
		data.thread_init++;
	else
	{
		data.is_alive = 0;
		pthread_mutex_unlock(&(&data)->alive);
		return (exit_sim(&data, "Monitoring thread could not be created\n"));
	}
	pthread_mutex_unlock(&(&data)->alive);
	exit_sim(&data, NULL);
}
