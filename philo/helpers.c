/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:51:06 by marvin            #+#    #+#             */
/*   Updated: 2025/06/02 21:22:31 by nrumpfhu         ###   ########.fr       */
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

void	take_odd(t_data *data, t_philos *philo)
{
	usleep(1000 * (philo->number % 2));
	if (philo->number % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->i]);
		pthread_mutex_lock(&data->forks[(philo->i + 1)
			% data->input.nb_philo]);
	}
	else
	{
		usleep(1000 * (data->input.time_to_eat / 2));
		pthread_mutex_lock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
		pthread_mutex_lock(&data->forks[philo->i]);
	}
}

void	forks_even(t_data *data, t_philos *philo, int take)
{
	int	first;
	int	sec;
	int	temp;

	first = philo->i;
	sec = (philo->i + 1) % data->input.nb_philo;
	if (first > sec)
	{
		temp = first;
		first = sec;
		sec = temp;
	}
	if (take)
	{
		pthread_mutex_lock(&data->forks[first]);
		pthread_mutex_lock(&data->forks[sec]);
	}
	else
	{
		pthread_mutex_unlock(&data->forks[sec]);
		pthread_mutex_unlock(&data->forks[first]);
	}
}
