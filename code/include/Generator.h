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
 * @param eventData event data structure passed by reference from which the event string is extracted and the component and function data are stored
 */
void getDataFromEvent(eventDataStr& eventData);

/**
 * @brief Get the skill Data From SCXML Root Name object
 * 
 * @param attributeName root attribute name from which the skill data is extracted
 * @param skillData skill data structure passed by reference where the skill data is stored
 * @return bool 
 */
bool getDataFromRootName(const std::string attributeName, skillDataStr& skillData);

/**
 * @brief Prints the event data
 * 
 * @param eventData event data structure to be printed
 */
void printEventData(eventDataStr eventData);

/**
 * @brief Prints the skill data
 * 
 * @param skillData skill data structure to be printed
 */
void printSkillData(skillDataStr skillData);

/**
 * @brief Process the event data
 * 
 * @param fileData the file data structure contianing the model and the interface file paths
 * @param eventData event data structure 
 * @param skillData skill data structure
 * @param target event target state
 * @param hCode structure where the header code is stored
 * @param cppCode structure where the cpp code is stored
 * @param topicCode structure where the topic code is stored
 */
void processEvent(fileDataStr fileData, eventDataStr eventData, const skillDataStr skillData, std::string target, hCodeStr& hCode, cppCodeStr& cppCode, topicCodeStr& topicCode);

/**
 * @brief Generate the code for the events
 * 
 * @param fileData the file data structure contianing the model and the interface file paths
 * @param elementsTransition vector of transition elements
 * @param elementsSend vector of send elements
 * @param skillData skill data structure
 * @param hCode structure where the header code is stored
 * @param cppCode structure where the cpp code is stored
 * @param topicCode structure where the topic code is stored
 */
void getEventsCode(fileDataStr fileData, const std::vector<tinyxml2::XMLElement*> elementsTransition, const std::vector<tinyxml2::XMLElement*> elementsSend, skillDataStr skillData, hCodeStr& hCode, cppCodeStr& cppCode, topicCodeStr& topicCode);

/**
 * @brief Write the header code
 * 
 * @param skillData skill data structure 
 * @param code code structure where the header code is stored
 * @param datamodel_mode data model mode flag
 */
void writeHCode(const skillDataStr skillData, hCodeStr& code, bool datamodel_mode);

/**
 * @brief Write the cpp code
 * 
 * @param skillData skill data structure
 * @param code code structure where the cpp code is stored
 * @param datamodel_mode data model mode flag
 */
void writeCppCode(const skillDataStr skillData, cppCodeStr& code, bool datamodel_mode);

/**
 * @brief Write the header code for the data model
 * 
 * @param skillData skill data structure
 * @param code code structure where the data model header code is stored
 */
void writeDataModelHCode(const skillDataStr skillData, hCodeStr& code);

/**
 * @brief Write the cpp code for the data model
 * 
 * @param skillData skill data structure
 * @param code code structure where the data model cpp code is stored
 */
void writeDataModelCppCode(const skillDataStr skillData, cppDataModelCodeStr& code);

/**
 * @brief Generate the data model header file
 * 
 * @param outputPath path where to save the output file
 * @param outputFileName filename of the output file
 * @param code code structure where the data model header code is stored
 */
void generateDataModelHFile(const std::string outputPath, const std::string outputFileName, hCodeStr code);

/**
 * @brief Generate the data model cpp file
 * 
 * @param outputPath path where to save the output file
 * @param outputFileName filename of the output file
 * @param code code structure where the data model cpp code is stored
 */
void generateDataModelCppFile(const std::string outputPath, const std::string outputFileName, cppDataModelCodeStr code);

/**
 * @brief Generate the header file
 * 
 * @param outputPath path where to save the output file
 * @param outputFileName filename of the output file
 * @param skillData skill data structure
 * @param code code structure where the header code is stored
 */
void generateHFile(const std::string outputPath, const std::string outputFileName, const skillDataStr skillData, hCodeStr code);

/**
 * @brief Generate the cpp file
 * 
 * @param outputPath path where to save the output file
 * @param outputFileName filename of the output file
 * @param skillData skill data structure
 * @param code code structure where the cpp code is stored
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






