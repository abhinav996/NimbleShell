#include "m_for_master.h"

bool is_empty(std::ifstream& filename)
{
    bool ans = filename.peek() == std::ifstream::traits_type::eof();
    return ans;
}
bool check(std::string s)
{
    for(char c : s) 
    {
        if (!std::isdigit(c)) 
        {
            return false;
        }
    }
    return true;
}


void addHistory(std::string argument,std::string dir)
{
    const std::string filename = dir+"/history.txt";  
    const int maxLines = 20;
    std::queue<std::string> q;
    std::string line;

    std::ifstream historyFile(filename);

    if(historyFile.is_open()) 
    {
        while (std::getline(historyFile, line)) 
        {
            q.push(line);
        }
        historyFile.close();
    } 
    else 
    {
        std::ofstream createFile(filename);
        if (!createFile.is_open()) 
        {
            std::cerr << "Unable to create file." << std::endl;
            return;
        }
        createFile.close();
    }

    
    if (q.size() == maxLines) 
        q.pop(); 

    std :: string x = argument.substr(0,1);
    if(q.empty() || q.back() != argument)
    {
        if(x != " ")
            q.push(argument);
    }


    std::ofstream outfile(filename, std::ios::trunc); 
    if (outfile.is_open()) 
    {
        while (!q.empty()) 
        {
            outfile << q.front() << "\n";
            q.pop();
        }
        outfile.close();
    } 
    else 
    {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

void viewHistory(std::vector<std::string> arguments,std::string dir)
{
    const std::string filename =  dir+"/history.txt";
    std::deque<std::string> lines;  
    std::string line;
    int numLinesToShow = 10;  
    std::ifstream infile(filename);
    if (!infile.is_open()) 
    {
        std::cerr << "Unable to open " << filename << std::endl;
        return;
    }

    while (std::getline(infile, line))
        lines.push_back(line);
    
    infile.close();

    int a = lines.size();
    if (arguments.size() == 1)
        numLinesToShow = std::min(numLinesToShow, a);
    else if(arguments.size() == 2){
        if(!check(arguments[1]))
        {
            std::cerr << "Non zero Integral value should be provided." << std::endl;
            return;
        }
        int requestedLines = std::stoi(arguments[1]);
        if (requestedLines >= 0 and requestedLines <= 20) {
            numLinesToShow = std::min(requestedLines, std::min(20, a));
        } 
        else 
        {
            std::cerr << "Invalid number of lines requested." << std::endl;
            return;
        }
    }
    else
    {
        std::cerr << "Maximum one non-negative integer should be providec.\n";
        return;   
    }

    auto start = lines.end() - numLinesToShow;
    for (auto it = start; it != lines.end(); ++it) 
    {
        std::cout << *it << std::endl;
    }
}