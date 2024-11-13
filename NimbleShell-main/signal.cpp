#include "m_for_master.h"


std::string getLastWord(const std::string& input) 
{
    auto pos = input.find_last_of(" \t\n");
    if (pos == std::string::npos) 
        return input;  
    else 
        return input.substr(pos + 1); 
}

void setRawMode(bool enable) 
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    if (enable) 
        term.c_lflag &= ~(ICANON | ECHO); 
    else 
        term.c_lflag |= (ICANON | ECHO);   
    
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


std::deque<std::string> loadHistoryFromFile(const std::string& filename) 
{
    std::deque<std::string> history;
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        history.push_back(line);
    }
    return history;
}

std::vector<std::string> getMatchingFiles(const std::string& prefix) 
{
    std::vector<std::string> matches;
    DIR* dir = opendir(".");
    if (dir == nullptr) return matches;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) 
    {
        std::string name = entry->d_name;
        if (name.find(prefix) == 0)  
            matches.push_back(name);
        
    }
    closedir(dir);
    std::sort(matches.begin(), matches.end()); 
    return matches;
}

std::string handleSignal(std::string filename,  std::string prompt) 
{
    std::deque<std::string> history = loadHistoryFromFile(filename + "/history.txt");
    std::string currentInput;
    int historyIndex = history.size(); 
    char c;

    setRawMode(true);  

    while (true) 
    {
        c = getchar();
        if(c == 4)
        {
            setRawMode(false);
            std::cout << std::endl;
            exit(0);
        }
        if (c == 27) 
        {  
            getchar();  
            c = getchar();
            
                if (c == 'A') 
                {  
                    if (!history.empty() && historyIndex > 0) 
                    {
                        historyIndex--;
                        currentInput = history[historyIndex];
                        std::cout << "\33[2K\r";
                        std::cout << prompt << currentInput;
                    }
                } 
                else if (c == 'B') 
                {  
                    int h = history.size();
                    if (!history.empty() && historyIndex < h - 1) 
                    {
                        historyIndex++;
                        currentInput = history[historyIndex];
                        std::cout << "\33[2K\r";
                        std::cout << prompt << currentInput;
                    } 
                    else if (historyIndex == h - 1) 
                    {
                        historyIndex++;
                        currentInput.clear();
                        std::cout << "\33[2K\r";
                        std::cout << prompt << currentInput;
                    }
                }
        } 
        else if (c == '\t') 
        {  
            std::string lastWord = getLastWord(currentInput);  
            std::vector<std::string> matches = getMatchingFiles(lastWord);
            
            if (matches.size() == 1) 
            {  
                currentInput = currentInput.substr(0, currentInput.size() - lastWord.size()) + matches[0];
                std::cout << "\33[2K\r" << prompt << currentInput;
            } 
            else if (matches.size() > 1) 
            { 
                std::cout << "\n";
                for (const auto& match : matches) 
                {
                    std::cout << match << " ";
                }
                std::cout << "\n" << prompt << currentInput;
            }
        } 
        else if (c == 127) 
        {  // Backspace key
            if (!currentInput.empty()) 
            {
                currentInput.pop_back();
                std::cout << "\33[2K\r" << prompt << currentInput;
            } 
            else 
            {
                std::cout << "\33[2K\r" << prompt;
            }
        } 
        else if (c == '\n') 
        {  // Enter key
            std::cout << std::endl;
            break;
        } 
        else 
        {
            currentInput += c;  
            std::cout << c;      
            std::cout.flush();
        }
    }

    setRawMode(false);  
    return currentInput;
}
