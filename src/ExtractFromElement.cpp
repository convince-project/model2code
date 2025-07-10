// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file ExtractFromElement.cpp
 * @brief This file contains the functions to extract data from an XML element.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include "ExtractFromElement.h"

/**
 * \defgroup ExtractFromXMLFunctions Extract from XML Functions
 * Functions to get data from XML file.
 * @{
 */
/**
 * \defgroup getFromElement Get from Element
 * \ingroup ExtractFromXMLFunctions
 * Get from Element object functions
 * @{
 */
/**
 * @brief Gets the value of a specific attribute of a given XML element
 * 
 * @param element element from which to get the attribute value
 * @param attribute attribute name 
 * @param attributeValue attribute value passed by reference
 * @return bool
 */
bool getElementAttValue(tinyxml2::XMLElement* element, const std::string attribute, std::string& attributeValue)
{
    const char* value = element->Attribute(attribute.c_str());
    if (value){
        attributeValue = std::string(value);
        return true;
    }
    else{
        std::cerr << "Attribute '" << attribute << "' of element '"<< element->Value()<<"' not found or has no value" << std::endl;
        return false;
    }
}

/**
 * @brief Gets the text of a given XML element
 * 
 * @param element element from which to get the text
 * @param textValue text value passed by reference
 * @return bool 
 */
bool getElementText(tinyxml2::XMLElement* element, std::string& textValue)
{
    const char* text = element->GetText();
    if (text){
        textValue = std::string(text);
        return true;
    }
    else{
        std::cerr << "Text of element '"<< element->Value()<<"' not found or has no value" << std::endl;
        return false;
    }
}
/** @} */ // end of getFromElement subgroup
/**
 * \defgroup findElements Find Elements
 * \ingroup ExtractFromXMLFunctions
 * Find Elements functions
 * @{
 */

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
bool findElementByTagAndAttValue(tinyxml2::XMLElement* root, const std::string tag, const std::string attributeName, const std::string attributeValue, tinyxml2::XMLElement*& element)
{

    for (tinyxml2::XMLElement* child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
        if (strcmp(child->Value(), tag.c_str()) == 0) {
            const char* id = child->Attribute(attributeName.c_str());
            if (id && std::string(id) == attributeValue){
                std::cerr << "Found element with tag '" << tag << "' and attribute '" << attributeName << "' with value '" << attributeValue << "'" << std::endl;
                //std::cout << "Found element with tag '" << tag << "' and attribute '"
                element = child;
                return true;
            }
        }
        if (findElementByTagAndAttValue(child, tag, attributeName, attributeValue, element)) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Find a XML element by tag
 * 
 * @param root root element from which to start the search
 * @param tag tag name to look for
 * @param element element found returned by reference
 * @return true 
 * @return false 
 */
bool findElementByTag(tinyxml2::XMLElement* root, const std::string tag, tinyxml2::XMLElement*& element)
{
    for (tinyxml2::XMLElement* child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
        if (strcmp(child->Value(), tag.c_str()) == 0){
            element = child;
            return true;
        }
        if (findElementByTag(child, tag, element)) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Find a vector of XML elements by tag and attribute name
 * 
 * @param root root element from which to start the search
 * @param tag tag name to look for
 * @param attribute attribute name to look for inside the tag
 * @param elementVector element vector containing the elements found 
 */
void findElementVectorByTagAndAttribute(tinyxml2::XMLElement* root, const std::string tag, const std::string attribute, std::vector<tinyxml2::XMLElement*>& elementVector)
{
    for (tinyxml2::XMLElement* child = root->FirstChildElement(); child; child = child->NextSiblingElement()){
        if (strcmp(child->Value(), tag.c_str()) == 0) {
            const char* childAttribute = child->Attribute(attribute.c_str());
            if (childAttribute) {
                elementVector.push_back(child);
            }
        }
        findElementVectorByTagAndAttribute(child, tag, attribute, elementVector);
    }
}

/**
 * @brief Find a vector of XML elements by tag
 * 
 * @param root root element from which to start the search
 * @param tag tag name to look for
 * @param elementVector element vector containing the elements found
 */
void findElementVectorByTag(tinyxml2::XMLElement* root, const std::string tag, std::vector<tinyxml2::XMLElement*>& elementVector)
{
    for (tinyxml2::XMLElement* child = root->FirstChildElement(); child; child = child->NextSiblingElement()){
        if (strcmp(child->Value(), tag.c_str()) == 0) {
            elementVector.push_back(child);
        }
        findElementVectorByTag(child, tag, elementVector);
    }
}
/** @} */ // end of findElements subgroup