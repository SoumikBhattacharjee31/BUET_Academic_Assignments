#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

#define MAX_GALLERY1_CAPACITY 5
#define MAX_CORRIDOR_CAPACITY 3
#define CLOCK_REALTIME 0

// Mutexes and semaphores
pthread_mutex_t step_mutex[3];
pthread_mutex_t gallery1_mutex, booth_mutex;
pthread_mutex_t standard_mutex, premium_mutex;
pthread_mutex_t priority_mutex, output_mutex;
sem_t gallery1_capacity, corridor_capacity;

// variables
int N, M, w, x, y, z;
int standard_count = 0, premium_count = 0;

// Visitor structure
typedef struct
{
    int id;
    char type; // 'S' for standard, 'P' for premium
} Visitor;

// prototypes
void *visit_museum(void *arg);

// delay
int get_random_number()
{
    random_device rd;
    mt19937 generator(rd());
    double lambda = 10000.234;
    poisson_distribution<int> poissonDist(lambda);
    return poissonDist(generator);
}
void random_delay(int max_time)
{
    usleep((get_random_number() % max_time + 1) * 1000); // Convert milliseconds to microseconds
}
void specific_delay(int time)
{
    usleep((time + 1) * 1000);
}

// time
auto start_time = std::chrono::high_resolution_clock::now();
long long get_time()
{
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(
        end_time - start_time);
    long long elapsed_time_ms = duration.count();
    return elapsed_time_ms;
}

// print
void write_output(std::string output)
{
    pthread_mutex_lock(&output_mutex);
    std::cout << output;
    pthread_mutex_unlock(&output_mutex);
}

int main(int argc, char *argv[])
{
    // Input
    if (argc == 7)
    {
        N = atoi(argv[1]); // Number of standard ticket holders
        M = atoi(argv[2]); // Number of premium ticket holders
        w = atoi(argv[3]); // Time in hallway AB
        x = atoi(argv[4]); // Time in Gallery 1
        y = atoi(argv[5]); // Time in Gallery 2
        z = atoi(argv[6]); // Time in photo booth
    }
    else
        scanf("%d%d%d%d%d%d", &N, &M, &w, &x, &y, &z);

    // Initialize mutexes and semaphores
    pthread_mutex_init(&step_mutex[0], NULL);
    pthread_mutex_init(&step_mutex[1], NULL);
    pthread_mutex_init(&step_mutex[2], NULL);
    pthread_mutex_init(&gallery1_mutex, NULL);
    pthread_mutex_init(&booth_mutex, NULL);
    pthread_mutex_init(&standard_mutex, NULL);
    pthread_mutex_init(&premium_mutex, NULL);
    pthread_mutex_init(&priority_mutex, NULL);
    sem_init(&gallery1_capacity, 0, MAX_GALLERY1_CAPACITY);
    sem_init(&corridor_capacity, 0, MAX_CORRIDOR_CAPACITY);

    // Create visitor threads for both standard and premium ticket holders
    pthread_t visitors[N + M];
    Visitor visitor_data[N + M];

    // create random vector to create threads in random order
    vector<int> indices;
    for (int i = 0; i < N + M; i++)
        indices.push_back(i);
    random_device rd;
    mt19937 generator(rd());
    shuffle(indices.begin(), indices.end(), generator);

    // Create visitor threads
    for (int j = 0; j < M + N; j++)
    {
        int i = indices.back();
        indices.pop_back();
        if (i < N)
        {
            visitor_data[i].id = 1001 + i;
            visitor_data[i].type = 'S';
            pthread_create(&visitors[i], NULL, visit_museum, &visitor_data[i]);
        }
        else
        {
            visitor_data[i].id = 2001 + i - N;
            visitor_data[i].type = 'P';
            pthread_create(&visitors[i], NULL, visit_museum, &visitor_data[i]);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < N + M; i++)
        pthread_join(visitors[i], NULL);

    // Cleanup
    pthread_mutex_destroy(&step_mutex[0]);
    pthread_mutex_destroy(&step_mutex[1]);
    pthread_mutex_destroy(&step_mutex[2]);
    pthread_mutex_destroy(&gallery1_mutex);
    pthread_mutex_destroy(&booth_mutex);
    pthread_mutex_destroy(&standard_mutex);
    pthread_mutex_destroy(&premium_mutex);
    pthread_mutex_destroy(&priority_mutex);
    sem_destroy(&gallery1_capacity);
    sem_destroy(&corridor_capacity);

    return 0;
}


void *visit_museum(void *arg)
{
    // init
    Visitor *visitor = (Visitor *)arg;

    // 0
    random_delay(50);
    // A
    write_output("Visitor " + to_string(visitor->id) + " has arrived at A at timestamp " + to_string(get_time()) + "\n");
    // A->B
    specific_delay(w);
    // B
    write_output("Visitor " + to_string(visitor->id) + " has arrived at B at timestamp " + to_string(get_time()) + "\n");
    // B->1
    pthread_mutex_lock(&step_mutex[0]);
    // 1
    write_output("Visitor " + to_string(visitor->id) + " is at step 1 at timestamp " + to_string(get_time()) + "\n");
    random_delay(50);
    // 1->2
    pthread_mutex_lock(&step_mutex[1]);
    pthread_mutex_unlock(&step_mutex[0]);
    // 2
    write_output("Visitor " + to_string(visitor->id) + " is at step 2 at timestamp " + to_string(get_time()) + "\n");
    random_delay(50);
    // 2->3
    pthread_mutex_lock(&step_mutex[2]);
    pthread_mutex_unlock(&step_mutex[1]);
    // 3
    write_output("Visitor " + to_string(visitor->id) + " is at step 3 at timestamp " + to_string(get_time()) + "\n");
    random_delay(50);
    // 3->C
    sem_wait(&gallery1_capacity);
    pthread_mutex_unlock(&step_mutex[2]);
    // C
    write_output("Visitor " + to_string(visitor->id) + " is at C (entered Gallery 1) at time " + to_string(get_time()) + "\n");
    // C->D
    specific_delay(x);
    sem_wait(&corridor_capacity);
    sem_post(&gallery1_capacity);
    // D
    write_output("Visitor " + to_string(visitor->id) + " is at D (exiting Gallery 1) at time " + to_string(get_time()) + "\n");
    // D->E
    random_delay(50);
    sem_post(&corridor_capacity);
    // E
    write_output("Visitor " + to_string(visitor->id) + " is at E (entered Gallery 2) at time " + to_string(get_time()) + "\n");
    specific_delay(y);
    random_delay(50);

    if (visitor->type == 'S')
    {
        write_output("Visitor " + to_string(visitor->id) + " is about to enter the photo booth at timestamp " + to_string(get_time()) + "\n");
        // priority
        pthread_mutex_lock(&priority_mutex);
        // prevent premium
        pthread_mutex_lock(&standard_mutex);
        standard_count++;
        if (standard_count == 1)
            pthread_mutex_lock(&booth_mutex);
        pthread_mutex_unlock(&standard_mutex);
        pthread_mutex_unlock(&priority_mutex);
        // access
        write_output("Visitor " + to_string(visitor->id) + " is inside the photo booth at timestamp " + to_string(get_time()) + "\n");
        specific_delay(z);
        write_output("Visitor " + to_string(visitor->id) + " is leaving the photo booth at timestamp " + to_string(get_time()) + "\n");
        // allow premium
        pthread_mutex_lock(&standard_mutex);
        standard_count--;
        if (standard_count == 0)
            pthread_mutex_unlock(&booth_mutex);
        pthread_mutex_unlock(&standard_mutex);
    }
    else
    {
        // priority
        write_output("Visitor " + to_string(visitor->id) + " is about to enter the photo booth at timestamp " + to_string(get_time()) + "\n");
        pthread_mutex_lock(&premium_mutex);
        premium_count++;
        if (premium_count == 1)
            pthread_mutex_lock(&priority_mutex);
        pthread_mutex_unlock(&premium_mutex);
        // access
        pthread_mutex_lock(&booth_mutex);
        write_output("Visitor " + to_string(visitor->id) + " is inside the photo booth at timestamp " + to_string(get_time()) + "\n");
        specific_delay(z);
        write_output("Visitor " + to_string(visitor->id) + " is leaving the photo booth at timestamp " + to_string(get_time()) + "\n");
        pthread_mutex_unlock(&booth_mutex);
        // priority
        pthread_mutex_lock(&premium_mutex);
        premium_count--;
        if (premium_count == 0)
            pthread_mutex_unlock(&priority_mutex);
        pthread_mutex_unlock(&premium_mutex);
    }
    return NULL;
}
