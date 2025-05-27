/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:45:45 by marvin            #+#    #+#             */
/*   Updated: 2025/05/27 22:45:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_data(t_data *data)
{
	free(data->philos);
	free(data->forks);
}

void	exit_sim(t_data *data)
{
	join_threads(data);
	destroy_mutexes(data);
	free_data(data);
	exit(0);
}

void	error_exit(char *error_msg, int errnum)
{
	printf("%s\n", error_msg);
	exit(errnum);
}
