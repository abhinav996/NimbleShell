#include "m_for_master.h"

std::vector<std::string> parsePipes(std::vector<std::string> arguments)
{
    std::vector<std::string> commands;
    std::string currentCommand;
    for (const auto& arg : arguments) 
    {
        if (arg == "|") 
        {
            if (!currentCommand.empty()) 
            {
                commands.push_back(currentCommand);
                currentCommand.clear();
            }
        } 
        else 
        {
            if (!currentCommand.empty()) 
            {
                currentCommand += " ";
            }
            currentCommand += arg;
        }
    }

    if (!currentCommand.empty()) 
    {
        commands.push_back(currentCommand);
    }
    return commands;
}

void handlePipe(std::vector<std::string> arguments) 
{
    std::vector<std::string> commands = parsePipes(arguments);
    int num_commands = commands.size();
    int pipefd[2];
    int prev_pipefd = -1;

    for (int i = 0; i < num_commands; ++i) 
    {
        if (i < num_commands - 1) 
        {
            if (pipe(pipefd) == -1) 
            {
                perror("pipe");
                exit(1);
            }
        }

        if (commands[i].substr(0, 2) == "cd") 
            return;

        bool has_redirect = (commands[i].find(">") != std::string::npos || 
                             commands[i].find(">>") != std::string::npos || 
                             commands[i].find("<") != std::string::npos);

        pid_t pid = fork();
        if (pid == -1) 
        {
            perror("fork");
            exit(1);
        } 
        else if (pid == 0) 
        {
            if (prev_pipefd != -1) 
            {
                dup2(prev_pipefd, STDIN_FILENO);
                close(prev_pipefd);
            }

            if (i < num_commands - 1) 
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            if (has_redirect) 
            {
                std::istringstream iss(commands[i]);
                std::vector<std::string> cmd_args;
                std::string token;
                while (iss >> token) 
                    cmd_args.push_back(token);

               
                handleRedirect(cmd_args);
                exit(0); 
            }

           
            std::vector<char*> exec_args;
            std::istringstream iss(commands[i]);
            std::string token;
            while (iss >> token) 
            {
                char* arg = new char[token.size() + 1];
                std::strcpy(arg, token.c_str());
                exec_args.push_back(arg);
            }
            exec_args.push_back(nullptr);

            execvp(exec_args[0], exec_args.data());
            perror("execvp failed"); 
            exit(1);
        } 
        else 
        {
            if (prev_pipefd != -1) 
                close(prev_pipefd);
            
            if (i < num_commands - 1) 
            {
                close(pipefd[1]);
                prev_pipefd = pipefd[0];
            }
        }
    }

    for (int i = 0; i < num_commands; ++i) 
        wait(nullptr);
}
