#include<iostream>
#include<vector>
#include <sys/utsname.h>
#include<sys/wait.h>
#include<string>
#include<filesystem>
#include<unistd.h>
#include<algorithm>
#include <cstdio> 
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <queue>
#include <sstream>
#include <dirent.h>
#include <fstream>
#include <cstring>
#include <fcntl.h>  
#include <termios.h>
#include <limits.h>
#include <sys/types.h>
#include <signal.h>



void long_format1(std::string& path, const std::string& filename);
void ls_dir(std::vector<std::string> argss);
void listDirectory(const std::string &path, bool show_all, bool long_format);

std::string get_pwd();
std::string get_current_directory(std::string c);

void bolo(std::vector<std::string>arguments);

int validate(std :: vector<std :: string> arguments);
void changePath(std::vector<std::string> arguments, std::string currentPath, std::string& previousPath, int flag,std::string myhome);
bool is_directoryReturn(const std::string& path);

void khojo(std::vector<std::string> arguments);

void addHistory(std::string raw, std::string dir);
void viewHistory(std::vector<std::string> arguments,std::string dir);


void handlePipe(std::vector<std::string> argument);
void handleRedirect(std::vector<std::string> argument);

std::string handleSignal(std::string path,std::string prompt);


void handlePinfo(const std::vector<std::string>& arguments);

void handleBg(std::vector<std::string> command);
void handleFg( std::vector<std::string> command);

void setupSignalHandlers();

#ifndef FPID
#define FPID
extern pid_t foregroundPid ;
#endif