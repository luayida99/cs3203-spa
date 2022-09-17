#pragma once
#include <regex>
#include <string>
#include <vector>

/*
 * Class containing helper functions for QPS.
 */
class Utils {
public:
    /*
     * Delimits a string based on char.
     */
    static std::vector<std::string> splitString(std::string str, char splitter);

    /*
     * Removes trailing space at the start and end of a string.
     */
    static std::string removeTrailingSpaces(std::string s);

    /*
     * Tokenizes a string based on a list of characters.
     */
    static std::vector<std::string> tokenize(std::string str,
                                             std::vector<char> special_chars);

    /*
     * Checks if a string contains a term.
     */
    static bool in(std::vector<std::string> v, std::string item);

    /*
     * Removes string from string.
     */
    static std::string removeString(std::string input, std::string remove);
};
