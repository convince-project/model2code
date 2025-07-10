// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file ExtractFromXML.cpp
 * @brief This file contains the functions to extract data from custom XML files.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include "ExtractFromXML.h"


/**
 * \defgroup readFromXMLFiles Read from custom XML Files
 * \ingroup ExtractFromXMLFunctions
 * Read from custom XML Files
 * @{
 */



bool extractInterfaceData(const fileDataStr fileData, eventDataStr& eventData)
{
    // const std::string componentName = eventData.componentName;
    std::string interfaceName;
    std::string fileName = fileData.modelFileName;

    // open the model file
    // and check if it exists
    // add_to_log("opening file: " + fileName + " at line " + std::to_string(__LINE__));
    // if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
    //     std::cerr << "Failed to load '" << fileName << "' file" << std::endl;
    //     return false;
    // }
    // add_to_log("******************************** event DATA ********************************\n");
    // printEventData(eventData);
    // add_to_log("******************************** event DATA END ********************************\n");

    // open the input file and check if it exists

    tinyxml2::XMLElement* element;
    findInterfaceType(fileData, eventData, element);
    add_to_log("******************************** event DATA AFTER ********************************\n");
    printEventData(eventData);
    add_to_log("******************************** event DATA AFTER END ********************************\n");

    // find the interface name in the high level tag
    // if (!is_ros_service_server) {
    //     std::cerr << "No rosServiceServer found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
    //     return false;
    // }
    return true;
    
}


bool findInterfaceType(const fileDataStr fileData, eventDataStr& eventData, tinyxml2::XMLElement*& element) 
{
    tinyxml2::XMLDocument doc;

    add_to_log("opening file: " + fileData.inputFileName + " at line" + std::to_string(__LINE__));
    if (doc.LoadFile(fileData.inputFileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load '" << fileData.inputFileName << "' file" << std::endl;
        return false;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found in file: " << fileData.inputFileName << std::endl;
        return false;
    }
    // std::string messageInterfaceType;

    // ROS SERVICE SERVER
    bool is_ros_service_server = findElementByTagAndAttValue(root, std::string("ros_service_server"), std::string("service_name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_ros_service_server: " + std::to_string(is_ros_service_server) + " at line " + std::to_string(__LINE__));
    if (is_ros_service_server) {
        if (!getElementAttValue(element, std::string("type"), eventData.messageInterfaceType))
        {
            std::cerr << "No type attribute found in ros_service_server element for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }

        // eventData.interfaceName = interfaceType;
        eventData.rosInterfaceType = "async-service";
        eventData.serverName = "/" + eventData.componentName + "/" + eventData.functionName;
        eventData.clientName = "/" + eventData.componentName + "/" + eventData.functionName;


        // handle request fields
        tinyxml2::XMLElement* fieldParent;
        findElementByTagAndAttValue(root, std::string("ros_service_handle_request"), std::string("name"), std::string(eventData.serverName), fieldParent);
        if (!fieldParent) {
            std::cerr << "No ros_service_handle_request element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        if (!getInterfaceFieldsFromExpr(fieldParent, eventData.interfaceRequestFields)) {
            std::cerr << "Failed to get interface request fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }


        // handle response fields
        tinyxml2::XMLElement* responseParent;
        findElementByTagAndAttValue(root, std::string("ros_service_send_response"), std::string("name"), std::string(eventData.clientName), responseParent);
        if (!responseParent) {
            std::cerr << "No ros_service_send_response element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }  
        if (!getInterfaceFieldsFromName(responseParent, eventData.interfaceResponseFields)) {
            std::cerr << "Failed to get interface response fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        add_to_log("fieldParent: " + std::string(fieldParent->Name()) + " at line " + std::to_string(__LINE__));
        return true;
    } 



    // ROS SERVICE CLIENT
    bool is_ros_service_client = findElementByTagAndAttValue(root, std::string("ros_service_client"), std::string("service_name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_ros_service_client: " + std::to_string(is_ros_service_client) + " at line " + std::to_string(__LINE__));
    if (is_ros_service_client) {
        if (!getElementAttValue(element, std::string("type"), eventData.messageInterfaceType))
        {
            std::cerr << "No type attribute found in ros_service_client element for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }


        eventData.rosInterfaceType = "async-service";
        eventData.clientName = "/" + eventData.componentName + "/" + eventData.functionName;
        eventData.serverName = "/" + eventData.componentName + "/" + eventData.functionName;        // handle request fields
        tinyxml2::XMLElement* fieldParent;
        if (!findElementByTagAndAttValue(root, std::string("ros_service_send_request"), std::string("name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), fieldParent)) 
        {
            std::cerr << "No ros_service_send_request element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        add_to_log("fieldParent: " + std::string(fieldParent->Name()) + " at line " + std::to_string(__LINE__));
        if (!getInterfaceFieldsFromExpr(fieldParent, eventData.interfaceRequestFields))
        {
            std::cerr << "Failed to get interface response fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }


        // handle response fields
        tinyxml2::XMLElement* responseParent;
        if (!findElementByTagAndAttValue(root, std::string("ros_service_handle_response"), std::string("name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), responseParent)) 
        {
            std::cerr << "No ros_service_handle_response element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        add_to_log("responseParent: " + std::string(responseParent->Name()) + " at line " + std::to_string(__LINE__));
        if (!getInterfaceFieldsFromName(responseParent, eventData.interfaceResponseFields))
        {
            std::cerr << "Failed to get interface request fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }

        return true;
    }



    // ROS TOPIC PUBLISHER
    bool is_topic_publisher = findElementByTagAndAttValue(root, std::string("ros_topic_publisher"), std::string("topic"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_topic_publisher: " + std::to_string(is_topic_publisher) + " at line " + std::to_string(__LINE__));
    if (is_topic_publisher) {
        eventData.rosInterfaceType = "topic";
        getElementAttValue(element, std::string("type"), eventData.messageInterfaceType);
        getElementAttValue(element, std::string("name"), eventData.scxmlInterfaceName);
        getElementAttValue(element, std::string("topic"), eventData.topicName);
        tinyxml2::XMLElement* fieldParent;

        //get the fields from the ros_topic_publish element
        findElementByTagAndAttValue(root, std::string("ros_topic_publish"), std::string("name"), std::string(eventData.scxmlInterfaceName), fieldParent);
        if (!fieldParent) {
            std::cerr << "No ros_topic_publish element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        if(!getInterfaceFieldsFromName(fieldParent, eventData.interfaceTopicFields)) {
            std::cerr << "Failed to get interface topic fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        return true;
    }



    // ROS TOPIC SUBSCRIBER
    bool is_topic_subscriber = findElementByTagAndAttValue(root, std::string("ros_topic_subscriber"), std::string("topic"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_topic_subscriber: " + std::to_string(is_topic_subscriber) + " at line " + std::to_string(__LINE__));
    if (is_topic_subscriber) {
        std::cerr << "Found ros_topic_subscriber for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
        eventData.rosInterfaceType = "topic";
        getElementAttValue(element, std::string("type"), eventData.messageInterfaceType);
        getElementAttValue(element, std::string("topic"), eventData.topicName);
        std::cerr << "Topic name: " << eventData.topicName << std::endl;
        tinyxml2::XMLElement* fieldParent;


        //get the fields from the ros_topic_subscribe element
        if (!findElementByTagAndAttValue(root, std::string("ros_topic_callback"), std::string("name"), std::string(eventData.topicName), fieldParent)){
            std::cerr << "No ros_topic_callback element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        std::cerr << "fieldParent: " << fieldParent->Name() << " at line " << __LINE__ << std::endl;
        if(!getInterfaceFieldsFromExpr(fieldParent, eventData.interfaceTopicFields)) {
            std::cerr << "Failed to get interface topic fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }


        std::cerr << "interfaceTopicFields: " << std::endl;
        // for (const auto& field : eventData.interfaceTopicFields) {
        //     std::cerr << " - " << field << std::endl;
        // }
        return true;
    }




    bool is_action_server = findElementByTagAndAttValue(root, std::string("ros_action_server"), std::string("action_name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_action_server: " + std::to_string(is_action_server) + " at line " + std::to_string(__LINE__));
    if (is_action_server) {
        getElementAttValue(element, std::string("type"), eventData.messageInterfaceType);
        eventData.rosInterfaceType = "action";
        eventData.serverName = "/" + eventData.componentName + "/" + eventData.functionName;
        return true;
    }




    bool is_action_client = findElementByTagAndAttValue(root, std::string("ros_action_client"), std::string("action_name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_action_client: " + std::to_string(is_action_client) + " at line " + std::to_string(__LINE__));
    if (is_action_client) {
        getElementAttValue(element, std::string("type"), eventData.messageInterfaceType);
        eventData.rosInterfaceType = "action";
        eventData.clientName = "/" + eventData.componentName + "/" + eventData.functionName;
        return true;
    }

    return false; // if no interface type is found, return false
}



bool getInterfaceFieldsFromName(tinyxml2::XMLElement* element, std::vector<std::string>& interfaceFields)
{
    if (!element) {
        std::cerr << "Element is null" << std::endl;
        return false;
    }
    
    tinyxml2::XMLElement* fieldElement = element->FirstChildElement("assign");
    while (fieldElement) {
        const char* fieldName = fieldElement->Attribute("expr");
        if (fieldName) {
            // find . in the string
            std::string fieldNameStr(fieldName);
            size_t dotPos = fieldNameStr.find('.');
            if (dotPos != std::string::npos) {
                // if . is found, take the part after it
                fieldNameStr = fieldNameStr.substr(dotPos + 1);
                interfaceFields.push_back(fieldNameStr);
            }
        }
        fieldElement = fieldElement->NextSiblingElement("assign");
    }
    return true;
}


bool getInterfaceFieldsFromExpr(tinyxml2::XMLElement* element, std::vector<std::string>& interfaceFields)
{
    if (element == nullptr) {
        std::cerr << "Element is null" << std::endl;
        return false;
    }
    
    // get the first child element
    tinyxml2::XMLElement* fieldElement = element->FirstChildElement("field");
    while (fieldElement) {
        const char* fieldName = fieldElement->Attribute("name");
        if (fieldName) {
            std::cerr << "Found field: " << fieldName << std::endl;
            interfaceFields.push_back(fieldName);
        }
        fieldElement = fieldElement->NextSiblingElement("expr");
    }
    return true;
}

/**
 * @brief Extract the interface name from the model file
 * 
 * @param fileName model file name from which to extract the interface name
 * @param eventData event data structure, where the interface name will be stored, returned by reference
 * @return bool
 */
bool extractInterfaceName(const std::string fileName, eventDataStr& eventData)
{
    const std::string componentName = eventData.componentName;
    std::string interfaceName;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load '" << fileName << "' file" << std::endl;
        return false;
    }
 
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found" << std::endl;
        return false;
    }
    
    tinyxml2::XMLElement* element;
    if(!findElementByTagAndAttValue(root, std::string("componentDeclaration"), std::string("id"), componentName, element)){
        std::cerr << "\nNo component '" << componentName << "'found in file '" << fileName << "'."<< std::endl;
        return false;
    }
    if (!getElementAttValue(element, std::string("interface"), interfaceName)){
        std::cerr << "\nNo interface found for component '" << componentName << "'."<< std::endl;
        return false;
    }
    eventData.interfaceName = interfaceName;

    return true;  
}


/**
 * @brief Extract the interface data from the interface file
 * 
 * @param fileName interface file name from which to extract the interface data
 * @param eventData event data structure, where the interface data will be stored, returned by reference
 * @return true 
 * @return false 
 */

// this is with the file interfaces.xml
// it extracts the interface type, dataType and dataField
// it also checks if the interface is virtual
bool extractInterfaceType(const std::string fileName, eventDataStr& eventData)
{
    const std::string interfaceName = eventData.interfaceName;
    const std::string functionName = eventData.functionName;
    std::string interfaceType, interfaceDataType, interfaceDataField;
    tinyxml2::XMLDocument doc;
    add_to_log("opening file: " + fileName + " at line " + std::to_string(__LINE__));
    std::cerr << "Extracting interface type from file: " << fileName << std::endl;
    // file loading
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load '" << fileName << "' file" << std::endl;
        return false;
    }

    // get the root element
    // and check if it exists
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found" << std::endl;
        return false;
    }


    std::string idValue;
    tinyxml2::XMLElement *elementInterface;
    tinyxml2::XMLElement *elementInterfaceType;
    tinyxml2::XMLElement *elementFunction;
    tinyxml2::XMLElement *elementDataType;
    tinyxml2::XMLElement *elementDataField;

    // find the interface element by tag and attribute value
    // until here it does not change
    // find either a 





    if (!findElementByTagAndAttValue(root, std::string("interface"), std::string("id"), interfaceName, elementInterface))
    {
        std::cerr << "\nNo interface '" << interfaceName << "'found in file '" << fileName << "'."<< std::endl;
        return false;
    }

    // check if the interface element has the "virtual" attribute
    eventData.virtualInterface = false;
    const char* virtual_attribute = elementInterface->Attribute("virtual");
    if (virtual_attribute != nullptr) {
        if (strcmp(virtual_attribute, "true") == 0) {
            eventData.virtualInterface = true;
        }
    }

    if(!findElementByTagAndAttValue(elementInterface, std::string("function"), std::string("id"), functionName, elementFunction))
    {
        std::cerr << "No function '" << functionName << "'found in file '" << fileName << "'."<< std::endl;
        return false;
    }
    if(!findElementByTag(elementFunction, std::string("interface"), elementInterfaceType))
    {
        return false;
    }
    if(!getElementAttValue(elementInterfaceType, std::string("type"), interfaceType))
    {
        return false;
    }
    eventData.interfaceType = interfaceType;
    if(!findElementByTag(elementFunction, std::string("dataField"), elementDataField))
    {   
        // std::cerr << "No tag <dataField> for function '" << functionName << "'."<< std::endl;
        add_to_log("No tag <dataField> for function '" + functionName + "'.");
        return true;
    } 
    if(!getElementText(elementDataField, interfaceDataField))
    {
        // std::cerr << "No value in tag <dataField> for function '" << functionName << "'."<< std::endl;
        add_to_log("No value in tag <dataField> for function '" + functionName + "'.");
        return true;
    }
    if(!findElementByTag(elementFunction, std::string("dataType"), elementDataType))
    {   
        // std::cerr << "No tag <dataType> for function '" << functionName << "'."<< std::endl;
        add_to_log("No tag <dataType> for function '" + functionName + "'.");
        //Not every interface has a defined DataType TODO
        return true;
    } 
    // if(!getElementText(elementDataType, interfaceDataType))
    // {
    //     // std::cerr << "No value in tag <dataType> for function '" << functionName << "'."<< std::endl;
    //     add_to_log("No value in tag <dataType> for function '" + functionName + "'.");
    //     return true;
    // }
    // tinyxml2::XMLElement* elementsReturnValue;
    // findElementByTag(elementFunction, "returnValue", elementsReturnValue);

    // if (elementsReturnValue == nullptr) {
    //     std::cerr << "No returnValue found for function '" << functionName << "'."<< std::endl;
    //     return false;
    // }
    // tinyxml2::XMLElement* dataType;
    // findElementByTag(element, std::string("dataType"), elementData);
    // getElementText(elementData, interfaceDataType);

    // std::vector<tinyxml2::XMLElement*> dataField;
    // findElementVectorByTag(element, std::string("dataField"), dataField);
    // if (dataField.empty()) {
    //     std::cerr << "No dataField found for function '" << functionName << "'."<< std::endl;
    //     return false;
    // }

    // for (const auto& element : dataField) {
    //     getEventDataFields(elementFunction, eventData, interfaceDataField, interfaceDataType);
    // }
    // // std::vector<tinyxml2::XMLElement*> elementsReturnValue;
    // // findElementVectorByTag(elementFunction, "returnValue", elementsReturnValue);
    // // for (const auto& element : elementsReturnValue) {
    // //     tinyxml2::XMLElement* elementData;
    // //     findElementByTag(element, std::string("dataField"), elementData);
    // //     getElementText(elementData, interfaceDataField);
    // //     findElementByTag(element, std::string("dataType"), elementData);
    // //     getElementText(elementData, interfaceDataType);    
    // //     eventData.interfaceData[interfaceDataField] = interfaceDataType;
    // // }
    // getEventDataFields(elementFunction, eventData, interfaceDataField, interfaceDataType);
    return true;
}


// /**
//  * @brief Get the Event Data Fields object  
//  * 
//  * @param elementFunction 
//  * @param eventData 
//  * @param interfaceDataField
//  * @param interfaceDataType
//  * @return true 
//  * @return false 
//  */
// bool getEventDataFields(tinyxml2::XMLElement * elementFunction, eventDataStr& eventData, std::string interfaceDataField, std::string interfaceDataType) 
// {

//     getElementText(elementData, interfaceDataField);
//         // findElementByTag(element, std::string("dataType"), elementData);
//     std::vector<tinyxml2::XMLElement*> dataTypes;
//     findElementVectorByTag(element, std::string("dataType"), dataTypes);
//     if (dataTypes.empty()) {
//         std::cerr << "No dataType found for function '" << eventData.functionName << "'."<< std::endl;
//         return false;
//     }
        
//         getElementText(elementData, interfaceDataType);    
//         eventData.interfaceData[interfaceDataField] = interfaceDataType;
    
//     return true;
// }

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
bool extractFromSCXML(tinyxml2::XMLDocument& doc, const std::string fileName, std::string& rootName, std::vector<tinyxml2::XMLElement*>& elementsTransition, std::vector<tinyxml2::XMLElement*>& elementsSend) 
{
    add_to_log("opening file: " + fileName + " at line " + std::to_string(__LINE__));
    if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load '" << fileName << "' file" << std::endl;
        return false;
    }
 
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found" << std::endl;
        return false;
    }
    rootName = std::string(root->Attribute("name")); 
    if(rootName == std::string(""))
    {
        std::cerr << "No root name found" << std::endl;
        return false;
    }
      
    // Get transitions
    findElementVectorByTagAndAttribute(root, std::string("transition"), "event", elementsTransition);
    if (elementsTransition.empty()) {
        add_to_log("No transition elements found.");
    } 
    else 
    {
        add_to_log("Transition elements found.");
    }

    // Get Send
    findElementVectorByTagAndAttribute(root, std::string("send"), "event", elementsSend);
    if (elementsSend.empty()) {
        add_to_log("No Send elements found.");
    } 
    else 
    {
        add_to_log("Send elements found.");
    }
    return true;
}
/** @} */ // end of readFromXMLFiles subgroup
/** @} */ // end of ExtractFromXMLFunctions group