//
// Created by Nikolay S on 10/29/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// setting max number of child processes
#define MAX_CHILDREN 100
#define BUFFER_SIZE 5
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 2

// supporting functions

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
sem_t mutex, empty, data;


int random_num() {
    return rand() % 100 + 1;
}

void* producer(void* p_id) {
    for (int i = 0; i < 20; i++) {
        int item = rand() % 100 + 1;;
        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = item;
        printf("Producer %ld produced %d. Buffer: ", (long) p_id, item);
        for (int j = 0; j < BUFFER_SIZE; j++) {
            printf("%d ", buffer[j]);
        }
        printf("\n");

        in = (in + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&data);
        usleep((random_num() % 400 + 100) * 1000);  // sleep for random time 100-500ms
    }
    return NULL;
}

void* consumer(void* c_id) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&data);
        sem_wait(&mutex);

        int item = buffer[out];
        buffer[out] = 0;  // Resetting the consumed slot
        printf("Consumer %ld consumed %d. Buffer: ", (long)c_id, item);
        for (int j = 0; j < BUFFER_SIZE; j++) {
            printf("%d ", buffer[j]);
        }
        printf("\n");

        out = (out + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&empty);
        usleep((random_num() % 400 + 100) * 1000);  // sleep for random time between 100ms and 500ms
    }
    return NULL;
}

void* thread_function(void* arg) {
    printf("Thread %ld started.\n", (long)arg);
    sleep(15);  // Let the thread sleep for 3 seconds
    printf("Thread %ld finished.\n", (long)arg);
    return NULL;
}

pid_t children[MAX_CHILDREN];
int current_count = 0;

typedef struct {
    unsigned long virt;
    unsigned long res;
    unsigned long shr;
} MemoryStats;

MemoryStats get_memory_stats(pid_t pid) {
    char filepath[256];
    char line[256];
    MemoryStats stats = {0, 0, 0};

    // getting virt and res mem from /proc FS
    snprintf(filepath, sizeof(filepath), "/proc/%d/status", pid);
    FILE *fp = fopen(filepath, "r");
    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "VmSize:", 7) == 0) {
                sscanf(line, "VmSize: %lu kB", &stats.virt);
            } else if (strncmp(line, "VmRSS:", 6) == 0) {
                sscanf(line, "VmRSS: %lu kB", &stats.res);
            }
        }
        fclose(fp);
    }

    // getting shr mem from /proc/${PID}/statm
    snprintf(filepath, sizeof(filepath), "/proc/%d/statm", pid);
    fp = fopen(filepath, "r");
    if (fp) {
        unsigned long total, shared;
        if (fscanf(fp, "%lu %lu", &total, &shared) == 2) {
            stats.shr = shared * 4;  // convert pages to kB
        }
        fclose(fp);
    }

    return stats;
}

// main menu functions
void proc_create() {
    printf("Process creation function called.\n");
    pid_t pid = fork();

    if (pid == 0) {
        // if 0 we are child
        printf("Child process %d running...\n", getpid());
        pause();  // put proc on pause to switch to parent
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        // otherwise we are parent
        children[current_count++] = pid;
    } else {
        // error handling
        perror("Error in fork");
        exit(EXIT_FAILURE);
    }
}

void proc_list() {
    printf("Process list function called.\n");
    for (int i = 0; i < current_count; i++) {
        printf("%d. PID: %d\n", i+1, children[i]);
    }
}

void proc_monit(int index) {
    printf("Process monitor function called.\n");
    // children[index-1]
    if (index < 1 || index > current_count) {
        printf("Invalid index!\n");
        return;
    }

    // pid_t current_pid = getpid();
    MemoryStats stats = get_memory_stats(children[index-1]);
    printf("For current process (PID: %d):\n", children[index-1]);
    printf("VIRT: %lu kB, RES: %lu kB, SHR: %lu kB\n", stats.virt, stats.res, stats.shr);

}

void proc_kill(int index) {
    printf("Process killer function called.\n");
    if (index < 1 || index > current_count) {
        printf("Invalid index!\n");
        return;
    }

    // rewrite array removing killed pid
    kill(children[index-1], SIGTERM);
    waitpid(children[index-1], NULL, 0); // wait for child to terminate

    for (int i = index-1; i < current_count-1; i++) {
        children[i] = children[i+1];
    }
    current_count--;
}

void thread_create() {
    printf("Thread creation function called.\n");

    pthread_t thread;
    static long thread_id = 0;  // id for each thread

    int result = pthread_create(&thread, NULL, thread_function, (void*)thread_id);
    if (result != 0) {
        perror("Thread creation failed");
        return;
    }

    pthread_detach(thread);

    printf("Thread %ld created.\n", thread_id);
    thread_id++;
}

void thread_sync() {
    printf("Thread synchronization function called.\n");
    // using semaphores to sync threads
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&data, 0, 0);

    for (long i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer, (void*)i);
    }

    for (long i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer, (void*)i);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&data);
}

void ipc_pipe_share() {
    printf("IPC using pipe function called.\n");
    // TODO: Implement IPC using pipes
}

int main() {
    int choice, index;

    while (1) {
        printf("\nMenu:\n");
        printf("1. proc_create\n");
        printf("2. proc_list\n");
        printf("3. proc_monit\n");
        printf("4. proc_kill\n");
        printf("5. thread_create\n");
//        printf("6. thread_kill\n");
        printf("6. thread_sync\n");
        printf("7. ipc_pipe_share\n");
        printf("8. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                proc_create(); // done
                break;
            case 2:
                proc_list(); // done
                break;
            case 3:
                printf("\nEnter index of child to monitor: ");
                scanf("%d", &index);
                proc_monit(index); // done
                break;
            case 4:
                printf("\nEnter index of child to terminate: ");
                scanf("%d", &index);
                proc_kill(index); // done
                break;
            case 5:
                thread_create();
                break;
//            case 6:
//                thread_kill();
//                printf("\nEnter index of child to terminate: ");
//                scanf("%d", &index); // tmp holder will have to be replaced
//                proc_kill(index);
//                break;
            case 7:
                thread_sync(); // done
                break;
            case 8:
                ipc_pipe_share();
                break;
            case 9:
                exit(0);
                break;
            default:
                printf("Invalid choice. Try again.");
                break;
        }
    }

    return 0;

}