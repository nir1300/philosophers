/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:34:14 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/05/27 22:49:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_states(t_philos *philos, t_data *data, int state)
{
	pthread_mutex_lock(&data->output);
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
	if (state == 5)
		printf("%lu %i died\n", get_time(data->start_time), philos->number);
	pthread_mutex_unlock(&data->output);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	data.start_time = get_starttime();
	error_handling(argc, argv);
	initialize(argc, argv, &data);
	create_threads(&data);
	pthread_create(&data.monitor, NULL, &monitoring, (void *)&data);
	exit_sim(&data);
}
