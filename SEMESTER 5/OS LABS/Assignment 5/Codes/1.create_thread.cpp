#include<bits/stdc++.h>
#include<pthread.h> 

void *print_message(void *arg) {
    printf("Thread message: %s\n", (char *)arg);
    return NULL;
}

int main() {
    pthread_t thread;
    const char *message = "Hello from the thread!";
    pthread_create(&thread, NULL, print_message, (void *)message);
    pthread_join(thread, NULL); // Wait for the thread to finish
    return 0;
}
