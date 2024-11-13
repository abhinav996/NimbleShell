#include "m_for_master.h"

std::vector<pid_t> backgroundProcesses;

void handleBg(std::vector<std::string> command)
{
    pid_t pid = fork(); 
    if (pid == 0)  
    {
        std::vector<char*> args;
        for (const auto& arg : command)
        {
            args.push_back(const_cast<char*>(arg.c_str()));  
        }
        args.push_back(nullptr); 

        setpgid(0,0);
        if (execvp(args[0], args.data()) == -1) 
        {
            std::cerr << "Command not found: " << command[0] << std::endl;
            exit(EXIT_FAILURE);  
        }
    }
    else if (pid > 0) 
    {
        backgroundProcesses.push_back(pid); 
        std::cout << "Process running in background with PID: " << pid << std::endl;
    }
    else  
    {
        std::cerr << "Failed to create a child process for background execution." << std::endl;
    }
}



void handleSigTstp(int signum)
{
    
    if (foregroundPid != -1)
    {
        std::cout << "Pushed to background with PID: " << foregroundPid << std::endl; 
        backgroundProcesses.push_back(foregroundPid);
        kill(foregroundPid, SIGTSTP);
        foregroundPid = -1;
    }
}

void handleSigInt(int signum)
{
    if (foregroundPid != -1)
        kill(foregroundPid, SIGINT);
    else
        return;
}

void setupSignalHandlers()
{

    struct sigaction sa_tstp;
    sa_tstp.sa_handler = handleSigTstp;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = 0;
    sigaction(SIGTSTP, &sa_tstp, nullptr);

    struct sigaction sa_int;
    sa_int.sa_handler = handleSigInt;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, nullptr);
}



void handleFg(std::vector<std::string> argument)
{
    if(argument.size() > 2 and argument.size() < 2)
    {
        std::cerr << "Provide process id to bring to foreground" << std::endl;
        return;
    }
    pid_t pid = stoi(argument[1]);
    
    bool flag = false;
    for(auto it: backgroundProcesses)
    {
        if(it == pid)
        {
            flag = true;
            break;
        }
    }
    if(!flag)
    {
        std::cerr << "Provide correct PID to bring to foreground" << std::endl;
        return;
    }
    kill(pid,SIGCONT);
    tcsetpgrp(STDIN_FILENO, getpgid(pid));
    foregroundPid = pid;
    waitpid(pid,NULL,WUNTRACED);
    backgroundProcesses.erase(find(backgroundProcesses.begin(),backgroundProcesses.end(),pid));
    foregroundPid = -1;
}
