/**
 * @file ExtractFromXML.h
 * @brief This file is the header file for the ExtractFromXML.cpp file.
 * @version 0.1
 * @date 2024-05-20
 * 
 */
#include "Data.h"
#include "ExtractFromElement.h"

bool extractInterfaceName(const std::string, eventDataStr&);

bool extractInterfaceType(const std::string, eventDataStr&);

bool extractFromSCXML(tinyxml2::XMLDocument&, const std::string, std::string&, std::vector<tinyxml2::XMLElement*>&, std::vector<tinyxml2::XMLElement*>&);
