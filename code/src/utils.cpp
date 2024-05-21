/**
 * @file utils.cpp
 * @brief Functions to manipulate strings.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include "utils.h"

/**
 * @brief Convert string to snake case
 * 
 * @param input 
 * @param output 
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
 * @param input 
 * @param output 
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
 * @brief Get the path 
 * 
 * @param filePath 
 * @param output 
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