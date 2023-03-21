#include "main.hpp"

int main()
{
    srand(time(NULL));
    cout << "Enter x, y, n: ";
    cin >> x >> y >> n;
    pthread_t guests[y], cleaningStaffs[x];

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
