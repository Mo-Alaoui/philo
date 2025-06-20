#include "philo.h"

long ft_atol(const char *str)
{
	long num = 0;
	int sign = 1;
	int i = 0;

	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num * sign);
}

size_t get_current_time(void)
{
	t_timeval time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void ft_usleep(size_t mls)
{
	size_t start = get_current_time();
	while (get_current_time() - start < mls)
		usleep(500);
}

void destroy_all(t_engine *engine, int count)
{
	while (--count >= 0)
		pthread_mutex_destroy(&engine->forks[count]);
	pthread_mutex_destroy(&engine->write_lock);
	pthread_mutex_destroy(&engine->meal_lock);
}

void print_status(t_philo *philo, char *status)
{
	size_t time;

	pthread_mutex_lock(philo->mutexes.write_lock);
	time = get_current_time() - philo->times.born_time;
	printf("%ld %d %s\n", time, philo->id, status);
	pthread_mutex_unlock(philo->mutexes.write_lock);
}
