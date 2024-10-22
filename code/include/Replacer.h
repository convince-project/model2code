// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file Replacer.h
 * @brief This file contains the functions to replace the placeholders in the template files with the data from the input file.
 * @version 0.1
 * @date 2024-05-24
 * 
 */

#include "ExtractFromXML.h"
#include <fstream>

/**
 * @brief Get the Event Data from the model and interface files 
 * 
 * @param fileData file data structure passed by reference from which the event data is extracted
 * @param eventData event data structure passed by reference where the event data is stored
 */
bool getEventData(fileDataStr fileData, eventDataStr& eventData);

/**
 * @brief Get the Event Data from the vector of event elements
 * 
 * @param fileData file data structure passed by reference where the file data is stored
 * @param elementsTransition vector of transition event elements
 * @param elementsSend vector of send event elements
 */
bool getEventsVecData(fileDataStr fileData, const std::vector<tinyxml2::XMLElement*> elementsTransition, const std::vector<tinyxml2::XMLElement*> elementsSend);

/**
 * @brief function to keep or delete the sections of the code related to the Command Tick event
 * 
 * @param code string of code to be modified
 * @param keepFlag flag to keep or delete the sections
 */
void handleCmdTickEvent(std::string& code,const bool keepFlag);

/**
 * @brief function to keep or delete the sections of the code related to the Command Halt event
 * 
 * @param code string of code to be modified
 * @param keepFlag flag to keep or delete the sections
 */
void handleCmdHaltEvent(std::string& code,const bool keepFlag);

/**
 * @brief function to keep or delete the sections of the code related to the Response Tick event
 * 
 * @param code string of code to be modified
 * @param keepFlag flag to keep or delete the sections
 */
void handleRspTickEvent(std::string& code,const bool keepFlag);

/**
 * @brief function to keep or delete the sections of the code related to the Response Halt event
 * 
 * @param code string of code to be modified
 * @param keepFlag flag to keep or delete the sections
 */
void handleRspHaltEvent(std::string& code,const bool keepFlag);

/**
 * @brief function to write the generic event code in the output string
 * 
 * @param eventData event data structure containing event information
 * @param savedCode saved code structure containing the saved code to be written
 * @param str output string to be modified
 */
void handleGenericEvent(const eventDataStr eventData, const savedCodeStr savedCode, std::string& str);

/**
 * @brief function to save specific sections of the code in the saved code structure
 * 
 * @param savedCode saved code structure passed by reference where the saved code is stored
 * @param code code string where the code is extracted and then deleted
 */
void saveCode(savedCodeStr& savedCode, std::string& code);

/**
 * @brief function to replace the event code in the code map
 * 
 * @param codeMap code map where the code is stored
 */
void replaceEventCode(std::map <std::string, std::string>& codeMap);

/**
 * @brief function to read the template files and store the code in the code map
 * 
 * @param templateFileData template file data structure passed by reference where the template file data is stored
 * @param codeMap code map where the code is stored
 * @return true 
 * @return false 
 */
bool readTemplates(templateFileDataStr& templateFileData, std::map <std::string, std::string>& codeMap);


bool createDirectory(const std::string& path);

/**
 * @brief main function to get the code from template files and replace the placeholders with the data from the input file
 * 
 * @param fileData file data structure passed by reference where the file data is stored
 * @param templateFileData template file data structure passed by reference where the template file data is stored
 * @return true 
 * @return false 
 */
bool Replacer(fileDataStr& fileData, templateFileDataStr& templateFileData);