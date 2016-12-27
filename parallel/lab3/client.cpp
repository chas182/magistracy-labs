#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

int main() {
    size_t message_len = 250;
    key_t shm_key = 182, sem_key = 1820;
    int shm_id;
    void *shm;

    if ((shm_id = shmget(shm_key, message_len, IPC_CREAT | 0666)) < 0) { perror("shmget"); return 1; }
    if ((shm = shmat(shm_id, NULL, 0)) == (void *) -1) { perror("shmat"); return 1; }

    int sem_id = semget(sem_key, 2, 0666);
    if (sem_id == -1) { perror("semget"); return 1; }

    struct sembuf sem[2];
    while (1) {
        // check that message is empty
        sem[0].sem_num = 0;
        sem[0].sem_op = 0;
        sem[0].sem_flg = 0;
        // and get semaphore
        sem[1].sem_num = 0;
        sem[1].sem_op = 1;
        sem[1].sem_flg = 0;

        printf("Trying to reach semaphore...\n");
        if (semop(sem_id, sem, 2) == -1) { perror("semop"); exit(1); }

        printf("Enter message: \n");
        fgets((char *) shm, message_len, stdin);
        ((char *) shm)[strlen((char *) shm) - 1] = '\0';

        sem[0].sem_num = 1;
        sem[0].sem_op = 1;
        sem[0].sem_flg = 0;

        if (semop(sem_id, sem, 1) == -1) { perror("semop"); exit(1); }
    }

    if (shmdt(shm) == -1) { perror("shmdt"); exit(1); }
    return 0;
}