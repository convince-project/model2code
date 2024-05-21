/**
 * @file ExtractFromElement.h
 * @brief This file is the header file for the ExtractFromElement.cpp file.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include "tinyxml2.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

bool getElementAttValue(tinyxml2::XMLElement*, const std::string, std::string& );

bool getElementText(tinyxml2::XMLElement*, std::string& );

bool findElementByTagAndAttValue(tinyxml2::XMLElement* root, const std::string tag, const std::string, const std::string, tinyxml2::XMLElement*& );

bool findElementByTag(tinyxml2::XMLElement*, const std::string, tinyxml2::XMLElement*& );

void findElementVectorByTagAndAttribute(tinyxml2::XMLElement*, const std::string, const std::string, std::vector<tinyxml2::XMLElement*>&);

void findElementVectorByTag(tinyxml2::XMLElement*, const std::string, std::vector<tinyxml2::XMLElement*>&);

