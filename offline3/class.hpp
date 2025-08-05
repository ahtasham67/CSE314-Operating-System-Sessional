#include "header.hpp"

class Operative
{
public:
    int id, group_id, station_id, writing_time;
    operative_state state;
    sem_t lock;

    Operative(int id) : id(id)
    {
        group_id = (id - 1) / M; 
        station_id = (id - 1) % NUM_STATIONS; 
        state = WRITING_REPORT;
        writing_time = get_random_number() % MAX_WRITING_TIME + 1;
        sem_init(&lock, 0, 0);
    }
    ~Operative() {
        sem_destroy(&lock);
    }
    bool is_leader() const { return id % M == 0; }
};

class Group
{
public:
    int id, leader_id;
    atomic<int> printed_count;

    Group(int id) : id(id), printed_count(0)
    {
        leader_id = (id + 1) * M;
    }

    Group(Group&& other) noexcept 
        : id(other.id), leader_id(other.leader_id), printed_count(other.printed_count.load()) {}
    
    Group& operator=(Group&& other) noexcept {
        if (this != &other) {
            id = other.id;
            leader_id = other.leader_id;
            printed_count.store(other.printed_count.load());
        }
        return *this;
    }
    Group(const Group&) = delete;
    Group& operator=(const Group&) = delete;

    void add_printed_count(vector<Operative> &operatives)
    {
        int count = printed_count.fetch_add(1) + 1;
        
        if (count == M)
        {
            sem_post(&operatives[leader_id - 1].lock);
        }
    }
};
class Station
{
public:
    int id;
    station_state state;
    Station(int id) : id(id), state(IDLE) {}
};