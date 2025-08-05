#include <chrono>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <random>
#include <atomic>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

#define SLEEP_MULTIPLIER 10000
#define NUM_STATIONS 4
#define NBINDERS 2
#define NSTAFFS 2
#define MAX_WRITING_TIME 10

pthread_mutex_t station_lock[NUM_STATIONS];
pthread_mutex_t group_mutex[100];
pthread_mutex_t rc_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t exit_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t exit_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t log_cond = PTHREAD_COND_INITIALIZER;
queue<string> log_queue;
atomic<bool> logging_active(true);
pthread_t logging_thread;
atomic<int> rc(0);
atomic<int> n_submissions(0);
atomic<bool> shutdown_flag(false);

int M, N;
int printing_time, binding_time, read_write_time;
auto start_time = std::chrono::high_resolution_clock::now();

enum station_state
{
    IDLE,
    OCCUPIED
};

enum operative_state
{
    WRITING_REPORT,
    WAITING_FOR_PRINTING,
    PRINTING,
    PRINTING_DONE
};
long long get_time()
{
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return duration.count();
}
void* logging_worker(void* arg) {
    while (logging_active.load() || !log_queue.empty()) {
        pthread_mutex_lock(&log_mutex);
        
        while (log_queue.empty() && logging_active.load()) {
            pthread_cond_wait(&log_cond, &log_mutex);
        }
        if (!log_queue.empty()) {
            string message = log_queue.front();
            log_queue.pop();
            pthread_mutex_unlock(&log_mutex);
            cout << message << endl;
            cout.flush();
        } else {
            pthread_mutex_unlock(&log_mutex);
        }
    }
    return nullptr;
}
void print_output(const string &msg)
{
    pthread_mutex_lock(&log_mutex);
    log_queue.push(msg);
    pthread_cond_signal(&log_cond);
    pthread_mutex_unlock(&log_mutex);
}
thread_local std::random_device rd;
thread_local std::mt19937 generator(rd());
thread_local std::poisson_distribution<int> poissonDist(10);
int get_random_number()
{
    return poissonDist(generator);
}