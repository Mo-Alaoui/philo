#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#define PHILO_MAX_COUNT 200


typedef pthread_mutex_t t_mutex;
typedef struct timeval t_timeval;

typedef struct s_mutexes
{
	t_mutex *left_fork;
	t_mutex *right_fork;
	t_mutex *write_lock;
	t_mutex *meal_lock;
} t_mutexes;

typedef struct s_times
{
	size_t die;
	size_t eat;
	size_t sleep;
	size_t last_meal;
	size_t born_time;
} t_times;

typedef struct s_philo
{
	int philo_count;
	int meals_eaten;
	int must_eat;
	int id;
	pthread_t thread_id;
	t_mutexes mutexes;
	t_times times;
} t_philo;

typedef struct s_engine
{
	t_mutex *forks;
	t_philo *philos;
	t_mutex meal_lock;
	t_mutex write_lock;
} t_engine;

void destroy_all(t_engine *engine, int count);
void print_status(t_philo *philo, char *status);
int start_simulation(t_engine *engine, int count);
void *monitor(void *ptr);
void *start_routine(void *ptr);
void philo_routine(t_philo *philo);
int all_philos_eat(t_philo *philos);
size_t ft_strlen(const char *s);
long ft_atol(const char *str);
size_t get_current_time(void);
void ft_usleep(size_t mls);
//void error_exit(char *message, int code);
