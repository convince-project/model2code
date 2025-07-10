// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file Data.cpp
 * @brief This file contains the structures used in the code generation and functions to manipulate the data.
 * @version 0.1
 * @date 2024-05-24
 * 
 */

#include "Data.h"

std::string log_str;
/**
 * @brief Get component and function data from event string (Format: componentName.functionName.eventName)
 * 
 * @param eventData event data structure passed by reference from which the event string is extracted and the component and function data are stored
 */
void getDataFromEvent(eventDataStr& eventData) 
{

    std::string firstWord, secondWord, thirdWord;
    std::string event = eventData.event;
    std::cout << "Event: " << event << std::endl;
    if (event == ""){
        std::cerr << "Event has no value" << std::endl;
        return;
    }
    size_t firstDotPos = event.find('.');
    if (firstDotPos != std::string::npos) {
        firstWord = event.substr(0, firstDotPos);
        size_t secondDotPos = event.find('.', firstDotPos + 1);
        if (secondDotPos != std::string::npos) {
            secondWord = event.substr(firstDotPos+1, secondDotPos - firstDotPos - 1);
            thirdWord = event.substr(secondDotPos + 1);
        }
        else{
            std::cerr << "only 2 words found, : "<< event << std::endl;
        }
    }
    else{
        std::cerr << "Error in name format for event: "<< event << std::endl;
    }
    eventData.componentName = firstWord;
    eventData.functionName  = secondWord;
    eventData.eventName     = thirdWord;
}

/**
 * @brief Get the data From SCXML Root Name object (Format: NameSkillType)
 * 
 * @param attributeName root attribute name from which the skill data is extracted
 * @param skillData skill data structure passed by reference where the skill data is stored
 * @return bool 
 */
bool getDataFromRootName(const std::string attributeName, skillDataStr& skillData)
{
    // e.g. attributeName = "FirstTemplateSkillAction"
    if (attributeName != ""){
        add_to_log("Root attribute name: " + attributeName);
        size_t dotPos = attributeName.find("Skill");
        if (dotPos != std::string::npos){
            skillData.SMName = attributeName; // e.g. SMName = "FirstTemplateSkillAction"
            skillData.skillName = attributeName.substr(0, dotPos); // e.g. skillName = "FirstTemplate"
            skillData.className = attributeName.substr(0, dotPos + 5); // e.g. className = "FirstTemplateSkill"
            turnToSnakeCase(skillData.className, skillData.classNameSnakeCase);
            skillData.skillType = attributeName.substr(dotPos + 5); // e.g. skillType = "Action"
            if(skillData.skillType == ""){
                std::cerr << "Skill type not found" << std::endl;
                return false;
            }
            std::cerr << "Skill type: " << skillData.skillType << std::endl;
            // convert skillType to lowercase
            skillData.skillTypeLC = skillData.skillType; 
            for (char &c : skillData.skillTypeLC) 
            { 
                c = std::tolower(c); // e.g. skillType = "action"
            } 
        }
        else{
            std::cerr << "Skill name not found" << std::endl;
            return false;
        }
    } 
    else{
        std::cerr << "Attribute 'name' not found or has no value" << std::endl;
        return false;
    }
    return true;

}

/**
 * \defgroup GenerateCode Generate code
 * Functions to generate code.
 * @{
 */

/**
 * \defgroup printData Print Data
 * \ingroup GenerateCode
 * Print Data
 * @{
 */
/**
 * @brief Prints the event data
 * 
 * @param eventData event data structure to be printed
 */
void printEventData(eventDataStr eventData)
{

    add_to_log("-----------");
    add_to_log("Event data:");
    add_to_log("\tevent=" + eventData.event + ", target=" + eventData.target);
    add_to_log("\teventName=" + eventData.eventName + ", eventType=" + eventData.eventType);
    add_to_log("\tparamMap:");
    // print the paramMap
    for (const auto& param : eventData.paramMap) {
        add_to_log("\t\t" + param.first + ": " + param.second);
    }
    add_to_log("\tcomponentName=" + eventData.componentName);
    add_to_log("\tfunctionName=" + eventData.functionName);
    add_to_log("\tfunctionNameSnakeCase=" + eventData.functionNameSnakeCase);
    add_to_log("\tnodeName=" + eventData.nodeName);
    add_to_log("\tserverName=" + eventData.serverName);
    add_to_log("\tclientName=" + eventData.clientName);
    add_to_log("\ttopicName=" + eventData.topicName);
    add_to_log("\tinterfaceName=" + eventData.interfaceName);
    add_to_log("\tinterfaceType=" + eventData.interfaceType);
    add_to_log("\tscxmlInterfaceName=" + eventData.scxmlInterfaceName);
    add_to_log("\tvirtualInterface=" + std::to_string(eventData.virtualInterface));
    add_to_log("\tinterfaceTopicFields:");
    // print the interface topic fields
    for (const auto& field : eventData.interfaceTopicFields) {
        add_to_log("\t\t" + field);
    }
    add_to_log("\tinterfaceRequestFields:");
    // print the interface request fields
    for (const auto& field : eventData.interfaceRequestFields) {
        add_to_log("\t\t" + field);
    }
    add_to_log("\tinterfaceResponseFields:");
    // print the interface response fields
    for (const auto& field : eventData.interfaceResponseFields) {
        add_to_log("\t\t" + field);
    }
    add_to_log("\tinterfaceData:");
    // print the interface data
    for (const auto& data : eventData.interfaceData) {
        add_to_log("\t\t" + data.second + " " + data.first);
    }
    add_to_log("-----------");

    
}

// equal to printEventData but prints to cerr
void printEventDataToCerr(const eventDataStr eventData)
{
    std::cerr << "-----------" << std::endl;
    std::cerr << "Event data: " << std::endl;
    std::cerr << "\tevent=" << eventData.event << ", target=" << eventData.target << std::endl;
    std::cerr << "\teventName=" << eventData.eventName << ", eventType=" << eventData.eventType << std::endl;   
    std::cerr << "\tparamMap:" << std::endl;
    // print the paramMap
    for (const auto& param : eventData.paramMap) {
        std::cerr << "\t\t" << param.first << ": " << param.second << std::endl;
    }
    std::cerr << "\tcomponentName=" << eventData.componentName << std::endl;
    std::cerr << "\tfunctionName=" << eventData.functionName << std::endl;
    std::cerr << "\tfunctionNameSnakeCase=" << eventData.functionNameSnakeCase << std::endl;
    std::cerr << "\tnodeName=" << eventData.nodeName << std::endl;
    std::cerr << "\tserverName=" << eventData.serverName << std::endl;
    std::cerr << "\tclientName=" << eventData.clientName    << std::endl;   
    std::cerr << "\ttopicName=" << eventData.topicName << std::endl;
    std::cerr << "\tinterfaceName=" << eventData.interfaceName << std::endl;
    std::cerr << "\tinterfaceType=" << eventData.interfaceType << std::endl;
    std::cerr << "\tscxmlInterfaceName=" << eventData.scxmlInterfaceName << std::endl;
    std::cerr << "\tvirtualInterface=" << std::boolalpha << eventData.virtualInterface << std::endl;
    std::cerr << "\tinterfaceTopicFields:" << std::endl;
    // print the interface topic fields
    for (const auto& field : eventData.interfaceTopicFields) {
        std::cerr << "\t\t" << field << std::endl;
    }
    std::cerr << "\tinterfaceRequestFields:" << std::endl;
    // print the interface request fields
    for (const auto& field : eventData.interfaceRequestFields) {
        std::cerr << "\t\t" << field << std::endl;
    }
    std::cerr << "\tinterfaceResponseFields:" << std::endl;
    // print the interface response fields
    for (const auto& field : eventData.interfaceResponseFields) {
        std::cerr << "\t\t" << field << std::endl;
    }
    std::cerr << "\tinterfaceData:" << std::endl;
    // print the interface data
    for (const auto& data : eventData.interfaceData) {
        std::cerr << "\t\t" << data.second << " " << data.first << std::endl;
    }
    std::cerr << "-----------" << std::endl;
}

/**
 * @brief Prints the skill data
 * 
 * @param skillData skill data structure to be printed
 */
void printSkillData(skillDataStr skillData)
{
    add_to_log("-----------");
    add_to_log("Class name: " + skillData.className + "\nSkill name: " + skillData.skillName + "\nSkill type: " + skillData.skillType);
    add_to_log("-----------");
}
/** @} */ // end of printData subgroup

/**
 * @brief Set the File Data object
 * 
 * @param fileData file data structure passed by reference where the file data is stored
 * @param skillData const skill data structure used to set the file data
 */
void setFileData(fileDataStr& fileData, const skillDataStr skillData)
{
    fileData.outputFileNameCPP = skillData.className + ".cpp";
    fileData.outputFileNameH = skillData.className + ".h";
    fileData.outputDatamodelFileNameH = skillData.className + "DataModel.h";
    fileData.outputDatamodelFileNameCPP = skillData.className + "DataModel.cpp";
    fileData.outputMainFileName = "main.cpp";
    fileData.outputCMakeListsFileName = "CMakeLists.txt";
    fileData.outputPackageXMLFileName = "package.xml";
}

/**
 * @brief Add a message to the log
 * 
 * @param fileName file name
 * @param path path passed by reference where the path is stored
 */
void add_to_log(const std::string& message) {
    log_str += message + "\n";
}
/**
 * @brief print the log
 * 
 */
void print_log() {
    std::cout << log_str;
}