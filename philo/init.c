/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:44:10 by marvin            #+#    #+#             */
/*   Updated: 2025/06/16 02:02:48 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	int_overflow(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+')
		i++;
	while (arg && arg[i] && arg[i] == '0')
		i++;
	if (ft_strlen(arg + i) >= 10)
	{
		if (ft_strlen(arg + i) > 10 || (ft_strlen(arg + i) == 10 && \
		ft_strcmp(arg + i, "2147483647") > 0))
			return (ft_putendl("Input number too big (integer overflow)"), 1);
	}
	return (0);
}

int	check_nums(char **arg, int argc)
{
	int	n;
	int	i;

	n = 1;
	i = 0;
	while (n < argc)
	{
		i = 0;
		if (arg[n][i] == '+')
			i++;
		if (arg[n][i] == '-')
			return (ft_putendl("Input variables must be positive"), 1);
		while (arg[n][i])
		{
			if (!(arg[n][i] >= 48 && arg[n][i] <= 57))
				return (ft_putendl("Input variables must be numbers"), 1);
			i++;
		}
		if (int_overflow(arg[n]))
			return (1);
		n++;
	}
	return (0);
}

int	error_handling(int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
		return (ft_putendl("Argument number invalid"), 1);
	if (check_nums(argv, argc))
		return (1);
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0 || ft_atoi(argv[3]) <= 0
		|| ft_atoi(argv[4]) <= 0 || (argc == 6 && ft_atoi(argv[5]) <= 0))
		return (ft_putendl("Input variables must be positive integers1"), 1);
	return (0);
}

int	initialize(int argc, char *argv[], t_data *data)
{
	data->input.nb_philo = ft_atoi(argv[1]);
	data->input.time_to_die = ft_atoi(argv[2]);
	data->input.time_to_eat = ft_atoi(argv[3]);
	data->input.time_to_sleep = ft_atoi(argv[4]);
	data->mutex_init = 0;
	data->thread_init = 0;
	if (argc == 6)
		data->input.nb_meals = ft_atoi(argv[5]);
	else
		data->input.nb_meals = -1;
	data->is_alive = 1;
	data->philos = ft_calloc(data->input.nb_philo, sizeof(t_philos));
	if (!data->philos)
		return (ft_putendl("Allocation failed"), 1);
	data->forks = malloc(data->input.nb_philo * sizeof(pthread_mutex_t));
	if (!data->forks)
	{
		free(data->philos);
		return (ft_putendl("Allocation failed"), 1);
	}
	init_philos(argv, data);
	return (0);
}

void	init_philos(char *argv[], t_data *data)
{
	int	i;

	i = 0;
	while (i < data->input.nb_philo)
	{
		data->philos[i].time_since_eaten = get_starttime();
		data->philos[i].input.nb_philo = ft_atoi(argv[1]);
		data->philos[i].input.time_to_die = ft_atoi(argv[2]);
		data->philos[i].input.time_to_eat = ft_atoi(argv[3]);
		data->philos[i].input.time_to_sleep = ft_atoi(argv[4]);
		data->philos[i].data = data;
		data->philos[i].number = i + 1;
		data->philos[i].state = 0;
		data->philos[i].i = i;
		data->philos[i].meals_eaten = 0;
		i++;
	}
}
