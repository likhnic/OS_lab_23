#ifndef __MAIN_HPP
#define __MAIN_HPP

#include <iostream>
#include <pthread.h>
#include <set>
#include <utility>
#include <semaphore.h>
#include <unistd.h>
#include <queue>
using namespace std;


extern int guests_entered = 0, guests_done = 0;
extern sem_t exculsive_cleaner; //mutex
extern sem_t guest_entered_sem, guest_cleaner_sem;
extern sem_t cleaner_sem, guest_sem, room_sem;
extern int cleaners = 0;

extern int x, y, n;

class Room{
    public:
    
    int guests_used;
    int time_used;
    int priority;
    Room(int priority=-1, int guests_used=0, int time_used = 0): guests_used(guests_used), time_used(time_used), priority(priority){}
};

extern Room *room;
extern void *guest(void *arg);
extern void *cleaningStaff(void *arg);

extern queue<Room*>freeRooms;
extern set<pair<int,Room*> >occupiedRooms;
extern queue<Room *>doneRooms;

#endif
