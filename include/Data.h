// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file Data.h
 * @brief This file contains the structures used in the code generation and functions to manipulate the data.
 * @version 0.1
 * @date 2024-05-24
 * 
 */
#pragma once


#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "strManipulation.h"
#include <vector>

#define modelFilePath "./templates/specifications/full-model.xml"
#define interfaceFilePath "./templates/specifications/interfaces.xml"

#define templateFilePath "./template_skill/"
#define cppTemplateFileRelativePath "/src/TemplateSkill.cpp"
#define hTemplateFileRelativePath "/include/TemplateSkill.h"
#define cppDataModelTemplateFileRelativePath "/src/TemplateSkillDataModel.cpp"
#define hDataModelTemplateFileRelativePath "/include/TemplateSkillDataModel.h"
#define mainTemplateFileRelativePath "/src/main.cpp"
#define cMakeTemplateFileRelativePath "/CMakeLists.txt"
#define pkgTemplateFileRelativePath "/package.xml"

#define RETURN_CODE_ERROR 1
#define RETURN_CODE_OK    0

#define cmdTick "CMD_TICK"
#define cmdHalt "CMD_HALT"
#define rspTick "TICK_RESPONSE"
#define rspHalt "HALT_RESPONSE"



struct skillDataStr{
    std::string SMName;
    std::string className;
    std::string classNameSnakeCase;
    std::string skillName;
    std::string skillType;
    std::string skillTypeLC;
};

struct interfaceDataStr{
    std::string interfaceDataType;
    std::string interfaceDataPath;
};

struct eventDataStr{
    std::string event;//
    std::string target;//
    std::string eventName;
    std::string eventType;//
    std::map<std::string, std::string> paramMap;
    std::string componentName;
    std::string functionName;
    std::string functionNameSnakeCase;
    std::string nodeName;
    std::string serverName;
    std::string clientName;
    std::string topicName;
    std::string interfaceName; 
    std::string interfaceType; // type of the interface (e.g. "sync-service", "async-service", "action", "topic")
    
    std::string messageInterfaceType; // type of the interface (e.g. std_msgs::srv::AddTwoInts, blackboard_interfaces::BlackboardInterface, std_msgs::msg::String)
    std::string rosInterfaceType; //type of the interface in ROS (e.g. "sync-service", "async-service", "action", "topic")
    std::string scxmlInterfaceName; //name of the interface in the scxml file
    bool virtualInterface;

    std::vector<std::string> interfaceRequestFields; //list of interfaces used in the event
    std::vector<std::string> interfaceResponseFields; //list of interfaces used in the event
    std::vector<std::string> interfaceTopicFields; //list of interfaces used in the event
    std::map<std::string, std::string> interfaceData; //field and type
};

struct fileDataStr{
    std::string inputFileName;
    std::string inputFileNameGeneration;
    std::string interfaceFileName;
    std::string modelFileName;
    std::string outputPath;
    std::string outputPathInclude;
    std::string outputPathSrc;
    std::string outputCMakeListsFileName;
    std::string outputPackageXMLFileName;
    std::string outputMainFileName;
    std::string outputDatamodelFileNameH;
    std::string outputDatamodelFileNameCPP;
    std::string outputFileNameH;
    std::string outputFileNameCPP;
    std::string outputFileTranslatedSM;
    bool debug_mode;
    bool datamodel_mode;   
    bool translate_mode;   
    bool generate_mode;   
    bool verbose_mode;
    bool is_action_skill; // true if the skill is an action skill, false otherwise
};

struct templateFileDataStr{
    std::string templatePath;
    std::string hFile;
    std::string cppFile;
    std::string hDatamodelFile;
    std::string cppDatamodelFile;
    std::string CMakeListsFile;
    std::string packageFile;
    std::string mainFile;
};

struct savedCodeStr{
    std::string eventC;
    std::string interfaceH;
    std::string sendParam;
    std::string returnParam;
    std::string interfaceCMake;
    std::string packageCMake;
    std::string interfaceXML;
    std::string topicSubscriptionC;
    std::string topicCallbackC;  
    std::string topicSubscriptionH;
    std::string topicCallbackH; 
    std::string topicInterfaceH; 
    std::string topicParamList;
    std::string actionC;
    std::string actionH;
    std::string actionInterfaceH;
    std::string actionSendParam;
    std::string actionFeedbackParam;
    std::string actionFeedbackParamFnc;
    std::string actionFeedbackData;
    std::string actionSendGoalLambda;
    std::string actionResultRequestLambda;
    std::string actionFeedbackLambda;   
    std::string actionSendGoalFnc; 
    std::string actionResultCallbackFnc; 
    std::string actionFeedbackCallback; 
    std::string actionGoalResponseFnc; 
};

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

static std::map<std::string, eventDataStr> eventsMap;

/**
 * @brief Get component and function data from event string (Format: componentName.functionName.eventName)
 * 
 * @param eventData event data structure passed by reference from which the event string is extracted and the component and function data are stored
 */
void getDataFromEvent(eventDataStr& eventData);

/**
 * @brief Get the data From SCXML Root Name object (Format: NameSkillType)
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
 * @brief Prints the event data to cerr
 * 
 * @param eventData event data structure to be printed
 */
void printEventDataToCerr(eventDataStr eventData);

/**
 * @brief Prints the skill data
 * 
 * @param skillData skill data structure to be printed
 */
void printSkillData(skillDataStr skillData);

/**
 * @brief Set the File Data object
 * 
 * @param fileData file data structure passed by reference where the file data is stored
 * @param skillData const skill data structure used to set the file data
 */
void setFileData(fileDataStr& fileData, const skillDataStr skillData);

/**
 * @brief Add a message to the log
 * 
 * @param fileName file name
 * @param path path passed by reference where the path is stored
 */
void add_to_log(const std::string& message);
/**
 * @brief print the log
 * 
 */
void print_log();