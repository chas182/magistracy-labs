#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t message_len = 250;
    key_t shm_key = 182, sem_key = 1820;
    int shm_id;
    void *shm;

    if ((shm_id = shmget(shm_key, message_len, IPC_CREAT | 0666)) < 0) { perror("shmget"); return 1; }
    if ((shm = shmat(shm_id, NULL, 0)) == (void *) -1) { perror("shmat"); return 1; }

    int sem_id = semget(sem_key, 2, 0666 | IPC_CREAT);
    if (sem_id == -1) { perror("semget"); return 1; }

    int init_args[2] = {0, 0};
    if (semctl(sem_id, 0, SETALL, init_args) == -1) { perror("semctl"); exit(1); }

    struct sembuf sem[1];
    while (1) {
        sem[0].sem_num = 1;
        sem[0].sem_op = -1;
        sem[0].sem_flg = 0;

        printf("Waiting for messages...\n");
        if (semop(sem_id, sem, 1) == -1) { perror("semop"); return 1; }

        printf("Received:\n%s\n", (char *) shm);

        sem[0].sem_num = 0;
        sem[0].sem_op = -1;
        sem[0].sem_flg = 0;

        if (semop(sem_id, sem, 1) == -1) { perror("semop"); return 1; }
    }

    if (semctl(sem_id, 0, IPC_RMID) == -1) { perror("semctl");return 1; }
    if (shmdt(shm) == -1) { perror("shmdt"); return 1; }
    if (shmctl(shm_id, IPC_RMID, NULL)) { perror("shmctl"); return 1; }

    return 0;
}