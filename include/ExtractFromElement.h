// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

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
 * @param element element from which to get the attribute value
 * @param attribute attribute name 
 * @param attributeValue attribute value passed by reference
 * @return bool
 */
bool getElementAttValue(tinyxml2::XMLElement* element, const std::string attribute, std::string& attributeValue);

/**
 * @brief Gets the text of a given XML element
 * 
 * @param element element from which to get the text
 * @param textValue text value passed by reference
 * @return bool 
 */
bool getElementText(tinyxml2::XMLElement* element, std::string& textValue);

/**
 * @brief Find a XML element by tag and attribute name and value
 * 
 * @param root root element from which to start the search
 * @param tag tag name to look for
 * @param attributeName attribute name to look for inside the tag
 * @param attributeValue attribute value associated with the attribute name to look for
 * @param element element found returned by reference
 * @return true 
 * @return false 
 */
bool findElementByTagAndAttValue(tinyxml2::XMLElement* root, const std::string tag, const std::string attributeName, const std::string attributeValue, tinyxml2::XMLElement*& element);

/**
 * @brief Find a XML element by tag
 * 
 * @param root root element from which to start the search
 * @param tag tag name to look for
 * @param element element found returned by reference
 * @return true 
 * @return false 
 */
bool findElementByTag(tinyxml2::XMLElement* root, const std::string tag, tinyxml2::XMLElement*& element);

/**
 * @brief Find a vector of XML elements by tag and attribute name
 * 
 * @param root root element from which to start the search
 * @param tag tag name to look for
 * @param attribute attribute name to look for inside the tag
 * @param elementVector element vector containing the elements found 
 */
void findElementVectorByTagAndAttribute(tinyxml2::XMLElement* root, const std::string tag, const std::string attribute, std::vector<tinyxml2::XMLElement*>& elementVector);

/**
 * @brief Find a vector of XML elements by tag
 * 
 * @param root root element from which to start the search
 * @param tag tag name to look for
 * @param elementVector element vector containing the elements found
 */
void findElementVectorByTag(tinyxml2::XMLElement* root, const std::string tag, std::vector<tinyxml2::XMLElement*>& elementVector);

