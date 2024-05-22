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
 * @param input input string to be converted (e.g. "BatteryState")
 * @param output output string converted to snake case (e.g. "battery_state")
 */
void turnToSnakeCase(const std::string input, std::string& output);

/**
 * @brief Get data type path from data type name
 * 
 * @param input input data type name (e.g. from "sensor_msgs::msg::BatteryState")
 * @param output output data type path (e.g. "sensor_msgs/msg/battery_state")
 */
void getDataTypePath(const std::string input, std::string& output);

/**
 * @brief Get the output path from the input file path
 * 
 * @param filePath input file path (e.g. "templates/skills/first_template_skill/src/FirstTemplateSkillSM.scxml")
 * @param output output path (e.g. "templates/skills/first_template_skill/")
 */
void getPath(const std::string filePath, std::string& output);
