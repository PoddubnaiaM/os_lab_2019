#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    pid_t PID;

    PID = fork();
    if(PID > 0)
    {
        printf("One child process created, PID=%d\n",PID);
        sleep(100);
    }
    else
        return 1;
    return 0;
}
