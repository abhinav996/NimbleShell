#include "m_for_master.h"

std::string get_pwd()
{
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    return cwd;
}

std::string get_current_directory(std::string c) 
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        perror("getcwd() error");
        return "";
    }
    std::string current_dir(cwd);
    if(current_dir == c)
        return "~";
    else if(current_dir.size() < c.size())
        return current_dir;
    else
    {
        if (current_dir.substr(0, c.size()) == c) 
            return "~" + current_dir.substr(c.size());

        return current_dir;
    }
}