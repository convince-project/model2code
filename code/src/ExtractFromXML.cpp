/*
 * SPDX-FileCopyrightText: 2024-2024 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file ExtractFromXML.cpp
 * @brief This file contains the functions to extract data from custom XML files.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include "ExtractFromXML.h"


/**
 * \defgroup readFromXMLFiles Read from custom XML Files
 * \ingroup ExtractFromXMLFunctions
 * Read from custom XML Files
 * @{
 */

/**
 * @brief Extract the interface name from the model file
 * 
 * @param fileName 
 * @param eventData 
 * @return bool
 */
bool extractInterfaceName(const std::string fileName, eventDataStr& eventData)
{
    const std::string componentName = eventData.componentName;
    std::string interfaceName;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load '" << fileName << "' file" << std::endl;
        return false;
    }
 
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found" << std::endl;
        return false;
    }
    
    tinyxml2::XMLElement* element;
    if(!findElementByTagAndAttValue(root, std::string("componentDeclaration"), std::string("id"), componentName, element)){
        std::cerr << "\nNo component '" << componentName << "'found in file '" << fileName << "'."<< std::endl;
        return false;
    }
    if (!getElementAttValue(element, std::string("interface"), interfaceName)){
        std::cerr << "\nNo interface found for component '" << componentName << "'."<< std::endl;
        return false;
    }
    eventData.interfaceName = interfaceName;

    return true;  
}
/**
 * @brief Extract the interface data from the interface file
 * 
 * @param fileName 
 * @param eventData 
 * @return true 
 * @return false 
 */
bool extractInterfaceType(const std::string fileName, eventDataStr& eventData)
{
    const std::string interfaceName = eventData.interfaceName;
    const std::string functionName = eventData.functionName;
    std::string interfaceType, interfaceDataType, interfaceDataField;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load '" << fileName << "' file" << std::endl;
        return false;
    }
 
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found" << std::endl;
        return false;
    }

    std::string idValue;
    tinyxml2::XMLElement* elementInterface, *elementInterfaceType, *elementFunction, *elementDataType, *elementDataField;
    if (!findElementByTagAndAttValue(root, std::string("interface"), std::string("id"), interfaceName, elementInterface))
    {
        std::cerr << "\nNo interface '" << interfaceName << "'found in file '" << fileName << "'."<< std::endl;
        return false;
    }
    if(!findElementByTagAndAttValue(elementInterface, std::string("function"), std::string("id"), functionName, elementFunction))
    {
        std::cerr << "No function '" << functionName << "'found in file '" << fileName << "'."<< std::endl;
        return false;
    }
    if(!findElementByTag(elementFunction, std::string("interface"), elementInterfaceType))
    {
        return false;
    }
    if(!getElementAttValue(elementInterfaceType, std::string("type"), interfaceType))
    {
        return false;
    }
    eventData.interfaceType = interfaceType;
    if(!findElementByTag(elementFunction, std::string("dataType"), elementDataType))
    {   
        std::cerr << "No tag <dataType> for function '" << functionName << "'."<< std::endl;
        //Not every interface has a defined DataType TODO
        return true;
    } 
    if(!getElementText(elementDataType, interfaceDataType))
    {
        std::cerr << "No value in tag <dataType> for function '" << functionName << "'."<< std::endl;
        return true;
    }
    if(!findElementByTag(elementFunction, std::string("dataField"), elementDataField))
    {   
        std::cerr << "No tag <dataField> for function '" << functionName << "'."<< std::endl;
        return true;
    } 
    if(!getElementText(elementDataField, interfaceDataField))
    {
        std::cerr << "No value in tag <dataField> for function '" << functionName << "'."<< std::endl;
        return true;
    }
    eventData.interfaceDataType = interfaceDataType;
    eventData.interfaceDataField = interfaceDataField;
    return true;
}

/**
 * @brief Extract data from SCXML file
 * 
 * @param doc 
 * @param fileName 
 * @param rootName 
 * @param elementsTransition 
 * @param elementsSend 
 * @return true 
 * @return false 
 */
bool extractFromSCXML(tinyxml2::XMLDocument& doc, const std::string fileName, std::string& rootName, std::vector<tinyxml2::XMLElement*>& elementsTransition, std::vector<tinyxml2::XMLElement*>& elementsSend) 
{
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load '" << fileName << "' file" << std::endl;
        return false;
    }
 
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found" << std::endl;
        return false;
    }
    rootName = std::string(root->Attribute("name")); 
    if(rootName == std::string(""))
    {
        std::cerr << "No root name found" << std::endl;
        return false;
    }
      
    // Get transitions
    findElementVectorByTagAndAttribute(root, std::string("transition"), "event", elementsTransition);
    if (elementsTransition.empty()) {
        std::cout << "No transition elements found." << std::endl;
    } 
    else 
    {
        std::cout << "Transition elements found." << std::endl;
    }

    // Get Send
    findElementVectorByTagAndAttribute(root, std::string("send"), "event", elementsSend);
    if (elementsSend.empty()) {
        std::cout << "No Send elements found." << std::endl;
    } 
    else 
    {
        std::cout << "Send elements found." << std::endl;
    }
    return true;
}
/** @} */ // end of readFromXMLFiles subgroup
/** @} */ // end of ExtractFromXMLFunctions group