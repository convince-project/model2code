/**
 * @file ExtractFromXML.h
 * @brief This file is the header file for the ExtractFromXML.cpp file.
 * @version 0.1
 * @date 2024-05-20
 * 
 */
#include "Data.h"
#include "ExtractFromElement.h"

/**
 * @brief Extract the interface name from the model file
 * 
 * @param fileName 
 * @param eventData 
 * @return bool
 */
bool extractInterfaceName(const std::string fileName, eventDataStr& eventData);

/**
 * @brief Extract the interface data from the interface file
 * 
 * @param fileName 
 * @param eventData 
 * @return true 
 * @return false 
 */
bool extractInterfaceType(const std::string fileName, eventDataStr& eventData);

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
bool extractFromSCXML(tinyxml2::XMLDocument& doc, const std::string fileName, std::string& rootName, std::vector<tinyxml2::XMLElement*>& elementsTransition, std::vector<tinyxml2::XMLElement*>& elementsSend); 
