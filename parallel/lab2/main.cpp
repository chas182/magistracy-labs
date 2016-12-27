#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct ThreadInfo {
    int pUID;
    double left_boundary;
    int steps;
    double step;
};

pthread_mutex_t mutex;
double result;

double func(double x) {
    return pow(sin(x), 2);
}

void *thread_main(void *arguments) {
    ThreadInfo *args = (ThreadInfo *)arguments;

    printf("Thread[%d] start from %.3f, count %d\n", args->pUID, args->left_boundary, args->steps);

    double sum = 0.;
    for (double i = 0; i < args->steps; ++i)
        sum += args->step * func(args->left_boundary + i * args->step);

    pthread_mutex_lock (&mutex);
    result += sum;
    pthread_mutex_unlock (&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Argument for process count is required!");
        return 1;
    }

    double a = 0;
    double b = 1;

    int p_count = atoi(argv[1]);
    int d_count = atoi(argv[2]);

    double step = (b - a) / d_count;

    pthread_t *threads = (pthread_t *) malloc(p_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    int rc;
    for (int i = 0; i < p_count; i++) {
        ThreadInfo *curArgs = (ThreadInfo *)malloc(sizeof(ThreadInfo));
        curArgs->pUID = i + 1;
        curArgs->left_boundary = a + i * d_count / p_count * step;
        curArgs->steps = d_count / p_count;
        curArgs->step = step;

        rc = pthread_create(&threads[i], NULL, thread_main, (void *) curArgs);
        if (rc) {
            fprintf(stderr, "ERR: pthread_create() returns %d\n", rc);
            exit(1);
        }
    }

    for (int i = 0; i < p_count; ++i) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            fprintf(stderr, "ERR: pthread_join() returns %d\n", rc);
            exit(1);
        }
    }

    printf("Result : %.5f\n", result);

    pthread_mutex_destroy(&mutex);
    free(threads);
    pthread_exit(NULL);

    return 0;
}