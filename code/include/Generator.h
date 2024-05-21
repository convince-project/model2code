/*
 * SPDX-FileCopyrightText: 2024-2024 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file Generator.h
 * @brief This file is the header of the code Generator.cpp file
 */
#include "ExtractFromXML.h"
#include "utils.h"
#include <fstream>
#include <map>

#define modelFilePath "./templates/specifications/full-model.xml"
#define interfaceFilePath "./templates/specifications/interfaces.xml"

#define RETURN_CODE_ERROR 1
#define RETURN_CODE_OK    0

#define cmdTick "CMD_TICK"
#define cmdHalt "CMD_HALT"
#define rspTick "TICK_RESPONSE"
#define rspHalt "HALT_RESPONSE"

static std::map<std::string, bool> eventsMap;

struct cppCodeStr
{
    std::string includeCode; 
    std::string converterCode; 
    std::string constructorCode;
    std::string spinCode;
    std::string startCode;
    std::string servicesCode; 
    std::string handlersCode;
    std::string endstartCode;
    std::string callbacksCode; 
};

struct hCodeStr
{
    std::string includeCode;
    std::string statusCode;
    std::string openClassCode;
    std::string publicCode;
    std::string privateCode;
    std::string closeClassCode;
};

struct cppDataModelCodeStr
{
    std::string includeCode; 
    std::string spinCode;
    std::string logCode;
    std::string setupCode;
    std::string subscriptionCode; 
    std::string callbacksCode; 
    std::string endsetupCode;
};

struct topicCodeStr
{
    std::string includeCode;
    std::string publicCode;
    std::string privateCode;
    std::string servicesCode;
    std::string callbacksCode;
};

/**
 * @brief Get component and function data from event name
 * 
 * @param eventData 
 */
void getDataFromEvent(eventDataStr& eventData);

/**
 * @brief Get the skill Data From SCXML Root Name object
 * 
 * @param attributeName 
 * @param skillData 
 * @return bool 
 */
bool getDataFromRootName(const std::string attributeName, skillDataStr& skillData);

/**
 * @brief Prints the event data
 * 
 * @param eventData 
 */
void printEventData(eventDataStr eventData);

/**
 * @brief Prints the skill data
 * 
 * @param skillData 
 */
void printSkillData(skillDataStr skillData);

/**
 * @brief Process the event data
 * 
 * @param eventData 
 * @param skillData 
 * @param target 
 * @param hCode 
 * @param cppCode 
 * @param topicCode 
 */
void processEvent(fileDataStr fileData, eventDataStr eventData, const skillDataStr skillData, std::string target, hCodeStr& hCode, cppCodeStr& cppCode, topicCodeStr& topicCode);

/**
 * @brief Generate the code for the events
 * 
 * @param elementsTransition 
 * @param elementsSend 
 * @param skillData 
 * @param hCode 
 * @param cppCode 
 * @param topicCode 
 */
void getEventsCode(fileDataStr fileData, const std::vector<tinyxml2::XMLElement*> elementsTransition, const std::vector<tinyxml2::XMLElement*> elementsSend, skillDataStr skillData, hCodeStr& hCode, cppCodeStr& cppCode, topicCodeStr& topicCode);

/**
 * @brief Write the header code
 * 
 * @param skillData 
 * @param code 
 * @param datamodel_mode 
 */
void writeHCode(const skillDataStr skillData, hCodeStr& code, bool datamodel_mode);

/**
 * @brief Write the cpp code
 * 
 * @param skillData 
 * @param code 
 * @param datamodel_mode 
 */
void writeCppCode(const skillDataStr skillData, cppCodeStr& code, bool datamodel_mode);

/**
 * @brief Write the header code for the data model
 * 
 * @param skillData 
 * @param code 
 */
void writeDataModelHCode(const skillDataStr skillData, hCodeStr& code);

/**
 * @brief Write the cpp code for the data model
 * 
 * @param skillData 
 * @param code 
 */
void writeDataModelCppCode(const skillDataStr skillData, cppDataModelCodeStr& code);

/**
 * @brief Generate the data model header file
 * 
 * @param outputPath 
 * @param outputFileName 
 * @param code 
 */
void generateDataModelHFile(const std::string outputPath, const std::string outputFileName, hCodeStr code);

/**
 * @brief Generate the data model cpp file
 * 
 * @param outputPath 
 * @param outputFileName 
 * @param code 
 */
void generateDataModelCppFile(const std::string outputPath, const std::string outputFileName, cppDataModelCodeStr code);

/**
 * @brief Generate the header file
 * 
 * @param outputPath 
 * @param outputFileName 
 * @param skillData 
 * @param code 
 */
void generateHFile(const std::string outputPath, const std::string outputFileName, const skillDataStr skillData, hCodeStr code);

/**
 * @brief Generate the cpp file
 * 
 * @param outputPath 
 * @param outputFileName 
 * @param skillData 
 * @param code 
 */
void generateCppFile(const std::string outputPath, const std::string outputFileName, const skillDataStr skillData, cppCodeStr code);

/**
 * @brief Print the help message
 * 
 */
void print_help();

/**
 * @brief Generator function
 * 
 * @param fileData 
 * @return true 
 * @return false 
 */
bool generator(fileDataStr fileData);






