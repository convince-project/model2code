/**
 * @file Data.cpp
 * @brief This file contains the structures used in the code generation and functions to manipulate the data.
 * @version 0.1
 * @date 2024-05-24
 * 
 */

#include "Data.h"

/**
 * @brief Get component and function data from event string (Format: componentName.functionName.eventName)
 * 
 * @param eventData event data structure passed by reference from which the event string is extracted and the component and function data are stored
 */
void getDataFromEvent(eventDataStr& eventData) 
{
    std::string firstWord, secondWord, thirdWord;
    std::string event = eventData.event;
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
            std::cerr << "Error in name format for event: "<< event << std::endl;
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
        std::cout << "Root attribute name: " << attributeName << std::endl;
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
    std::cout << "\tcomponent=" << eventData.componentName << ", service=" << eventData.functionName << ", eventName=" << eventData.eventName << std::endl;
    std::cout<< "\tinterface=" << eventData.interfaceName << ", type=" << eventData.interfaceType;
    for(auto it =  eventData.interfaceData.begin(); it != eventData.interfaceData.end(); ++it)
    {
        std::cout << "\n\t dataField=" << it->first << ", dataType=" << it->second;
    }
    std::cout << std::endl;
    
}

/**
 * @brief Prints the skill data
 * 
 * @param skillData skill data structure to be printed
 */
void printSkillData(skillDataStr skillData)
{
    std::cout << "-----------" << std::endl;
    std::cout << "Class name: " << skillData.className << std::endl << "Skill name: " << skillData.skillName << std::endl << "Skill type: " << skillData.skillType << std::endl;
    std::cout << "-----------" << std::endl;
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