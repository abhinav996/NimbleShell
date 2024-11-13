#include "m_for_master.h"

bool is_directory(const std::string& path) 
{
    // std::cout << path << "\n";
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

int validate(std::vector<std::string> arguments) {

    if (arguments.size() == 1)
        return true;
    if(arguments.size() > 2)
        return false;
    
    std :: vector<std::string> correctFlags = {".","..","/-","/~"};
    
    bool flags = find(correctFlags.begin(), correctFlags.end(), arguments[1]) == correctFlags.end();
    bool flags2 = is_directory(arguments[1]);

    if(!flags and !flags2)
        return 0;
    return 1;
}
void changePath(std::vector<std::string> arguments, std::string currentPath, std::string& previousPath, int flag, std::string myhome) 
{
    // std :: cout << arguments[1] << std :: endl;
    if (flag == 0) {
        std::cerr << "Wrong arguments." << std::endl;
        return;
    }
    std :: vector<std::string> correctFlags = {".","..","/-","/~"}; 
    bool flags = find(correctFlags.begin(), correctFlags.end(), arguments[1]) == correctFlags.end();
   
    if(arguments.size() == 2 and !flags and  !is_directory(arguments[1]))
    {
        std::cerr << "Wrong arguments." << std::endl;
        return;   
    }
    // std::cout << previousPath << "\n";
    std::string newPath = currentPath;
    bool f = 0;
    if (arguments.size() == 1 || arguments[1] == "~") 
        newPath = myhome;
    else if (arguments[1] == "-")
    {
        // std::swap(newPath, previousPath);
        std  :: string temp = newPath;
        newPath = previousPath;
        previousPath = temp;
        f = 1;
    } 
    else 
        newPath = arguments[1];


    chdir(newPath.c_str());
    if(!f)
        previousPath = currentPath;
}