/**
 * @file utils.h
 * @brief Functions to manipulate strings.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include <string>
#include <iostream>

/**
 * @brief Convert string to snake case
 * 
 * @param input 
 * @param output 
 */
void turnToSnakeCase(const std::string input, std::string& output);

/**
 * @brief Get data type path from data type name (e.g. from "sensor_msgs::msg::BatteryState" to "sensor_msgs/msg/battery_state")
 * 
 * @param input 
 * @param output 
 */
void getDataTypePath(const std::string input, std::string& output);

/**
 * @brief Get the path 
 * 
 * @param filePath 
 * @param output 
 */
void getPath(const std::string filePath, std::string& output);
