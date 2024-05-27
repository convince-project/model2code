/**
 * @file strManipulation.cpp
 * @brief Functions to manipulate strings.
 * @version 0.1
 * @date 2024-05-20
 * 
*/

#include "strManipulation.h"

/**
 * @brief Convert string to snake case
 * 
 * @param input string to be converted
 * @param output output string converted to snake case
 */
void turnToSnakeCase(const std::string input, std::string& output)
{   
    std::string temp = input;
    bool flag = false;
    for (char c : temp){
        if (isupper(c)){
            if(flag){
                output.push_back('_');
            }
            else{
                flag = true;
            }
        }
        output.push_back(tolower(c));
    }
}

/**
 * @brief Get data type path from data type name (e.g. from "sensor_msgs::msg::BatteryState" to "sensor_msgs/msg/battery_state")
 * 
 * @param input string of the data type name
 * @param output string of the data type path
 */
void getDataTypePath(const std::string input, std::string& output)
{
    std::string temp = input;
    size_t pos = 0;
    if (temp == ""){
        std::cerr << "Input DataType has no value" << std::endl;
        return;
    }

    while ((pos = temp.find("::", pos)) != std::string::npos) {
        temp.replace(pos, 2, "/");
        pos += 1;
    }

    turnToSnakeCase(temp, output);
}

/**
 * @brief Get path of the file main folder from the file path
 * 
 * @param filePath file path (e.g. "templates/skills/first_template_skill/src/FirstTemplateSkillSM.scxml")
 * @param output string of the path (e.g. "templates/skills/first_template_skill/")
 */
void getPath(const std::string filePath, std::string& output) {
    // searching for the second to last appearance of '/' to get the path
    size_t pos = filePath.find_last_of('/');
    std::string temp = filePath;

    if (pos != std::string::npos) {
        pos = filePath.find_last_of('/', pos - 1);
        if (pos != std::string::npos) {
            output = temp.substr(0, pos + 1);
        }
    }
}

/**
 * @brief function to read the template file
 * 
 * @param filePath constant string of the file path
 * @param fileContent string of the file content to be read
 * @return true 
 * @return false 
 */
bool readTemplateFile(const std::string filePath, std::string& fileContent)
{
    std::ifstream readFile(filePath);
    if (!readFile.is_open()) {
        std::cerr << "Failed to open template file for reading: " << filePath << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << readFile.rdbuf();

    if (readFile.fail() && !readFile.eof()) {
        std::cerr << "Error while reading the file: " << filePath << std::endl;
        readFile.close();
        return false;
    }
    fileContent = buffer.str();
    readFile.close();
    return true;
}

/**
 * @brief function to write the file
 * 
 * @param filePath constant string of the file path
 * @param fileContent constant string of the file content to be written
 * @return true 
 * @return false 
 */
bool writeFile(const std::string filePath, const std::string fileContent)
{
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }
    outputFile << fileContent;
    outputFile.close();
    return true;
}

/**
 * @brief function that replaces all the occurences of a string with another string
 * 
 * @param str string to be modified passed by reference
 * @param from original string to be replaced
 * @param to new string to replace the original string
 */
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); 
    }
}

/**
 * @brief function that checks if a string is present in another string
 * 
 * @param str main string where the search is done
 * @param subStr string to be searched
 * @return true 
 * @return false 
 */
bool checkIfStrPresent(const std::string str, const std::string subStr) {
    return str.find(subStr) != std::string::npos;
}

/**
 * @brief function that deletes a section of a string that is between two strings
 * 
 * @param str string to be modified passed by reference
 * @param from left string
 * @param to right string
 */
void deleteSection(std::string& str, const std::string& from, const std::string& to){
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        size_t end_pos = str.find(to, start_pos);
        str.erase(start_pos, end_pos - start_pos + to.length());
    }
}

/**
 * @brief function that saves a section of a string that is between two strings
 * 
 * @param str string to be modified passed by reference
 * @param from left string
 * @param to right string
 * @param savedSection string where to save the section that was found passed by reference
 */
void saveSection(const std::string str, const std::string& from, const std::string& to, std::string& savedSection){
    size_t start_pos = str.find(from);
    if (start_pos != std::string::npos) {
        start_pos += from.length(); 
        size_t end_pos = str.rfind(to); 
        if (end_pos != std::string::npos && end_pos > start_pos) {
            savedSection = str.substr(start_pos, end_pos - start_pos);
        }
    }
}

/**
 * @brief function that keeps a section of a string that is between two strings and deletes the left and right strings
 * 
 * @param str string to be modified passed by reference
 * @param from left string 
 * @param to right string 
 */
void keepSection(std::string& str, const std::string& from, const std::string& to){
    replaceAll(str, from, "");
    replaceAll(str, to, "");
}

/**
 * @brief function that writes a string after a command
 * 
 * @param str string to be modified passed by reference
 * @param command command string after which the string is to be written
 * @param addedStr string to be written after the command
 */
void writeAfterCommand(std::string& str, const std::string& command, const std::string& addedStr) {
    size_t start_pos = 0;
    while((start_pos = str.find(command, start_pos)) != std::string::npos) {
        start_pos += command.length();
        str.insert(start_pos, addedStr);
        start_pos += addedStr.length();
    }
}

/**
 * @brief function that deletes a command from a string
 * 
 * @param str string to be modified passed by reference
 * @param command command string to be deleted
 */
void deleteCommand(std::string& str, const std::string& command) {
    size_t start_pos = 0;
    while((start_pos = str.find(command, start_pos)) != std::string::npos) {
        str.erase(start_pos, command.length());
    }
}

