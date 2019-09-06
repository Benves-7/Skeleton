#include <iostream>
#include <string>
#include <pthread.h>
#include <thread>
#include <limits.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <math.h>
#include <fstream>
#include <wait.h>

void BigParser()
{
    std::string line;
    std::ifstream file ("/proc/cpuinfo");
    while (getline(file, line))
    {
        if (line.substr(0, 10) == "model name")
        {
            std::cout << "Model name: " << line.substr(13) << std::endl;
            break;
        }
    }
}
void calcAndShutdown()
{
    std::cout << "PID: " << getpid() << std::endl;
    double value;
    for (unsigned int i = 1; i <= 50000; ++i)
    {
        for (unsigned int j = 1; j <= i; ++j)
        {
            value += sqrt(i*j);
        }
    }
    std::cout << "PID " << getpid() << " got SUM: " << value << std::endl;
    return;
}
void fork_entry(int num)
{
    pid_t ParentPID = getpid();
    for (int k = 0; k < num; ++k)
    {
        pid_t pid = fork();
        if (getpid() != ParentPID)
        {
            // child process
            calcAndShutdown();
            kill(getpid(), SIGTERM);
        }
    }
    for (int i = 0; i < num; ++i)
    {
        wait(NULL);
    }
}
void thread_entry(int num)
{
    std::thread threads[num];
    for (int i = 0; i < num; ++i)
    {
        threads[i] = std::thread(calcAndShutdown);
    }
    for (int j = 0; j < num; ++j)
    {
        threads[j].join();
    }

}

int main()
{
    while (true)
    {
        int num;
        std::string input;
        std::cout << "Input: ";
        std::getline(std::cin, input);
        if (input == "-i")
        {
            char hostname[HOST_NAME_MAX];   gethostname(hostname,HOST_NAME_MAX);
            std::cout << "Hostname: " << hostname << std::endl;
            /* Conversion constants. */
            const long minute = 60;
            const long hour = minute * 60;
            const long day = hour * 24;
            const double megabyte = 1024 * 1024;
            /* Obtain system statistics. */
            struct sysinfo si;
            sysinfo (&si);
            /* Summarize interesting values. */
            printf ("system uptime : %ld days, %ld:%02ld:%02ld\n",
                    (si.uptime / day),
                    (si.uptime % day) / hour,
                    (si.uptime % hour) / minute,
                    (si.uptime % minute));
            printf ("total RAM   : %5.1f MB\n", si.totalram / megabyte);
            printf ("free RAM   : %5.1f MB\n", si.freeram / megabyte);
            printf ("process count : %d\n", si.procs);

            BigParser();
            unsigned int numCPU = std::thread::hardware_concurrency();
            std::cout << "Number of CPUs: " << numCPU << std::endl;
        }
        else if (input[0] == '-' && input[1] == 'f' && input[2] == ' ')
        {
            input.erase(0,2);
            if (input == "")
                continue;
            num = std::stoi(input);

            fork_entry(num);
        }
        else if (input[0] == '-' && input[1] == 't' && input[2] == ' ')
        {
            input.erase(0,2);
            if (input == "")
                continue;
            num = std::stoi(input);

            thread_entry(num);
        }
        else if (input[0] == '-' && input[1] == 'e')
            break;
        else
        {
            std::cout << "Not Valid Input!" << std::endl;
            std::cout << "-i for info." << std::endl;
            std::cout << "-f X to fork processors in X amounts." << std::endl;
            std::cout << "-t X to do the same as -f but with threads." << std::endl;
        }
    }
}




