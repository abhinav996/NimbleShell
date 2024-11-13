#include "m_for_master.h"

std::string stripQuotes(const std::string& str) 
{
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2); 
    }
    return str;
}

void handleRedirect(std::vector<std::string> argument) 
{
    int input_fd = -1, output_fd = -1;
    bool input_redirect = false, output_redirect = false, append_redirect = false;
    std::vector<std::string> command;

    // Parse arguments for redirection
    for (size_t i = 0; i < argument.size(); ++i) 
    {
        if (argument[i] == ">") 
        {
            if (i + 1 < argument.size()) 
            {
                output_fd = open(argument[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_fd == -1) 
                {
                    std::cerr << "Error opening file for output: " << strerror(errno) << std::endl;
                    return;
                }
                output_redirect = true;
                ++i; 
            } 
            else 
            {
                std::cerr << "Syntax error: expected filename after '>'\n";
                return;
            }
        }
        else if (argument[i] == ">>") 
        {
            if (i + 1 < argument.size()) 
            {
                output_fd = open(argument[i + 1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (output_fd == -1) 
                {
                    std::cerr << "Error opening file for appending: " << strerror(errno) << std::endl;
                    return;
                }
                append_redirect = true;
                ++i;
            } 
            else 
            {
                std::cerr << "Syntax error: expected filename after '>>'\n";
                return;
            }
        } 
        else if (argument[i] == "<") 
        {
            if (i + 1 < argument.size()) 
            {
                input_fd = open(argument[i + 1].c_str(), O_RDONLY);
                if (input_fd == -1) 
                {
                    std::cerr << "Error opening file for input: " << strerror(errno) << std::endl;
                    return;
                }
                input_redirect = true;
                ++i;
            } 
            else 
            {
                std::cerr << "Syntax error: expected filename after '<'\n";
                return;
            }
        } 
        else 
            command.push_back(stripQuotes(argument[i]));
        
    }

    pid_t pid = fork();
    if (pid == -1) 
    {
        std::cerr << "Fork failed: " << strerror(errno) << std::endl;
        return;
    } 
    else if (pid == 0) 
    { 
        if (input_redirect) 
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_redirect || append_redirect) 
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        std::vector<char*> exec_args;
        for (auto &arg : command) 
        {
            exec_args.push_back(&arg[0]);
        }
        exec_args.push_back(nullptr);
        execvp(exec_args[0], exec_args.data());
        std::cerr << "Command execution failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    } 
    else 
        waitpid(pid, nullptr, 0);

    if (input_fd != -1) close(input_fd);
    if (output_fd != -1) close(output_fd);
}
