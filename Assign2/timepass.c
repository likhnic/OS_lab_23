#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    char *arv[] = {"pwd",  NULL};
    // execvp(argv[1], argv+1);
    execvp("pwd", arv);
    // printf("%s\n", argv[2]);
    fprintf(stderr, "Failed to execvp() '%s' (%d: %s)\n", arv[0], errno,strerror(errno));
    return(EXIT_FAILURE);
}
