#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

double func(double x) {
    return pow(sin(x), 2);
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

    int steps;
    double step = (b - a) / d_count;
    double left_boundary;

    int processUID = 0;
    pid_t pid;
    pid_t* children = (pid_t *) malloc(sizeof(pid_t) * p_count);

    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe creation failed!");
        return 1;
    }
    for (int i = 0; i < p_count; ++i) {
        processUID = i + 1;
        left_boundary = a + i * d_count / p_count * step;
        steps = d_count / p_count;

        if ((pid = fork()) == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) break;
        children[i] = pid;
    }

    if (pid == 0) {
        printf("Proc[%d] start from %.3f, count %d\n", processUID, left_boundary, steps);

        double sum = 0.;
        // left rectangle method
        for (double i = 0; i < steps; ++i) {
            sum += step * func(left_boundary + i * step);
        }
        write(fd[1], &sum, sizeof(sum));
    } else {
        int exitCode = 0;
        int status;
        for (size_t i = 0; i < p_count; ++i) {
            if (waitpid(children[i], &status, 0) == -1) {
                perror("waitpid");
                return 1;
            }
            if (WIFEXITED(status))
            if (WEXITSTATUS(status) != 0)
                exitCode = 1;
        }

        if (exitCode)
            fprintf(stderr, "Some of child processes failed");
        else {
            double result = 0.;
            double sum;
            close(fd[1]);
            for (size_t i = 0; i < p_count; ++i) {
                read(fd[0], &sum, sizeof(double));
                result += sum;
            }
            close(fd[0]);
            printf("Result : %.5f\n", result);
        }

        free(children);

        return exitCode;
    }

    return 0;
}