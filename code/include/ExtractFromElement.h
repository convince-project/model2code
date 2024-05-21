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

/**
 * @brief Gets the value of a specific attribute of a given XML element
 * 
 * @param element 
 * @param attribute 
 * @param attributeValue 
 * @return bool
 */
bool getElementAttValue(tinyxml2::XMLElement* element, const std::string attribute, std::string& attributeValue);

/**
 * @brief Gets the text of a given XML element
 * 
 * @param element 
 * @param textValue 
 * @return bool 
 */
bool getElementText(tinyxml2::XMLElement* element, std::string& textValue);

/**
 * @brief Find a XML element by tag and attribute name and value
 * 
 * @param root 
 * @param tag 
 * @param attributeName 
 * @param attributeValue 
 * @param element 
 * @return true 
 * @return false 
 */
bool findElementByTagAndAttValue(tinyxml2::XMLElement* root, const std::string tag, const std::string attributeName, const std::string attributeValue, tinyxml2::XMLElement*& element);

/**
 * @brief Find a XML element by tag
 * 
 * @param root 
 * @param tag 
 * @param element 
 * @return true 
 * @return false 
 */
bool findElementByTag(tinyxml2::XMLElement* root, const std::string tag, tinyxml2::XMLElement*& element);

/**
 * @brief Find a vector of XML elements by tag and attribute name
 * 
 * @param root 
 * @param tag 
 * @param attribute 
 * @param elementVector 
 */
void findElementVectorByTagAndAttribute(tinyxml2::XMLElement* root, const std::string tag, const std::string attribute, std::vector<tinyxml2::XMLElement*>& elementVector);

/**
 * @brief Find a vector of XML elements by tag
 * 
 * @param root 
 * @param tag 
 * @param elementVector 
 */
void findElementVectorByTag(tinyxml2::XMLElement* root, const std::string tag, std::vector<tinyxml2::XMLElement*>& elementVector);

