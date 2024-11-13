#include "m_for_master.h"
pid_t foregroundPid = -1;

std::string get_username() 
{
    char* username = getenv("USER");
    if (username == nullptr) {
        return "unknown";
    }
    return username;
}
std::string get_system_name() 
{
    struct utsname uname_info;
    uname(&uname_info);
    return uname_info.nodename;
}


int main()
{
    setupSignalHandlers();
    std :: string line;
    std::string c = get_pwd();
    std::string previousPath = get_pwd();
    while(true)
    {
        std::string username, sys_name, current_dir;
        username = get_username();
        sys_name = get_system_name();
        current_dir = get_current_directory(c);
        std::string prompt =  username + "@" + sys_name+":"+current_dir+"> ";
        std::cout << username << "@" << sys_name << ":" << current_dir << "> ";
    
        std::string in = handleSignal(c,prompt);
        line = in;
        std::istringstream stream(line);
        std::string token;
        std::vector<std::vector<std::string>> commands;
        std::vector<std::string> current_command;

        addHistory(line,c);
        while (stream >> token) 
        {
            if(token[0] == token[token.size() - 1] and token[0] == '\"')
            {
                current_command.push_back(token);
                continue;
            }
            size_t pos = 0;
            while ((pos = token.find(';')) != std::string::npos) 
            {
                if (pos > 0) 
                    current_command.push_back(token.substr(0, pos));

                if (!current_command.empty()) 
                {
                    commands.push_back(current_command);
                    current_command.clear();
                }
                token = token.substr(pos + 1);
            }
            if (!token.empty()) 
                current_command.push_back(token);
        }

        if (!current_command.empty())
            commands.push_back(current_command);
        
        bool exit = false;
        for(auto i : commands)
        {
                            

            if(i.size() == 0)
                continue;

            bool pipe = false, redirect =false, bg = false;
            for(auto& j : i)
            {
                int n = j.size();
                // std::cout << "loda " << j << std::endl;
                if(j == "|")
                    pipe = true;
                if(j == ">" || j == "<" || j == ">>")
                    redirect = true;
                if(j[n-1] == '&')
                {
                    j = j.substr(0, n - 1);
                    bg = true;
                }
            }
            
            if(pipe)
            {
                handlePipe(i);
                continue;
            }
            if(redirect)
            {
                handleRedirect(i);
                continue;
            }
            if(bg)
            {
                handleBg(i);
                continue;
            }
    
            std :: string curr_command = i[0];
            if(curr_command == "fg")
            {
                handleFg(i);
            }
            else if(curr_command == "exit")
            {
                exit = true;
                break;
            }
            if(curr_command == "pwd")
            {
                std::cout << get_pwd() << std::endl;
            }
            else if(curr_command == "cd")
            {
                
                int flag = validate(i);
                changePath(i,get_pwd(),previousPath,flag,c);
            }
            else if(curr_command == "ls")
            {
                ls_dir(i);
            }
            else if(curr_command == "echo")
            {
                bolo(i);
            }
            else if(curr_command == "search")
            {
                khojo(i);
            }
            else if(curr_command == "history")
            {
                viewHistory(i,c);
            }
            else if(curr_command == "pinfo")
            {
                handlePinfo(i);
            }
            else
            {
                pid_t pid = fork();  
                if (pid == 0)  
                {
                    std::vector<char*> args;  
                    for (const auto& arg : i)  
                    {
                        args.push_back(const_cast<char*>(arg.c_str()));  
                    }
                    args.push_back(nullptr);  
                    
                    if (execvp(args[0], args.data()) == -1)
                    {
                        std::cerr << "Command not found: " << curr_command << std::endl;
                        continue;
                    }
                }
                else if (pid > 0) 
                {
                    int status;
                    foregroundPid = pid;
                    waitpid(pid, &status, 0);
                }
                else
                {
                    std::cerr << "Failed to create a child process." << std::endl;
                }
            }
        }
        if(exit)
            break;
    }
}