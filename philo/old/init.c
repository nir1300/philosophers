/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:05:39 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/02/27 21:50:50 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_exit(char *error_msg, int errnum)
{
    printf("%s\n", error_msg);
    exit(errnum);
}

void	free_data(t_data *data)
{
	free(data->philos);
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
	data->alive = 1;
	data->philos = ft_calloc(data->input.nb_philo, sizeof(t_philos));
	if (!data->philos)
		error_exit("Allocation failed", 1);
	init_philos(argv, data);
}

void	init_philos(char *argv[], t_data *data)
{
	int i = 0;
	while (i < data->input.nb_philo)
	{
		data->philos[i].input.nb_philo = ft_atoi(argv[1]);
		data->philos[i].input.time_to_die = ft_atoi(argv[2]);
		data->philos[i].input.time_to_eat = ft_atoi(argv[3]);
		data->philos[i].input.time_to_sleep = ft_atoi(argv[4]);
		data->philos[i].data = data;
		i++;
	}
}


void	join_threads(t_data *data)
{
	int	i;
	int	error;

	i = 0;
	while (i < data->input.nb_philo)
	{
		error = pthread_join(data->philos[i].thread_id, NULL);
		if (error != 0)
			printf("Thread of philosopher number %i could not be joined", i);
		i++;
	}

}

void	destroy_mutexes(t_data *data)
{
	int	i;
	int	error;

	i = 0;
	while (i < data->input.nb_philo)
	{
		if (!data->philos[i].locked)
		{
			error = pthread_mutex_destroy(&data->philos[i].left_fork);
			if (error != 0)
				printf("Left fork mutex of philosopher number \
				%i could not be destroyed", i);
		}
		i++;
	}
}