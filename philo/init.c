/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrumpfhu <nrumpfhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 16:05:39 by nrumpfhu          #+#    #+#             */
/*   Updated: 2025/02/20 17:34:47 by nrumpfhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_exit(char *error_msg, int errnum)
{
    printf("%s\n", error_msg);
    exit(errnum);
}

void	error_handling(int argc, char *argv[])
{
	if (argc < 5)
		error_exit("Argument number invalid", 1);
	if (argv[1] <= 0 || argv[2] <= 0 || argv[3] <= 0 || argv[4] <= 0 \
	|| (argc == 6 && argv[5] <= 0))
		error_exit("Input variables must be positive", 1);
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
}
