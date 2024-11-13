#include "m_for_master.h"



// Function to get process information
void handlePinfo(const std::vector<std::string>& arguments) 
{
    std::string pid;

    if (arguments.size() == 1) {
        pid = std::to_string(getpid());
    } 
    else if (arguments.size() == 2) 
        pid = arguments[1]; 
    else 
    {
        std::cerr << "Usage: pinfo [pid]" << std::endl;
        return;
    }

    // File paths in the /proc directory
    std::string status_path = "/proc/" + pid + "/status";
    std::string stat_path = "/proc/" + pid + "/stat";
    std::string exe_path = "/proc/" + pid + "/exe";

    // Reading process status
    std::ifstream status_file(status_path);
    if (!status_file.is_open()) {
        std::cerr << "Error: Could not open status file for PID " << pid << std::endl;
        return;
    }

    std::string line;
    std::string process_state;
    std::string memory;

    // Parse the /proc/[pid]/status file
    while (std::getline(status_file, line)) {
        if (line.rfind("State:", 0) == 0) {
            std::istringstream iss(line);
            std::string ignore;
            iss >> ignore >> process_state;  
        } else if (line.rfind("VmSize:", 0) == 0) {
            std::istringstream iss(line);
            std::string ignore;
            iss >> ignore >> memory;  
        }
    }

    status_file.close();

    
    std::ifstream stat_file(stat_path);
    if (!stat_file.is_open()) 
    {
        std::cerr << "Error: Could not open stat file for PID " << pid << std::endl;
        return;
    }

    std::vector<std::string> stat_fields;
    std::string field;

    
    while (stat_file >> field) 
    {
        stat_fields.push_back(field);
    }
    
    stat_file.close();
    std::string tty_nr = stat_fields[6];
    std::string sid = stat_fields[5];

    pid_t pi = getpid();
    if(arguments.size() > 1)
    {
        pi = std::stoi(arguments[1]);
    }
    if(tcgetpgrp(STDIN_FILENO) == getpgid(pi)) 
        process_state += "+";

    char exe_path_buffer[PATH_MAX];
    ssize_t len = readlink(exe_path.c_str(), exe_path_buffer, sizeof(exe_path_buffer) - 1);
    if (len != -1) 
        exe_path_buffer[len] = '\0';
    else 
    {
        std::cerr << "Error: Could not read executable path for PID " << pid << std::endl;
        return;
    }

    // Print process information
    std::cout << "pid -- " << pid << std::endl;
    std::cout << "Process Status -- " << process_state << std::endl;
    std::cout << "memory -- " << memory << " {Virtual Memory}" << std::endl;
    std::cout << "Executable Path -- " << exe_path_buffer << std::endl;
}
