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
 * @brief Extract the interface name from the model file
 * 
 * @param fileName model file name from which to extract the interface name
 * @param eventData event data structure, where the interface name will be stored, returned by reference
 * @return bool
 */
bool extractInterfaceName(const std::string fileName, eventDataStr& eventData);

/**
 * @brief Extract the interface data from the interface file
 * 
 * @param fileName interface file name from which to extract the interface data
 * @param eventData event data structure, where the interface data will be stored, returned by reference
 * @return true 
 * @return false 
 */
bool extractInterfaceType(const std::string fileName, eventDataStr& eventData);

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
