/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:06:29 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/02/20 16:13:43 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(long start_time)
{
	struct timeval	tv;
	int				error;

	error = gettimeofday(&tv, NULL);
	if (error == -1)
		return (-1);
	return (((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - start_time);
}

long	get_starttime(void)
{
	struct timeval	tv;
	int				error;

	error = gettimeofday(&tv, NULL);
	if (error == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_states(t_data *data, int state, int philo_id)
{
	if (state == 1)
		printf("%lu %i has taken a fork\n", get_time(data->start_time), \
		philo_id);
	if (state == 2)
		printf("%lu %i is eating\n", get_time(data->start_time), philo_id);
	if (state == 3)
		printf("%lu %i is sleeping\n", get_time(data->start_time), philo_id);
	if (state == 4)
		printf("%lu %i is thinking\n", get_time(data->start_time), philo_id);
	if (state == 5)
		printf("%lu %i died\n", get_time(data->start_time), philo_id);
}
