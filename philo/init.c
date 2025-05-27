/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:44:10 by marvin            #+#    #+#             */
/*   Updated: 2025/05/27 22:44:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_handling(int argc, char *argv[])
{
	if (argc < 5)
		error_exit("Argument number invalid", 1);
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0 || ft_atoi(argv[3]) <= 0
		|| ft_atoi(argv[4]) <= 0 || (argc == 6 && ft_atoi(argv[5]) <= 0))
		error_exit("Input variables must be positive", 1);
	if (ft_atoi(argv[1]) == 1)
		(printf("0 1 died"), exit(1));
}

void	initialize(int argc, char *argv[], t_data *data)
{
	data->input.nb_philo = ft_atoi(argv[1]);
	data->input.time_to_die = ft_atoi(argv[2]);
	data->input.time_to_eat = ft_atoi(argv[3]);
	data->input.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->input.nb_meals = ft_atoi(argv[5]);
	else
		data->input.nb_meals = -1;
	data->is_alive = 1;
	data->philos = ft_calloc(data->input.nb_philo, sizeof(t_philos));
	data->forks = malloc(data->input.nb_philo * sizeof(pthread_mutex_t));
	if (!data->philos || !data->forks)
		error_exit("Allocation failed", 1);
	init_philos(argv, data);
	pthread_mutex_init(&data->output, NULL);
	pthread_mutex_init(&data->access, NULL);
	pthread_mutex_init(&data->alive, NULL);
}

void	init_philos(char *argv[], t_data *data)
{
	int	i;

	i = 0;
	while (i < data->input.nb_philo)
	{
		data->philos[i].input.nb_philo = ft_atoi(argv[1]);
		data->philos[i].input.time_to_die = ft_atoi(argv[2]);
		data->philos[i].input.time_to_eat = ft_atoi(argv[3]);
		data->philos[i].input.time_to_sleep = ft_atoi(argv[4]);
		data->philos[i].data = data;
		data->philos[i].number = i + 1;
		data->philos[i].state = 0;
		data->philos[i].i = i;
		data->philos[i].meals_eaten = 0;
		data->philos[i].time_since_eaten = get_starttime();
		i++;
	}
}
