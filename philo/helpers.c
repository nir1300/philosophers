/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:51:06 by marvin            #+#    #+#             */
/*   Updated: 2025/05/27 22:51:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_alive(t_data *data)
{
	int	alive;

	pthread_mutex_lock(&data->alive);
	alive = data->is_alive;
	pthread_mutex_unlock(&data->alive);
	if (!alive)
		return (0);
	return (1);
}

int	unlock(t_philos *philo, t_data *data)
{
	int	state;

	pthread_mutex_lock(&data->access);
	state = philo->state;
	if (state == 1 || state == 2)
		philo->state = 0;
	pthread_mutex_unlock(&data->access);
	return (state);
}
