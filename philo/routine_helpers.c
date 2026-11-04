/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 21:36:05 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/06/15 22:30:23 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo(t_data *data, t_philos *philo)
{
	pthread_mutex_lock(&data->forks[0]);
	print_states(philo, data, 1);
	pthread_mutex_unlock(&data->forks[0]);
	ft_usleep(data->input.time_to_die);
}

void	stagger_start(t_data *data, t_philos *philo)
{
	usleep(50 * ((data->input.nb_philo - philo->number + 1) / 2));
	if (philo->number % 2 != 0 && data->input.nb_philo % 2 == 0)
		ft_usleep((data->input.time_to_eat));
}
