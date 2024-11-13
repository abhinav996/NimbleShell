#include "m_for_master.h"


void khojo(std::vector<std::string> arguments) 
{
    if (arguments.size() != 2) 
    {
        std::cerr << "Please provide only one argument with search." << std::endl;
        return;
    }

    std::string target = arguments[1];  
    std::string current_directory = "."; 

    std::queue<std::string> directories; 
    directories.push(current_directory);

    while (!directories.empty()) 
    {
        std::string current_path = directories.front();
        directories.pop();

        DIR* dir = opendir(current_path.c_str());
        if (dir == nullptr) {
            perror(("Cannot open directory: " + current_path).c_str());
            continue;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) 
        {
            std::string entry_name = entry->d_name;

            if (entry_name == "." || entry_name == "..") 
                continue;


            std::string full_path = current_path + "/" + entry_name;

            if (entry_name == target) 
            {
                std::cout << "True" << std::endl;
                closedir(dir);
                return;
            }
            if (entry->d_type == DT_DIR) 
                directories.push(full_path);
        }
        closedir(dir);
    }
    std::cout << "False" << std::endl;
}