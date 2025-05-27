/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:41:50 by marvin            #+#    #+#             */
/*   Updated: 2025/05/27 22:41:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(long start_time)
{
	struct timeval	tv;
	int				error;

	error = gettimeofday(&tv, NULL);
	if (error == -1)
		return (-1);
	return (((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - start_time);
}

long	get_starttime(void)
{
	struct timeval	tv;
	int				error;

	error = gettimeofday(&tv, NULL);
	if (error == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long duration)
{
	long	start;

	start = get_starttime();
	while (get_starttime() - start < duration)
		usleep(duration / 10);
}
