#include "class.hpp"

vector<Operative> operatives;
vector<Group> groups;
vector<Station> stations;
void cleanup_resources()
{
    logging_active.store(false);
    pthread_cond_signal(&log_cond);
    pthread_join(logging_thread, nullptr);

    for (int i = 0; i < NUM_STATIONS; i++)
    {
        pthread_mutex_destroy(&station_lock[i]);
    }

    for (int i = 0; i < N / M; i++)
    {
        pthread_mutex_destroy(&group_mutex[i]);
    }

    pthread_mutex_destroy(&rc_mutex);
    pthread_mutex_destroy(&rw_mutex);
    pthread_mutex_destroy(&exit_mutex);
    pthread_mutex_destroy(&log_mutex);
    pthread_cond_destroy(&exit_cond);
    pthread_cond_destroy(&log_cond);
}

void initialize()
{
    operatives.reserve(N);
    for (int i = 1; i <= N; i++)
        operatives.emplace_back(Operative(i));

    groups.reserve(N / M);
    for (int i = 0; i < N / M; i++)
    {
        groups.emplace_back(Group(i));
        pthread_mutex_init(&group_mutex[i], nullptr);
    }

    for (int i = 0; i < NUM_STATIONS; i++)
    {
        stations.emplace_back(Station(i));
        pthread_mutex_init(&station_lock[i], nullptr);
    }

    pthread_mutex_init(&log_mutex, nullptr);
    pthread_create(&logging_thread, nullptr, logging_worker, nullptr);

    start_time = chrono::high_resolution_clock::now();
}

void *operative_activities(void *arg)
{
    Operative *op = (Operative *)arg;
    usleep(op->writing_time * SLEEP_MULTIPLIER);
    print_output("Operative " + to_string(op->id) + " has arrived at station " +
                 to_string(op->station_id + 1) + " at time " + to_string(get_time()));
    pthread_mutex_lock(&station_lock[op->station_id]);
    print_output("Operative " + to_string(op->id) + " has started printing at station " +
                 to_string(op->station_id + 1) + " at time " + to_string(get_time()));
    usleep(printing_time * SLEEP_MULTIPLIER);
    pthread_mutex_unlock(&station_lock[op->station_id]);
    print_output("Operative " + to_string(op->id) + " has finished printing at station " +
                 to_string(op->station_id + 1) + " at time " + to_string(get_time()));
    groups[op->group_id].add_printed_count(operatives);
    if (op->is_leader())
    {
        sem_wait(&op->lock);
        print_output("Group " + to_string(op->group_id + 1) + " has completed printing phase at time " +
                     to_string(get_time()));

        usleep((get_random_number() % 3 + 1) * SLEEP_MULTIPLIER);

        pthread_mutex_lock(&rw_mutex);
        print_output("Group " + to_string(op->group_id + 1) + " started writing to the logbook at time " +
                     to_string(get_time()));

        usleep((get_random_number() % 4 + 1) * SLEEP_MULTIPLIER);

        int submissions = n_submissions.fetch_add(1) + 1;
        print_output("Group " + to_string(op->group_id + 1) + " finished writing to the logbook at time " +
                     to_string(get_time()));
        pthread_mutex_unlock(&rw_mutex);
    }

    return nullptr;
}

void *staff_activities(void *arg)
{
    int staff_id = *(int *)arg;
    int starting_time = get_random_number() % 3 + 1;
    int reading_interval = get_random_number() % 4 + 2;
    usleep(starting_time * SLEEP_MULTIPLIER);
    while (true)
    {
        pthread_mutex_lock(&rc_mutex);
        rc++;
        if (rc == 1)
        {
            pthread_mutex_lock(&rw_mutex);
        }
        pthread_mutex_unlock(&rc_mutex);

        int current_submissions = n_submissions.load();
        print_output("Staff " + to_string(staff_id) + " has started reading the logbook at time " +
                     to_string(get_time()) + ". Submissions = " + to_string(current_submissions));

        usleep(read_write_time * SLEEP_MULTIPLIER);

        current_submissions = n_submissions.load();
        print_output("Staff " + to_string(staff_id) + " has finished reading the logbook at time " +
                     to_string(get_time()) + ". Submissions = " + to_string(current_submissions));

        pthread_mutex_lock(&rc_mutex);
        rc--;
        if (rc == 0)
        {
            pthread_mutex_unlock(&rw_mutex);
        }
        pthread_mutex_unlock(&rc_mutex);

        if (current_submissions >= N / M)
        {
            break;
        }
        usleep(reading_interval * SLEEP_MULTIPLIER);
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: ./a.out <input_file> <output_file>" << endl;
        return 1;
    }
    ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        cerr << "Error: Cannot open input file " << argv[1] << endl;
        return 1;
    }
    ofstream outputFile(argv[2]);
    if (!outputFile.is_open())
    {
        cerr << "Error: Cannot open output file " << argv[2] << endl;
        return 1;
    }
    streambuf *cinBuf = cin.rdbuf(inputFile.rdbuf());
    streambuf *coutBuf = cout.rdbuf(outputFile.rdbuf());
    if (!(cin >> N >> M >> printing_time >> read_write_time))
    {
        cerr << "Error: Invalid input format" << endl;
        cin.rdbuf(cinBuf);
        cout.rdbuf(coutBuf);
        return 1;
    }
    initialize();
    pthread_t operative_threads[N];
    pthread_t staff_threads[NSTAFFS];
    int staff_ids[NSTAFFS];
    for (int i = 0; i < NSTAFFS; ++i)
    {
        staff_ids[i] = i + 1;
        if (pthread_create(&staff_threads[i], nullptr, staff_activities, &staff_ids[i]) != 0)
        {
            throw runtime_error("Failed to create staff thread");
        }
    }
    for (int i = 0; i < N; ++i)
    {
        if (pthread_create(&operative_threads[i], nullptr, operative_activities, &operatives[i]) != 0)
        {
            throw runtime_error("Failed to create operative thread");
        }
    }
    for (int i = 0; i < N; ++i)
    {
        pthread_join(operative_threads[i], nullptr);
    }
    pthread_mutex_lock(&exit_mutex);
    while (!shutdown_flag.load() && n_submissions.load() < N / M)
    {
        struct timespec timeout;
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += 10;

        if (pthread_cond_timedwait(&exit_cond, &exit_mutex, &timeout) != 0)
        {
            shutdown_flag.store(true);
            break;
        }
    }
    pthread_mutex_unlock(&exit_mutex);
    shutdown_flag.store(true);
    for (int i = 0; i < NSTAFFS; ++i)
    {
        pthread_join(staff_threads[i], nullptr);
    }
    cleanup_resources();
    cin.rdbuf(cinBuf);
    cout.rdbuf(coutBuf);
    return 0;
}