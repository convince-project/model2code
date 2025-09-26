// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

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
 * @brief  
 * 
 * @param fileData 
 * @param eventData 
 * @return true 
 * @return false 
 */
bool extractInterfaceData(const fileDataStr fileData, eventDataStr& eventData);


/**
 * @brief 
 * 
 * @param fileData 
 * @param eventData 
 * @param element 
 * @param rosInterfaceType 
 * @return true 
 * @return false 
 */
 bool findInterfaceType(const fileDataStr fileData, eventDataStr& eventData, tinyxml2::XMLElement*& element);

/**
 * @brief Get the Interface Fields From Name object
 * 
 * @param element 
 * @param interfaceFields 
 * @return true 
 * @return false 
 */
bool getInterfaceFieldsFromAssignTag(tinyxml2::XMLElement* element, std::vector<std::string>& interfaceFields);

/**
 * @brief Get the Interface Fields From Expr object
 * 
 * @param element 
 * @param interfaceFields 
 * @return true 
 * @return false 
 */
bool getInterfaceFieldsFromFieldTag(tinyxml2::XMLElement* element, std::vector<std::string>& interfaceFields);


/**
 * @brief Get the Event Data Fields object  
 * 
 * @param elementFunction 
 * @param eventData 
 * @param interfaceDataField
 * @param interfaceDataType
 * @return true 
 * @return false 
 */
bool getEventDataFields(tinyxml2::XMLElement * elementFunction, eventDataStr& eventData, std::string interfaceDataField, std::string interfaceDataType);


/**
 * @brief Extract data from SCXML file (rootname, transition and send event elements)
 * 
 * @param doc SCXML document object
 * @param fileName file name of the SCXML file
 * @param rootName name value of the root element of the SCXML file
 * @param elementsTransition vector of transition event elements found in the SCXML file
 * @param elementsSend vector of send event elements found in the SCXML file
 * @return true 
 * @return false 
 */
bool extractFromSCXML(tinyxml2::XMLDocument& doc, const std::string fileName, std::string& rootName, std::vector<tinyxml2::XMLElement*>& elementsTransition, std::vector<tinyxml2::XMLElement*>& elementsSend); 
