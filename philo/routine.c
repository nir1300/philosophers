/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:43:18 by marvin            #+#    #+#             */
/*   Updated: 2025/05/27 22:43:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_fork(t_philos *philo, t_data *data)
{
	if (!all_alive(data))
		return ;
	pthread_mutex_lock(&data->access);
	philo->state = 1;
	pthread_mutex_unlock(&data->access);
	usleep(1000 * (philo->number % 2));
	if (philo->number % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->i]);
		pthread_mutex_lock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
	}
	else
	{
		usleep(1000 * (data->input.time_to_eat / 2));
		pthread_mutex_lock(&data->forks[(philo->i + 1) % data->input.nb_philo]);
		pthread_mutex_lock(&data->forks[philo->i]);
	}
	if (!all_alive(data))
		return (put_down_fork(philo, data));
	print_states(philo, data, 1);
	print_states(philo, data, 1);
}

void	eat(t_philos *philo, t_data *data)
{
	if (!all_alive(data))
		return (put_down_fork(philo, data));
	pthread_mutex_lock(&data->access);
	philo->meals_eaten++;
	philo->state = 2;
	philo->time_since_eaten = get_starttime();
	pthread_mutex_unlock(&data->access);
	print_states(philo, data, 2);
	ft_usleep(philo->input.time_to_eat);
	pthread_mutex_lock(&data->access);
	philo->time_since_eaten = get_starttime();
	pthread_mutex_unlock(&data->access);
}

void	put_down_fork(t_philos *philo, t_data *data)
{
	if (unlock(philo, data))
	{
		if (philo->number % 2 == 0)
		{
			pthread_mutex_unlock(&data->forks[(philo->i + 1)
				% data->input.nb_philo]);
			pthread_mutex_unlock(&data->forks[philo->i]);
		}
		else if (philo->number % 2 != 0)
		{
			pthread_mutex_unlock(&data->forks[philo->i]);
			pthread_mutex_unlock(&data->forks[(philo->i + 1)
				% data->input.nb_philo]);
		}
	}
}

void	sleepy(t_philos *philo, t_data *data)
{
	int	nb_philo;

	nb_philo = data->input.nb_philo;
	if (!all_alive(data))
		return ;
	print_states(philo, data, 3);
	ft_usleep(philo->input.time_to_sleep);
	if (!all_alive(data))
		return ;
	if (nb_philo % 2 != 0)
		print_states(philo, data, 4);
}

void	*philo_routine(void *var)
{
	t_philos	*philo;
	t_data		*data;

	philo = (t_philos *)var;
	data = philo->data;
	if (data->input.nb_philo % 2 != 0)
		usleep(100);
	while (1)
	{
		if (!all_alive(data))
			return (NULL);
		if (data->input.nb_philo % 2 != 0 && philo->number % 2 != 0)
			usleep(800);
		take_fork(philo, data);
		eat(philo, data);
		put_down_fork(philo, data);
		sleepy(philo, data);
	}
	return (NULL);
}
