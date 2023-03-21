#include "main.hpp"

int guests_entered = 0, guests_done = 0;
sem_t exculsive_cleaner; // mutex
sem_t guest_entered_sem, guest_cleaner_sem;
sem_t cleaner_sem, guest_sem, room_sem;
int cleaners = 0;
pthread_t *guests, *cleaningStaffs;
Room *room;
int x, y, n;
queue<Room *> freeRooms;
set<pair<int, Room *>> occupiedRooms;
queue<Room *> doneRooms;

int main()
{
    srand(time(NULL));
    cout << "Enter x, y, n: ";
    cin >> x >> y >> n;
    
    guests = new pthread_t[y];
    cleaningStaffs = new pthread_t[x];

    // init semaphores
    sem_init(&room_sem, 0, 1);
    sem_init(&cleaner_sem, 0, 0);

    for (int i = 0; i < y; ++i)
    {
        int k = rand() % y;
        pthread_create(&guests[i], NULL, guest, (void *)&k);
    }

    for (int i = 0; i < x; ++i)
    {
        pthread_create(&cleaningStaffs[i], NULL, cleaningStaff, (void *)&i);
    }

    for (int i = 0; i < y; ++i)
    {
        pthread_join(guests[i], NULL);
    }

    for (int i = 0; i < x; ++i)
    {
        pthread_join(cleaningStaffs[i], NULL);
    }

    // destroy semaphores
    sem_destroy(&room_sem);
    sem_destroy(&cleaner_sem);
}
