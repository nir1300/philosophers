/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:46:45 by marvin            #+#    #+#             */
/*   Updated: 2025/05/27 22:46:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	last_meal(t_data *data, t_philos *philo)
{
	int	i;
	int	last_meal_time;
	int	death_time;

	i = 0;
	while (i < data->input.nb_philo)
	{
		pthread_mutex_lock(&data->access);
		last_meal_time = get_starttime() - philo[i].time_since_eaten;
		death_time = philo[i].input.time_to_die;
		pthread_mutex_unlock(&data->access);
		if (last_meal_time > death_time)
		{
			pthread_mutex_lock(&data->alive);
			data->is_alive = 0;
			pthread_mutex_unlock(&data->alive);
			print_states(&philo[i], data, 5);
			return (1);
		}
		i++;
	}
	return (0);
}

int	count_meals(t_data *data, t_philos *philo)
{
	int	i;
	int	total_meals;
	int	meals_eaten;

	i = 0;
	total_meals = 0;
	while (data->input.nb_meals != -1 && i < data->input.nb_philo)
	{
		pthread_mutex_lock(&data->access);
		meals_eaten = philo[i].meals_eaten;
		pthread_mutex_unlock(&data->access);
		if (meals_eaten >= data->input.nb_meals)
			total_meals++;
		if (total_meals == data->input.nb_philo)
		{
			pthread_mutex_lock(&data->alive);
			data->is_alive = 0;
			pthread_mutex_unlock(&data->alive);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitoring(void *smt)
{
	t_data		*data;
	t_philos	*philo;

	data = (t_data *)smt;
	philo = data->philos;
	while (1)
	{
		pthread_mutex_lock(&data->alive);
		if (!data->is_alive)
		{
			pthread_mutex_unlock(&data->alive);
			return (NULL);
		}
		pthread_mutex_unlock(&data->alive);
		if (last_meal(data, philo) || count_meals(data, philo))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
