// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file strManipulation.h
 * @brief Functions to manipulate strings.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief Convert string to snake case
 * 
 * @param input input string to be converted (e.g. "BatteryState")
 * @param output output string converted to snake case (e.g. "battery_state")
 */
void turnToSnakeCase(const std::string input, std::string& output);


/**
 * @brief Convert string to snake case
 * 
 * @param input input string to be converted (e.g. "BatteryState")
 * @return std::string output string converted to snake case (e.g. "battery_state")
 */
std::string turnToSnakeCase(const std::string input);

/**
 * @brief Get data type path from data type name (e.g. from "sensor_msgs::msg::BatteryState" to "sensor_msgs/msg/battery_state")
 * 
 * @param input string of the data type name
 * @param output string of the data type path
 */
void getDataTypePath(const std::string input, std::string& output);

/**
 * @brief Get path of the file main folder from the file path
 * 
 * @param filePath file path (e.g. "templates/skills/first_template_skill/src/FirstTemplateSkillSM.scxml")
 * @param output string of the path (e.g. "templates/skills/first_template_skill/")
 */
void getPath(const std::string filePath, std::string& output);

/**
 * @brief function to read the template file
 * 
 * @param filePath constant string of the file path
 * @param fileContent string of the file content to be read
 * @return true 
 * @return false 
 */
bool readTemplateFile(const std::string filePath, std::string& fileContent);

/**
 * @brief function to write the file
 * 
 * @param filePath constant string of the file path
 * @param fileContent constant string of the file content to be written
 * @return true 
 * @return false 
 */
bool writeFile(const std::string filePath, const std::string fileName, const std::string fileContent);

/**
 * @brief function that replaces all the occurences of a string with another string
 * 
 * @param str string to be modified passed by reference
 * @param from original string to be replaced
 * @param to new string to replace the original string
 */
void replaceAll(std::string& str, const std::string& from, const std::string& to);

/**
 * @brief function that checks if a string is present in another string
 * 
 * @param str main string where the search is done
 * @param subStr string to be searched
 * @return true 
 * @return false 
 */
bool checkIfStrPresent(const std::string str, const std::string subStr);

/**
 * @brief function that deletes a section of a string that is between two strings
 * 
 * @param str string to be modified passed by reference
 * @param from left string
 * @param to right string
 */
void deleteSection(std::string& str, const std::string& from, const std::string& to);

/**
 * @brief function that saves a section of a string that is between two strings
 * 
 * @param str string to be modified passed by reference
 * @param from left string
 * @param to right string
 * @param savedSection string where to save the section that was found passed by reference
 */
void saveSection(const std::string str, const std::string& from, const std::string& to, std::string& savedSection);

/**
 * @brief function that keeps a section of a string that is between two strings and deletes the left and right strings
 * 
 * @param str string to be modified passed by reference
 * @param from left string 
 * @param to right string 
 */
void keepSection(std::string& str, const std::string& from, const std::string& to);

/**
 * @brief function that writes a string after a command
 * 
 * @param str string to be modified passed by reference
 * @param command command string after which the string is to be written
 * @param addedStr string to be written after the command
 */
void writeAfterCommand(std::string& str, const std::string& command, const std::string& addedStr);

/**
 * @brief function that deletes a command from a string
 * 
 * @param str string to be modified passed by reference
 * @param command command string to be deleted
 */
void deleteCommand(std::string& str, const std::string& command);
