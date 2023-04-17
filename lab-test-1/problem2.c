#include <stdio.h>
#include <signal.h>

void handler(int signo){
    if(signo == SIGINT){
        printf("I am unstoppable!\n");
    }
}

int main(){

    char c;
    signal(SIGINT, handler);
    while(1){
        scanf("%c%*c", &c);
        if(c=='x'){
            printf("Valar Morghulis\n");
            break;
        }
        if(!((c>='a' && c<='z') || (c>='A' && c<='Z'))){
            printf("Do you speak-a my language?\n");
        }
        else{
            printf("%c\n", c);
        }
    }
}
