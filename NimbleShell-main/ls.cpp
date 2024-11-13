#include "m_for_master.h"

bool is_directoryReturn(const std::string& path) 
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

std::string getPermissions(struct stat &fileStat) {
    std::string permissions;
    permissions += (S_ISDIR(fileStat.st_mode)) ? 'd' : '-';
    permissions += (fileStat.st_mode & S_IRUSR) ? 'r' : '-';
    permissions += (fileStat.st_mode & S_IWUSR) ? 'w' : '-';
    permissions += (fileStat.st_mode & S_IXUSR) ? 'x' : '-';
    permissions += (fileStat.st_mode & S_IRGRP) ? 'r' : '-';
    permissions += (fileStat.st_mode & S_IWGRP) ? 'w' : '-';
    permissions += (fileStat.st_mode & S_IXGRP) ? 'x' : '-';
    permissions += (fileStat.st_mode & S_IROTH) ? 'r' : '-';
    permissions += (fileStat.st_mode & S_IWOTH) ? 'w' : '-';
    permissions += (fileStat.st_mode & S_IXOTH) ? 'x' : '-';
    return permissions;
}

// Function to display files in long format
void long_format1(const std::string &path, const std::string &file) {
    struct stat fileStat;
    std::string fullPath = path + "/" + file;
    
    if (stat(fullPath.c_str(), &fileStat) == -1) {
        perror("stat");
        return;
    }

    std::string permissions = getPermissions(fileStat);
    std::cout << permissions << " "
              << std::setw(3) << fileStat.st_nlink << " "
              << std::setw(8) << getpwuid(fileStat.st_uid)->pw_name << " "
              << std::setw(8) << getgrgid(fileStat.st_gid)->gr_name << " "
              << std::setw(6) << fileStat.st_size << " "
              << std::setw(12) << std::string(ctime(&fileStat.st_mtime)).substr(4, 12) << " "
              << file << std::endl;
}


void listDirectory(const std::string &path, bool show_all, bool long_format) 
{
    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) 
    {
        perror("Not able to open dir");
        return;
    }

    struct dirent* entry;
    std::vector<std::string> files;
    long total_blocks = 0; 

    while ((entry = readdir(dir)) != nullptr) 
    {
        if (show_all || entry->d_name[0] != '.') 
        {
            files.push_back(entry->d_name);

            std::string fullPath = path + "/" + entry->d_name;
            struct stat fileStat;

            if (stat(fullPath.c_str(), &fileStat) == 0) 
                total_blocks += fileStat.st_blocks; 
            else 
                perror("stat");  
        }
    }
    closedir(dir);

    // Print total block count if long format is enabled
    if (long_format) 
    {
        std::cout << "total " << total_blocks / 2 << std::endl;
        for (const auto& file : files) 
            long_format1(path, file);
        
    } 
    else 
    {
        for (const auto& file : files) 
        {
            std::cout << file << std::endl;
        }
    }
}
// Main function to parse arguments and handle flags/directories
void ls_dir(std::vector<std::string> argss) 
{
    bool show_all = false;
    bool long_format = false;
    std::vector<std::string> directories;

    std::vector<std::string>hmm;
    int n = argss.size();
    for(int i = 1; i < n; i++)
    {
        hmm.push_back(argss[i]);
    }
    // Parse flags and dirctories from the arguments
    for (const auto& arg : hmm) 
    {
        if (arg == "-a") 
            show_all = true;
        else if (arg == "-l") 
            long_format = true;
        else if (arg == "-al" || arg == "-la") 
        {
            show_all = true;
            long_format = true;
        } 
        else 
            directories.push_back(arg);
    }

    if (directories.empty()) 
        directories.push_back(".");

    
    for (const auto& dir : directories) 
    {
        listDirectory(dir, show_all, long_format);
        std::cout << std::endl;
    }
}