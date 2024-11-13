#include "m_for_master.h"


void bolo(std::vector<std::string> arguments) 
{

    std::string echout;
    bool double_quote_open = false; 
    bool single_quote_open = false; 

    if (arguments.size() <= 1) 
    {
        std::cout << "\n";
        return;
    }

    for (size_t i = 1; i < arguments.size(); ++i) 
    {
        const std::string& word = arguments[i];
        for (char ch : word) 
        {
            if (ch == '"') 
            {
                if (!single_quote_open) 
                {
                    double_quote_open = !double_quote_open; 
                    continue; 
                }
            } 
            else if (ch == '\'') 
            {
                if (!double_quote_open) 
                {
                    single_quote_open = !single_quote_open; 
                    continue; 
                }
            }
            echout += ch;
        }

        echout += " "; 
    }

    if (!double_quote_open && !single_quote_open) 
        std::cout << echout << "\n";
    else 
        std::cout << "Provide Correct inverted commas\n";

}