// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file Translator.h
 * @brief This file contains the functions to translate the High-Level CONVINCE XML to the compilable SCXML used for code generation.
 * @version 0.1
 * @date 2024-09-18
 * 
 */
#include "Replacer.h"
#include <fstream>

/**
 * @brief Deletes a specific attribute of a given XML element
 * 
 * @param element element from which to delete the attribute
 * @param attribute attribute name to be deleted
 * @return bool
 */
bool deleteElementAttribute(tinyxml2::XMLElement* element, const std::string& attribute);

/**
 * @brief Deletes a specific attribute from all elements in a vector
 * 
 * @param elements vector of elements from which to delete the attribute
 * @param attribute attribute name to be deleted
 * @return bool Returns true if at least one attribute was successfully deleted, false otherwise
 */
bool deleteElementAttributeFromVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& attribute);

/**
 * @brief
 * 
 * @param 
 */
bool readHLXMLFile(tinyxml2::XMLDocument& doc, const std::string fileName);

/**
 * @brief
 * 
 * @param 
 */
bool Translator(fileDataStr& fileData);