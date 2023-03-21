#include "main.hpp"


void *guest(void *arg){

    int priority = *(int *)arg;

    while(1){
        // sleep before requesting a room
        int time = rand()%11+2;
        // sem_wait(&prints);
        // printf("Guest %d is sleeping for %d seconds\n", priority, time);
        // sem_post(&prints);
        sleep(time);

        // guest will take a free room if exists, else 
        // he will take a room which has least priority
        Room *room;
        while(1){
            // printf("OK\n");
            sem_wait(&room_sem);
            // printf("OK\n");

            if(!freeRooms.empty()){
                room = freeRooms.front();
                freeRooms.pop();
                // guest will enter the room so increase
                // guests_used in room
                room->guests_used++;
                guests_entered++;
                room->guest_thread = pthread_self();
                room->priority = priority;
                if(room->guests_used == 1)occupiedRooms.insert({room->priority, room});
            }
            else{
                // take room which has priority just less than priority
                // of guest thread
                auto it = occupiedRooms.lower_bound({priority, NULL});
                if(it == occupiedRooms.begin()){
                    // no room is available
                    // so guest will wait
                    sem_post(&room_sem);
                    continue;
                }
                it--;
                room = it->second;
                // signal SIGUSR1 to the guest thread which is using the room
                pthread_kill(room->guest_thread, SIGUSR1);
                printf("Guest %d got kicked out by guest %d\n", room->priority, priority);
                occupiedRooms.erase(it);
                room->guests_used++;
                room->priority = priority;
                guests_entered++;
                room->guest_thread = pthread_self();
            }
            time = rand()%21 + 2;
            room->time_used += time;
            // sem_wait(&prints);
            printf("Guest %d is using room for %d seconds\n", priority, time);
            // sem_post(&prints);
            sem_post(&room_sem);
            break;
        }

        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, NULL);
        struct timespec timeout;
        timeout.tv_sec = time;
        timeout.tv_nsec = 0;

        int sig = sigtimedwait(&mask, NULL, &timeout);


        sem_wait(&room_sem);
        // sem_wait(&prints);
        printf("Guest %d is leaving room\n", priority);
        // sem_post(&prints);
        if(room->guests_used != 2)freeRooms.push(room);
        else {
            doneRooms.push(room);
            printf("Room %d need to be cleaned\n", room->priority);
        }
        if(doneRooms.size() == n){
            
            // now time to clean
            // post to cleaning staff
            guests_entered = 0;
            for(int i=0;i<n;++i){
                Room *room = new Room();
                freeRooms.push(room);
            }
            sem_post(&cleaner_sem); // initially value = 0
        }
        else sem_post(&room_sem);
    }

}