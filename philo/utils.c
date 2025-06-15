#include "philo.h"

size_t ft_strlen(const char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

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
		error_exit("Time error\n", 1);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void ft_usleep(size_t mls)
{
	size_t start = get_current_time();
	while (get_current_time() - start < mls)
		usleep(500);
}

void error_exit(char *message, int code)
{
	if (message)
		write(2, message, ft_strlen(message));
	exit(code);
}