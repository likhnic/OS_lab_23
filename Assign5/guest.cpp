#include "main.hpp"


void *guest(void *arg){

    int priority = *(int *)arg;

    while(1){
        // sleep before requesting a room
        int time = rand()%11+10;
        sleep(time);

        // guest will take a free room if exists, else 
        // he will take a room which has least priority
        Room *room;
        while(1){

            sem_wait(&room_sem);
            if(!freeRooms.empty()){
                room = freeRooms.front();
                freeRooms.pop();
                // guest will enter the room so increase
                // guests_used in room
                room->guests_used++;
                guests_entered++;
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
                occupiedRooms.erase(it);
                room->guests_used++;
                guests_entered++;
            }
            time = rand()%21 + 10;
            room->time_used += time;
            sem_post(&room_sem);
            break;
        }

        sleep(time);
        
        sem_wait(&room_sem);
        if(room->guests_used != 2)freeRooms.push(room);
        else doneRooms.push(room);
        sem_post(&room_sem);

        sem_wait(&room_sem);
        if(guests_entered == 2*n){
            sem_post(&room_sem);
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