// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file Translator.cpp
 * @brief This file contains the functions to translate the High-Level CONVINCE XML to the compilable SCXML used for code generation.
 * @version 0.1
 * @date 2024-09-18
 * 
 */
#include "Translator.h"

#define m2cVersion M2C_VERSION

/**
 * @brief Deletes a specific attribute of a given XML element
 * 
 * @param element element from which to delete the attribute
 * @param attribute attribute name to be deleted
 * @return bool
 */
bool deleteElementAttribute(tinyxml2::XMLElement* element, const std::string& attribute)
{
    if (element->Attribute(attribute.c_str())) {
        
        element->DeleteAttribute(attribute.c_str());
        return true;
    } else {
        std::cerr << "Attribute '" << attribute << "' of element '" << element->Value() << "' not found." << std::endl;
        return false;
    }
}

/**
 * @brief Deletes a specific attribute from all elements in a vector
 * 
 * @param elements vector of elements from which to delete the attribute
 * @param attribute attribute name to be deleted
 * @return bool Returns true if at least one attribute was successfully deleted, false otherwise
 */
bool deleteElementAttributeFromVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& attribute)
{
    bool deleted = false;

    for (auto& element : elements) {
        deleted = deleteElementAttribute(element, attribute);
    }

    return deleted;
}

/**
 * @brief Deletes the given XML element from its parent
 * 
 * @param element the XML element to be deleted
 * @return bool Returns true if the element was successfully deleted, false otherwise
 */
bool deleteElement(tinyxml2::XMLElement* element)
{
    if (!element) {
        std::cerr << "Invalid element." << std::endl;
        return false;
    }

    tinyxml2::XMLNode* parent = element->Parent();
    if (!parent) {
        std::cerr << "Element has no parent." << std::endl;
        return false;
    }

    // Remove the element from its parent
    parent->DeleteChild(element);

    return true;
}

/**
 * @brief Deletes all elements in a vector
 * 
 * @param elements vector of elements to delete 
 * @return bool Returns true if at least one element was successfully deleted, false otherwise
 */
bool deleteElementFromVector(std::vector<tinyxml2::XMLElement*>& elements)
{
    // Check if the vector itself is empty
    if (elements.empty()) {
        std::cerr << "The vector is empty In function deleteElementFromVector." << std::endl;
        return false;
    }

    bool deletedAny = false;

    for (auto& element : elements) {
        if (element) {  // Check if the element pointer is valid
            if (deleteElement(element)) {
                deletedAny = true;
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return deletedAny;
}



/**
 * @brief Replaces the tag name of a given XML element
 * 
 * @param doc XMLDocument to which the element belongs
 * @param element the element whose tag name needs to be changed
 * @param newTagName the new tag name
 * @return bool Returns true if the tag name was successfully changed
 */
bool replaceTagName(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, const std::string& newTagName)
{
    if (!element || !doc) {
        std::cerr << "Invalid document or element." << std::endl;
        return false;
    }

    // Create a new element with the new tag name
    tinyxml2::XMLElement* newElement = doc->NewElement(newTagName.c_str());

    // Copy attributes from the old element to the new element
    for (const tinyxml2::XMLAttribute* attr = element->FirstAttribute(); attr; attr = attr->Next()) {
        newElement->SetAttribute(attr->Name(), attr->Value());
    }

    // Copy the text content of the old element to the new element
    if (element->GetText()) {
        newElement->SetText(element->GetText());
    }

    // Move child elements from the old element to the new element
    for (tinyxml2::XMLElement* child = element->FirstChildElement(); child; ) {
        tinyxml2::XMLElement* next = child->NextSiblingElement();
        newElement->InsertEndChild(child);
        child = next;
    }

    // Replace the old element with the new element in the document
    element->Parent()->InsertAfterChild(element, newElement);
    element->Parent()->DeleteChild(element);

    return true;
}


/**
 * @brief Replaces an attribute name of a given XML element
 * 
 * @param element the element whose attribute name needs to be changed
 * @param oldAttributeName the current attribute name to be replaced
 * @param newAttributeName the new attribute name
 * @return bool Returns true if the attribute name was successfully changed
 */
bool replaceAttributeName(tinyxml2::XMLElement* element, const std::string& oldAttributeName, const std::string& newAttributeName)
{
    if (!element) {
        std::cerr << "Invalid element." << std::endl;
        return false;
    }

    // Get the value of the old attribute
    const char* attributeValue = element->Attribute(oldAttributeName.c_str());
    if (!attributeValue) {
        std::cerr << "Attribute '" << oldAttributeName << "' not found in element '" << element->Value() << "'" << std::endl;
        return false;
    }

    // Remove the old attribute
    element->DeleteAttribute(oldAttributeName.c_str());

    // Set the new attribute with the same value
    element->SetAttribute(newAttributeName.c_str(), attributeValue);

    return true;
}


/**
 * @brief Replaces the value of a given attribute in an XML element
 * 
 * @param element the element whose attribute value needs to be changed
 * @param attributeName the name of the attribute to be updated
 * @param newValue the new value to set for the attribute
 * @return bool Returns true if the attribute value was successfully changed, false if the attribute doesn't exist
 */
bool replaceAttributeValue(tinyxml2::XMLElement* element, const std::string& attributeName, const std::string& newValue)
{
    if (!element) {
        std::cerr << "Invalid element." << std::endl;
        return false;
    }

    // Check if the attribute exists
    const char* attributeValue = element->Attribute(attributeName.c_str());
    if (!attributeValue) {
        std::cerr << "Attribute '" << attributeName << "' not found in element '" << element->Value() << "'" << std::endl;
        return false;
    }

    // Set the new value for the attribute
    element->SetAttribute(attributeName.c_str(), newValue.c_str());

    return true;
}


/**
 * @brief Replaces the tag name of all elements in a vector
 * 
 * @param doc XMLDocument to which the elements belong
 * @param elements vector of elements whose tag names need to be replaced
 * @param newTagName the new tag name to set
 * @return bool Returns true if at least one tag name was successfully changed, false otherwise
 */
bool replaceTagNameFromVector(tinyxml2::XMLDocument* doc, std::vector<tinyxml2::XMLElement*>& elements, const std::string& newTagName)
{
    bool replacedAny = false;

    for (auto& element : elements) {
        if (element) {
            if (replaceTagName(doc, element, newTagName)) {
                replacedAny = true;
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return replacedAny;
}

/**
 * @brief Replaces an attribute name in all elements in a vector
 * 
 * @param elements vector of elements in which to replace the attribute name
 * @param oldAttributeName the current attribute name to be replaced
 * @param newAttributeName the new attribute name
 * @return bool Returns true if at least one attribute name was successfully replaced, false otherwise
 */
bool replaceAttributeNameFromVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& oldAttributeName, const std::string& newAttributeName)
{
    bool replacedAny = false;

    for (auto& element : elements) {
        if (element) {
            if (replaceAttributeName(element, oldAttributeName, newAttributeName)) {
                replacedAny = true;
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return replacedAny;
}

/**
 * @brief Replaces the value of a given attribute in all elements in a vector
 * 
 * @param elements vector of elements in which to replace the attribute value
 * @param attributeName the name of the attribute to be updated
 * @param newValue the new value to set for the attribute
 * @return bool Returns true if at least one attribute value was successfully replaced, false otherwise
 */
bool replaceAttributeValueFromVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& attributeName, const std::string& newValue)
{
    bool replacedAny = false;

    for (auto& element : elements) {
        if (element) {
            if (replaceAttributeValue(element, attributeName, newValue)) {
                replacedAny = true;
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return replacedAny;
}


/**
 * @brief Replaces the value of a given attribute in all elements in a vector if the current value contains a specific substring
 * 
 * @param elements vector of elements in which to replace the attribute value
 * @param attributeName the name of the attribute to be updated
 * @param oldSubstring the substring to look for within the attribute value
 * @param newValue the new value to set for the attribute if the substring is found
 * @return bool Returns true if at least one attribute value was successfully replaced, false otherwise
 */
bool replaceAttributeValueContaingFromVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& attributeName, const std::string& oldSubstring, const std::string& newValue)
{
    bool replacedAny = false;
    for (auto& element : elements) {
        if (element) {
            const char* attributeValue = element->Attribute(attributeName.c_str());
            if (attributeValue) {
                std::string attributeStr(attributeValue);
                if (attributeStr.find(oldSubstring) != std::string::npos) {
                    if (replaceAttributeValue(element, attributeName, newValue)) {
                        replacedAny = true;
                    }
                }
            } else {
                std::cerr << "Attribute '" << attributeName << "' not found in element." << std::endl;
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return replacedAny;
}

/**
 * @brief Replaces a substring within the value of a given attribute in all elements in a vector if the current value contains a specific substring
 * 
 * @param elements vector of elements in which to replace the attribute value
 * @param attributeName the name of the attribute to be updated
 * @param oldSubstring the substring to look for within the attribute value
 * @param newSubstring the new substring to replace the old substring with
 * @return bool Returns true if at least one attribute value was successfully updated, false otherwise
 */
bool replaceAttributeValueSubstringFromVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& attributeName, const std::string& oldSubstring, const std::string& newSubstring)
{
    bool replacedAny = false;

    for (auto& element : elements) {
        if (element) {
            const char* attributeValue = element->Attribute(attributeName.c_str());
            if (attributeValue) {
                std::string attributeStr(attributeValue);
                // Check if the attribute value contains the substring
                size_t pos = attributeStr.find(oldSubstring);
                if (pos != std::string::npos) {
                    // Replace the old substring with the new substring
                    attributeStr.replace(pos, oldSubstring.length(), newSubstring);
                    // Set the updated value for the attribute
                    element->SetAttribute(attributeName.c_str(), attributeStr.c_str());
                    replacedAny = true;
                }
            } else {
                std::cerr << "Attribute '" << attributeName << "' not found in element." << std::endl;
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return replacedAny;
}



/**
 * @brief Appends a substring to the end of the value of a given attribute in all elements in a vector
 * 
 * @param elements vector of elements in which to append the attribute value
 * @param attributeName the name of the attribute to be updated
 * @param valueToAppend the substring to append to the attribute value
 * @return bool Returns true if at least one attribute value was successfully updated, false otherwise
 */
bool appendAttributeValueFromVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& attributeName, const std::string& valueToAppend)
{
    bool updatedAny = false;

    for (auto& element : elements) {
        if (element) {
            // Get the current attribute value
            const char* attributeValue = element->Attribute(attributeName.c_str());
            if (attributeValue) {
                std::string attributeStr(attributeValue);
                // Append the new value to the end of the current value
                attributeStr.append(valueToAppend);
                // Set the updated value for the attribute
                element->SetAttribute(attributeName.c_str(), attributeStr.c_str());
                updatedAny = true;
            } else {
                std::cerr << "Attribute '" << attributeName << "' not found in element." << std::endl;
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return updatedAny;
}

/**
 * @brief Appends a substring saved in the location attribute to the end of the value of a given attribute in all elements in a vector
 * 
 * @param elements vector of elements in which to append the attribute value
 * @param attributeName the name of the attribute to be updated
 * @param valueToAppend the substring to append to the attribute value
 * @return bool Returns true if at least one attribute value was successfully updated, false otherwise
 */
bool appendAttributeValueLocationFromVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& attributeName)
{
    bool updatedAny = false;

    for (auto& element : elements) {
        if (element) {
            // Get the current attribute value
            const char* attributeValue = element->Attribute(attributeName.c_str());
            // Get the location attribute value
            const char* locationValue = element->Attribute("location");

            if (attributeValue && locationValue) {
                std::string attributeStr(attributeValue);
                // Append the location value to the end of the current attribute value
                attributeStr.append(locationValue);
                // Set the updated value for the attribute
                element->SetAttribute(attributeName.c_str(), attributeStr.c_str());
                updatedAny = true;
            } else {
                if (!attributeValue) {
                    std::cerr << "Attribute '" << attributeName << "' not found in element." << std::endl;
                }
                if (!locationValue) {
                    std::cerr << "Location attribute not found in element." << std::endl;
                }
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return updatedAny;
}

/**
 * @brief Appends a substring to the end of the value of a given attribute in all elements in a vector
 * only if the attribute value contains a specific substring.
 * 
 * @param elements vector of elements in which to append the attribute value
 * @param attributeName the name of the attribute to be updated
 * @param oldSubstring the substring that must be present in the attribute value
 * @param valueToAppend the substring to append to the attribute value
 * @return bool Returns true if at least one attribute value was successfully updated, false otherwise
 */
bool appendIfContainsSubstringVector(std::vector<tinyxml2::XMLElement*>& elements, const std::string& attributeName, const std::string& oldSubstring, const std::string& valueToAppend)
{
    bool updatedAny = false;

    for (auto& element : elements) {
        if (element) {
            // Get the current attribute value
            const char* attributeValue = element->Attribute(attributeName.c_str());
            const char* locationValue = element->Attribute("location");
            if (attributeValue && locationValue) {
                std::string attributeStr(attributeValue);
                // Check if the attribute value contains the old substring
                if (attributeStr.find(oldSubstring) != std::string::npos) {
                    // Append the new value to the end of the current value
                    attributeStr.append(valueToAppend);
                    attributeStr.append(locationValue);
                    // Set the updated value for the attribute
                    element->SetAttribute(attributeName.c_str(), attributeStr.c_str());
                    updatedAny = true;
                }
            } else {
                std::cerr << "Attribute or location value'" << attributeName << "' not found in element." << std::endl;
            }
        } else {
            std::cerr << "Encountered a null element pointer in the vector." << std::endl;
        }
    }

    return updatedAny;
}


/**
 * @brief Find a XML element by tag and attribute name where the attribute's value contains a specific substring
 * 
 * @param root root element from which to start the search
 * @param tag tag name to look for
 * @param attributeName attribute name to look for inside the tag
 * @param substring substring to check for in the attribute value
 * @param element element found returned by reference
 * @return true if an element with the tag and attribute containing the substring is found
 * @return false otherwise
 */
bool findElementByTagAndAttValueContaining(tinyxml2::XMLElement* root, const std::string& tag, const std::string& attributeName, const std::string& substring, tinyxml2::XMLElement*& element)
{
    for (tinyxml2::XMLElement* child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
        if (strcmp(child->Value(), tag.c_str()) == 0) {
            const char* attributeValue = child->Attribute(attributeName.c_str());
            if (attributeValue && std::string(attributeValue).find(substring) != std::string::npos) {
                element = child;
                return true;
            }
        }
        if (findElementByTagAndAttValueContaining(child, tag, attributeName, substring, element)) {
            return true;
        }
    }
    return false;
}

/**
 * @brief replaces the value of the event attribute in all elements in a vector from / to .
 * 
 * @param elements vector of elements in which to replace the event value
 */
void replaceEventValueFromVectorFromSlashToPoint(std::vector<tinyxml2::XMLElement*>& elements)
{
    std::string eventName;
    for (auto& element : elements) 
    {
        getElementAttValue(element, "event", eventName);
        std::string temp = eventName;
        std::cout << "Event before replacement: " << temp << std::endl;
        size_t pos = 0;
        if (temp == ""){
            std::cerr << "Input DataType has no value" << std::endl;
        }
        else if(temp.find("/") == std::string::npos){
            std::cerr << "Input DataType has no / to replace" << std::endl;
        }
        if(temp.find("/") == 0){
            std::cerr << "Input DataType starts with /, removing it" << std::endl;
            temp.replace(0, 1, "");
            pos = 0;
        }

        // Replace all occurrences of '/' with '.'
        while ((pos = temp.find("/")) != std::string::npos) {
            temp.replace(pos, 1, ".");
            pos += 1; // Move past the replaced character
        }
        
        // else{
        //     if((pos = temp.find("/", pos)) != std::string::npos)
        //     {
        //         temp.replace(pos, 1, "");
        //         pos += 1;
        //     }
        //     if((pos = temp.find("/", pos)) != std::string::npos)
        //     {
        //         temp.replace(pos, 1, ".");
        //         pos += 1;
        //     }
        // }
        std::cout << "Event after replacement: " << temp << std::endl;
        replaceAttributeValue(element, "event", temp);
    }
}


/**
 * @brief Get the data From SCXML Root Name object (Format: NameSkillType)
 * 
 * @param attributeName root attribute name from which the skill data is extracted
 * @param skillData skill data structure passed by reference where the skill data is stored
 * @return bool 
 */
bool getDataFromRootNameHighLevel(const std::string attributeName, skillDataStr& skillData)
{
    // e.g. attributeName = "FirstTemplateSkill"
    if (attributeName != ""){
        add_to_log("Root attribute name: " + attributeName);
        size_t dotPos = attributeName.find("Skill");
        if (dotPos != std::string::npos){
            skillData.SMName = attributeName; // e.g. SMName = "FirstTemplateSkill" +"Action" added later
            skillData.skillName = attributeName.substr(0, dotPos); // e.g. skillName = "FirstTemplate"
            skillData.className = attributeName.substr(0, dotPos + 5); // e.g. className = "FirstTemplateSkill"
            turnToSnakeCase(skillData.className, skillData.classNameSnakeCase);
        }
        else{
            std::cerr << "Skill name not found" << std::endl;
            return false;
        }
    } 
    else{
        std::cerr << "Attribute 'name' not found or has no value" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief read the high level xml file
 * 
 * @param doc XMLDocument to load the file
 * @param fileContent string to store the file content
 * @param fileName file name to read
 * @return true if the file is read successfully
 */
bool readHLXMLFile(tinyxml2::XMLDocument& doc, std::string& fileContent, const std::string fileName){
    add_to_log("readFile");
    add_to_log("opening file: " + fileName + " at line " + std::to_string(__LINE__));
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load '" << fileName << "' file" << std::endl;
        return false;
    }
    if(!readTemplateFile(fileName, fileContent)){
        std::cerr << "Failed to read '" << fileName << "' file" << std::endl;
        return false;
    }
    return true;
}

void insertElementAfter(tinyxml2::XMLElement* parent, tinyxml2::XMLElement* referenceElement, tinyxml2::XMLElement* newElement) {
    if (parent && referenceElement && newElement) {
        parent->InsertAfterChild(referenceElement, newElement);
    } else {
        std::cerr << "Errore: parent, referenceElement o newElement è nullo.\n";
    }
}

/**
 * @brief Adds log entries to each state's onentry element
 * 
 * @param doc XMLDocument to create new elements
 * @param root root element from which to start the search
 * @return true if log entries were successfully added
 */
bool addLogToStateEntries(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* root)
{
    std::vector<tinyxml2::XMLElement*> stateVector;
    findElementVectorByTag(root, std::string("state"), stateVector);
    
    for (auto& element : stateVector) {
        tinyxml2::XMLElement* onEntryElement = element->FirstChildElement("onentry");
        if (onEntryElement) {
            tinyxml2::XMLElement* logElement = doc->NewElement("log");
            std::string logMessage = "Entering state: " + std::string(element->Attribute("id"));
            logElement->SetAttribute("label", logMessage.c_str());
            onEntryElement->InsertFirstChild(logElement);
        }
        else {
            tinyxml2::XMLElement* newOnEntryElement = doc->NewElement("onentry");
            tinyxml2::XMLElement* logElement = doc->NewElement("log");
            std::string logMessage = "Entering state: " + std::string(element->Attribute("id"));
            logElement->SetAttribute("label", logMessage.c_str());
            newOnEntryElement->InsertFirstChild(logElement);
            element->InsertFirstChild(newOnEntryElement);
        }
    }
    
    return true;
}

bool translateRosActionHandleGoalResponseTag(tinyxml2::XMLElement* root, std::map<std::string, std::string> nameToActionNameMap)
{
    std::vector<tinyxml2::XMLElement*> actionHandleGoalRspVector;
    findElementVectorByTag(root, std::string("ros_action_handle_goal_response"), actionHandleGoalRspVector);

    if (actionHandleGoalRspVector.empty()) {
        std::cerr << "The vector is empty in function translateRosActionHandleGoalResponseTag." << std::endl;
        return false; 
    }

    replaceAttributeNameFromVector(actionHandleGoalRspVector, "name", "event");
    for(auto& element : actionHandleGoalRspVector){
        const char* eventValue = element->Attribute("event");
        if (eventValue) {
            std::string eventStr(eventValue);
            auto it = nameToActionNameMap.find(eventStr);
            if (it != nameToActionNameMap.end()) {
                // Replacing the event value with the action name
                element->SetAttribute("event", it->second.c_str());
                add_to_log("Updating: " + eventStr + " -> " + it->second);
            } else {
                add_to_log("No update for: " + eventStr);
            }
        } else {
            add_to_log("No 'event' attribute found");
        }
        replaceEventValueFromVectorFromSlashToPoint(actionHandleGoalRspVector);
            appendAttributeValueFromVector(actionHandleGoalRspVector, "event", ".GoalResponse");
        const char* newEventValue = element->Attribute("event");
        const char* acceptState = element->Attribute("accept");
        const char* rejectState = element->Attribute("reject");
        if (newEventValue && acceptState && rejectState) {
            std::string eventStr(newEventValue);

            // Crea il nodo <transition> per "accept"
            tinyxml2::XMLElement* acceptTransition = element->GetDocument()->NewElement("transition");
            acceptTransition->SetAttribute("event", (eventStr ).c_str());
            acceptTransition->SetAttribute("cond", "_event.data.call_succeeded == true");
            acceptTransition->SetAttribute("target", acceptState);

            // Crea il nodo <transition> per "reject"
            tinyxml2::XMLElement* rejectTransition = element->GetDocument()->NewElement("transition");
            rejectTransition->SetAttribute("event", (eventStr).c_str());
            rejectTransition->SetAttribute("cond", "_event.data.call_succeeded == false");
            rejectTransition->SetAttribute("target", rejectState);

            // Ottieni il genitore dell'elemento corrente
            tinyxml2::XMLElement* parent = element->Parent()->ToElement();

            // Inserisci i nuovi elementi dopo l'elemento corrente
            insertElementAfter(parent, element, acceptTransition);
            insertElementAfter(parent, acceptTransition, rejectTransition);

            // Rimuovi l'elemento originale
            parent->DeleteChild(element);
        } else {
            add_to_log("Attributi mancanti: event, accept o reject.");
        }
    }
    return true;
}

/**
 * @brief translator function to translate the high level xml to scxml
 * 
 * @param fileData fileDataStr structure containing the input and output file names
 * @return true if the translation is successful
 */
bool Translator(fileDataStr& fileData){
    add_to_log("-----------");
    add_to_log("Translator");
    skillDataStr skillData;
    tinyxml2::XMLDocument doc;
    std::string fileContent;
    if( !readHLXMLFile(doc, fileContent, fileData.inputFileName)){
        return false;
    }

    // Get Root and SkillData
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found" << std::endl;
        return false;
    }
    getDataFromRootNameHighLevel(root->Attribute("name"), skillData);

    // Add log print for each state entry
    if (fileData.debug_mode) {
        addLogToStateEntries(&doc, root);
    }

    // Get Skill Type
    tinyxml2::XMLElement* haltServerElement;
    if(findElementByTagAndAttValueContaining(root, std::string("ros_service_server"), std::string("type"), std::string("bt_interfaces_dummy/HaltAction"), haltServerElement) ||
       findElementByTagAndAttValueContaining(root, std::string("ros_service_server"), std::string("type"), std::string("bt_interfaces/HaltAction"), haltServerElement))
    {
        add_to_log("Halt found => Action Skill");
        skillData.skillType = "Action";
        fileData.is_action_skill = true;
    } else {
        add_to_log("Halt not found => Condition Skill");
        skillData.skillType = "Condition"; 
        fileData.is_action_skill = false;
    }
    skillData.SMName = skillData.SMName + skillData.skillType;
    skillData.skillTypeLC = skillData.skillType; 
    for (char &c : skillData.skillTypeLC) 
    { 
        c = std::tolower(c); // e.g. skillType = "action"
    } 
    if(!replaceAttributeValue(root, "name", skillData.SMName))
    {
        add_to_log("Could not replace name attribute for root element");
        return false;
    }

    // Delete server and topic declarations
    std::vector<tinyxml2::XMLElement*> serverVector;
    
    findElementVectorByTag(root, std::string("ros_service_server"), serverVector); 
    findElementVectorByTag(root, std::string("ros_topic_subscriber"), serverVector);
    findElementVectorByTag(root, std::string("ros_service_client"), serverVector); 
    findElementVectorByTag(root, std::string("ros_topic_publisher"), serverVector); 
    std::vector<tinyxml2::XMLElement*> actionVector;
    findElementVectorByTag(root, std::string("ros_action_client"), actionVector);
    std::map<std::string, std::string> nameToActionNameMap;
     for (tinyxml2::XMLElement* element : actionVector) {
        const char* name = element->Attribute("name");
        const char* actionName = element->Attribute("action_name");

        if (name && actionName) {
            nameToActionNameMap[name] = actionName;
        } else {
            std::cerr << "Missing attribute in ros_action_client tag\n";
        }
    }
    deleteElementFromVector(actionVector);
    deleteElementFromVector(serverVector);
    
    // add xmln
    replaceAttributeValue(root, "xmlns", "http://www.w3.org/2005/07/scxml");

    // Delete attribute type from data elements
    std::vector<tinyxml2::XMLElement*> dataVector;
    findElementVectorByTag(root, std::string("data"), dataVector);
    deleteElementAttributeFromVector(dataVector, "type");

    // Translate elements with tag ros_service_handle_request 
    std::vector<tinyxml2::XMLElement*> srvHndlReqVector;
    findElementVectorByTag(root, std::string("ros_service_handle_request"), srvHndlReqVector);
    replaceAttributeNameFromVector(srvHndlReqVector, "name", "event");
    replaceAttributeValueContaingFromVector(srvHndlReqVector, "event", "tick", "CMD_TICK");
    replaceAttributeValueContaingFromVector(srvHndlReqVector, "event", "halt", "CMD_HALT");
    replaceTagNameFromVector(&doc, srvHndlReqVector, "transition");

    // Translate elements with tag ros_service_send_request 
    std::vector<tinyxml2::XMLElement*> srvSendReqVector;
    findElementVectorByTag(root, std::string("ros_service_send_request"), srvSendReqVector);
    replaceAttributeNameFromVector(srvSendReqVector, "name", "event");
    replaceEventValueFromVectorFromSlashToPoint(srvSendReqVector);
    appendAttributeValueFromVector(srvSendReqVector, "event", ".Call");
    for (auto& element : srvSendReqVector) {
        std::vector<tinyxml2::XMLElement*> srvSendReqFieldVector;
        findElementVectorByTag(element, std::string("field"), srvSendReqFieldVector);
        replaceTagNameFromVector(&doc, srvSendReqFieldVector, "param");
    }
    replaceTagNameFromVector(&doc, srvSendReqVector, "send");

    // Translate elements with tag ros_service_handle_response 
    std::vector<tinyxml2::XMLElement*> srvHndlRspVector;
    findElementVectorByTag(root, std::string("ros_service_handle_response"), srvHndlRspVector);
    replaceAttributeNameFromVector(srvHndlRspVector, "name", "event");
    // replaceAttributeValueFromVector(srvHndlRspVector, "event", ""); get value e modificare le / con .
    replaceAttributeValueSubstringFromVector(srvHndlRspVector, "cond", "_res.", "_event.data.");
    replaceEventValueFromVectorFromSlashToPoint(srvHndlRspVector);
    appendAttributeValueFromVector(srvHndlRspVector, "event", ".Return");
    replaceTagNameFromVector(&doc, srvHndlRspVector, "transition");

    // Translate elements with tag ros_service_send_response 
    std::vector<tinyxml2::XMLElement*> srvSendRspVector;
    findElementVectorByTag(root, std::string("ros_service_send_response"), srvSendRspVector);
    replaceAttributeNameFromVector(srvSendRspVector, "name", "event");
    replaceAttributeValueContaingFromVector(srvSendRspVector, "event", "tick", "TICK_RESPONSE"); 
    replaceAttributeValueContaingFromVector(srvSendRspVector, "event", "halt", "HALT_RESPONSE"); 
    for (auto& element : srvSendRspVector) {
        std::vector<tinyxml2::XMLElement*> srvSendRspFieldVector;
        findElementVectorByTag(element, std::string("field"), srvSendRspFieldVector);
        replaceTagNameFromVector(&doc, srvSendRspFieldVector, "param");
    }
    replaceTagNameFromVector(&doc, srvSendRspVector, "send");

    // Translate elements with tag ros_topic_callback 
    std::vector<tinyxml2::XMLElement*> topicCallbackVector;
    findElementVectorByTag(root, std::string("ros_topic_callback"), topicCallbackVector);
    replaceAttributeNameFromVector(topicCallbackVector, "name", "event");
    replaceEventValueFromVectorFromSlashToPoint(topicCallbackVector);
    appendAttributeValueFromVector(topicCallbackVector, "event", ".Sub");
    replaceTagNameFromVector(&doc, topicCallbackVector, "transition");

    std::vector<tinyxml2::XMLElement*> assignVector;
    findElementVectorByTag(root, std::string("assign"), assignVector);
    // appendIfContainsSubstringVector(assignVector, "expr", "_msg.", ".");
    replaceAttributeValueSubstringFromVector(assignVector, "expr", "_msg.", "_event.data.");
    replaceAttributeValueSubstringFromVector(assignVector, "expr", "_res.", "_event.data.");
    // appendAttributeValueFromVector(assignVector, "expr", ".");
    // appendAttributeValueLocationFromVector(assignVector, "expr");

    // Translate elements with tag ros_action_send_goal
    std::vector<tinyxml2::XMLElement*> actionSendGoalVector;
    findElementVectorByTag(root, std::string("ros_action_send_goal"), actionSendGoalVector);
    replaceAttributeNameFromVector(actionSendGoalVector, "name", "event");
    for (auto& element : actionSendGoalVector) {
        const char* eventValue = element->Attribute("event");
        if (eventValue) {
            std::string eventStr(eventValue);
            auto it = nameToActionNameMap.find(eventStr);
            if (it != nameToActionNameMap.end()) {
                // Replacing the event value with the action name
                element->SetAttribute("event", it->second.c_str());
                add_to_log("Updating: " + eventStr + " -> " + it->second);
            } else {
                add_to_log("No update for: " + eventStr);
            }
        } else {
            add_to_log("No 'event' attribute found");
        }
    }    
    replaceEventValueFromVectorFromSlashToPoint(actionSendGoalVector);
    appendAttributeValueFromVector(actionSendGoalVector, "event", ".SendGoal");
    for (auto& element : actionSendGoalVector) {
        std::vector<tinyxml2::XMLElement*> actionFieldVector;
        findElementVectorByTag(element, std::string("field"), actionFieldVector);
        replaceTagNameFromVector(&doc, actionFieldVector, "param");
    }
    replaceTagNameFromVector(&doc, actionSendGoalVector, "send");

    // Translate elements with tag ros_action_handle_goal_response
    translateRosActionHandleGoalResponseTag(root, nameToActionNameMap);

    //Translate elements with tag ros_action_handle_feedback
    std::vector<tinyxml2::XMLElement*> actionHandleFeedbackVector;
    findElementVectorByTag(root, std::string("ros_action_handle_feedback"), actionHandleFeedbackVector);
    replaceAttributeNameFromVector(actionHandleFeedbackVector, "name", "event");
    for (auto& element : actionHandleFeedbackVector) {
        const char* eventValue = element->Attribute("event");
        if (eventValue) {
            std::string eventStr(eventValue);
            auto it = nameToActionNameMap.find(eventStr);
            if (it != nameToActionNameMap.end()) {
                // Replacing the event value with the action name
                element->SetAttribute("event", it->second.c_str());
                add_to_log("Updating: " + eventStr + " -> " + it->second);
            } else {
                add_to_log("No update for: " + eventStr);
            }
        } else {
            add_to_log("No 'event' attribute found");
        }
    }
    replaceEventValueFromVectorFromSlashToPoint(actionHandleFeedbackVector);
    appendAttributeValueFromVector(actionHandleFeedbackVector, "event", ".FeedbackReturn");
    replaceTagNameFromVector(&doc, actionHandleFeedbackVector, "transition");
    replaceAttributeValueSubstringFromVector(assignVector, "expr", "_feedback.", "_event.data.");

    // Translate elements with tag ros_action_handle_success_result
    std::vector<tinyxml2::XMLElement*> actionHandleSuccessResultVector;
    findElementVectorByTag(root, std::string("ros_action_handle_success_result"), actionHandleSuccessResultVector);
    replaceAttributeNameFromVector(actionHandleSuccessResultVector, "name", "event");
    for (auto& element : actionHandleSuccessResultVector) {
        const char* eventValue = element->Attribute("event");
        if (eventValue) {
            std::string eventStr(eventValue);
            auto it = nameToActionNameMap.find(eventStr);
            if (it != nameToActionNameMap.end()) {
                // Replacing the event value with the action name
                element->SetAttribute("event", it->second.c_str());
                add_to_log("Updating: " + eventStr + " -> " + it->second);
            } else {
                add_to_log("No update for: " + eventStr);
            }
        } else {
            add_to_log("No 'event' attribute found");
        }
    }
    replaceEventValueFromVectorFromSlashToPoint(actionHandleSuccessResultVector);
    appendAttributeValueFromVector(actionHandleSuccessResultVector, "event", ".ResultResponse");
    replaceTagNameFromVector(&doc, actionHandleSuccessResultVector, "transition");
    replaceAttributeValueSubstringFromVector(assignVector, "expr", "_wrapped_result.result.", "_event.data.");

    // Translate elements with tag ros_action_handle_cancel_response
    std::vector<tinyxml2::XMLElement*> actionHandleCancelResultVector;
    findElementVectorByTag(root, std::string("ros_action_handle_cancel_result"), actionHandleCancelResultVector);
    replaceAttributeNameFromVector(actionHandleCancelResultVector, "name", "event");
    for (auto& element : actionHandleCancelResultVector) {
        const char* eventValue = element->Attribute("event");
        if (eventValue) {
            std::string eventStr(eventValue);
            auto it = nameToActionNameMap.find(eventStr);
            if (it != nameToActionNameMap.end()) {
                // Replacing the event value with the action name
                element->SetAttribute("event", it->second.c_str());
                add_to_log("Updating: " + eventStr + " -> " + it->second);
            } else {
                add_to_log("No update for: " + eventStr);
            }
        } else {
            add_to_log("No 'event' attribute found");
        }
    }
    replaceEventValueFromVectorFromSlashToPoint(actionHandleCancelResultVector);
    appendAttributeValueFromVector(actionHandleCancelResultVector, "event", ".CancelResult");
    replaceTagNameFromVector(&doc, actionHandleCancelResultVector, "transition");

    //Translate elements with tag ros_action_send_cancel
    std::vector<tinyxml2::XMLElement*> actionSendCancelVector;
    findElementVectorByTag(root, std::string("ros_action_send_cancel"), actionSendCancelVector);
    replaceAttributeNameFromVector(actionSendCancelVector, "name", "event");
    for (auto& element : actionSendCancelVector) {
        const char* eventValue = element->Attribute("event");
        if (eventValue) {
            std::string eventStr(eventValue);
            auto it = nameToActionNameMap.find(eventStr);
            if (it != nameToActionNameMap.end()) {
                // Replacing the event value with the action name
                element->SetAttribute("event", it->second.c_str());
                add_to_log("Updating: " + eventStr + " -> " + it->second);
            } else {
                add_to_log("No update for: " + eventStr);
            }
        } else {
            add_to_log("No 'event' attribute found");
        }
    }
    replaceEventValueFromVectorFromSlashToPoint(actionSendCancelVector);
    appendAttributeValueFromVector(actionSendCancelVector, "event", ".SendCancel");
    replaceTagNameFromVector(&doc, actionSendCancelVector, "send");

    // doc.Print();
    std::string ouputFilePath = fileData.outputPathSrc + skillData.className + "SM.scxml";
    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);  // Print the XML document into the printer
    std::string outputContent = std::string(printer.CStr());
    
    // Add autogenerated header comment to SCXML (after XML declaration)
    std::string currentDatetime = getCurrentDatetime();
    std::string header = "<!-- Autogenerated by model2code (https://github.com/convince-project/model2code) -->\n";
    header += "<!-- File autogenerated at " + currentDatetime + " with M2C version " + std::string(m2cVersion) + " -->\n";
    header += "<!-- This is an automatically generated file. -->\n";
    
    // Find the end of the XML declaration and insert comment after it
    size_t xmlDeclEnd = outputContent.find("?>");
    if (xmlDeclEnd != std::string::npos) {
        xmlDeclEnd += 2; // Move past "?>"
        // Find the newline after the XML declaration
        size_t newlinePos = outputContent.find('\n', xmlDeclEnd);
        if (newlinePos != std::string::npos) {
            outputContent.insert(newlinePos + 1, header);
        } else {
            outputContent.insert(xmlDeclEnd, "\n" + header);
        }
    }
    
    // add_to_log("-----------");
    createDirectory(fileData.outputPath);
    createDirectory(fileData.outputPathSrc);
    std::cout << "-----------" << std::endl;
    std::cout << "Model2Code version " << m2cVersion << std::endl;
    std::cout << "Generating Skill-level code for: "<< skillData.className << std::endl; 
    std::cout << "Output directory: " << fileData.outputPath << std::endl;
    std::cout << "-----------" << std::endl;
    writeFile(fileData.outputPathSrc, skillData.className + "SM.scxml", outputContent);
    fileData.outputFileTranslatedSM = ouputFilePath;

    return true;
}
