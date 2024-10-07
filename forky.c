// forky.c
// Jacob Gray, 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void pattern1(int num_processes)
{
    pid_t processes[num_processes];
    for (int i = 1; i <= num_processes; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            pid_t current_pid = getpid();
            printf("Process %d (PID %d) beginning\n", i, current_pid);
            fflush(stdout);

            srand(time(NULL) ^ (getpid() << 16));
            int sleep_time = (rand() % 8) + 1;
            sleep(sleep_time);

            printf("Process %d (PID %d) exiting after %d seconds\n", i, current_pid, sleep_time);
            fflush(stdout);
            exit(0);
        }
        else if (pid > 0)
        {
            processes[i - 1] = pid;
            printf("Main process (PID %d) created Process %d (PID %d)\n", getpid(), i, pid);
            fflush(stdout);
        }
        else
        {
            exit(1);
        }
    }

    for (int i = 0; i < num_processes; i++)
    {
        waitpid(processes[i], NULL, 0);
    }
}

void pattern2(int i, int num_processes)
{
    if (i > num_processes) return;

    pid_t pid = fork();
    if (pid == 0)
    {
        pid_t current_pid = getpid();
        printf("Process %d (PID %d) beginning\n", i, current_pid);
        fflush(stdout);

        int sleep_time = (rand() % 8) + 1;
        sleep(sleep_time);

        printf("Process %d (PID %d) exiting after %d seconds\n", i, current_pid, sleep_time);
        fflush(stdout);

        pattern2(i + 1, num_processes);
        exit(0);
    }
    else if (pid > 0)
    {
        printf("Process %d (PID %d) started Process %d (PID %d)\n", i, getpid(), i + 1, pid);
        fflush(stdout);
        waitpid(pid, NULL, 0);
    }
    else
    {
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Invalid number of arguments\n");
        exit(1);
    }

    int num_processess = atoi(argv[1]);
    int pattern = atoi(argv[2]);

    switch (pattern)
    {
    case 1:
        pattern1(num_processess);
        break;
    case 2:
        pattern2(1, num_processess);
        break;
    default:
        printf("Invalid pattern\n");
        exit(1);
    }

    return 0;
}