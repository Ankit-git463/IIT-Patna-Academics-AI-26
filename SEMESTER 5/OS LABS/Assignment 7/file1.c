#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

pthread_mutex_t forks[N]; // Mutexes representing forks
pthread_t philosophers[N];

void* philosopher(void* arg) {
    int id = *(int*)arg;
    int leftFork = id;
    int rightFork = (id + 1) % N;

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", id + 1);
        sleep(1); // Simulate thinking time

        // Pick up forks (left fork first, then right fork)
        printf("Philosopher %d is hungry and tries to pick up fork %d.\n", id + 1, leftFork + 1);
        pthread_mutex_lock(&forks[leftFork]);

        printf("Philosopher %d picked up fork %d.\n", id + 1, leftFork + 1);
        printf("Philosopher %d tries to pick up fork %d.\n", id + 1, rightFork + 1);
        pthread_mutex_lock(&forks[rightFork]);

        // Eating
        printf("Philosopher %d is eating, picked up forks %d and %d.\n", id + 1, leftFork + 1, rightFork + 1);
        sleep(2); // Simulate eating time

        // Put down forks
        pthread_mutex_unlock(&forks[rightFork]);
        printf("Philosopher %d put down fork %d.\n", id + 1, rightFork + 1);
        pthread_mutex_unlock(&forks[leftFork]);
        printf("Philosopher %d put down fork %d.\n", id + 1, leftFork + 1);
    }

    return NULL;
}

int main() {
    int ids[N];

    // Initialize mutexes for forks
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for philosopher threads to complete (this will not happen in an infinite loop)
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes for forks
    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
