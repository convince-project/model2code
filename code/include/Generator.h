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


void getDataFromEvent(eventDataStr&);

bool getDataFromRootName(const std::string, skillDataStr&);

void printEventData(eventDataStr);

void printSkillData(skillDataStr);

void processEvent(fileDataStr fileData, eventDataStr, const skillDataStr, std::string, hCodeStr&, cppCodeStr&, topicCodeStr&);

void getEventsCode(fileDataStr fileData, const std::vector<tinyxml2::XMLElement*>, const std::vector<tinyxml2::XMLElement*>, skillDataStr, hCodeStr&, cppCodeStr&, topicCodeStr&);

void writeHCode(const skillDataStr, hCodeStr&, bool);

void writeCppCode(const skillDataStr, cppCodeStr&, bool);

void writeDataModelHCode(const skillDataStr, hCodeStr&);

void writeDataModelCppCode(const skillDataStr, cppDataModelCodeStr&);

void generateDataModelHFile(const std::string, const std::string, hCodeStr);

void generateDataModelCppFile(const std::string, const std::string, cppDataModelCodeStr);

void generateHFile(const std::string, const std::string, const skillDataStr, hCodeStr);

void generateCppFile(const std::string, const std::string, const skillDataStr, cppCodeStr);

void print_help();

bool generator(fileDataStr fileData);






