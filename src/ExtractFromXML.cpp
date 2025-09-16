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
    
    // Parse interface file to populate interfaceData with field types
    parseInterfaceFile(eventData);
    
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
    std::cout << std::string("/" + eventData.componentName + "/" + eventData.functionName) << std::endl;

    // ROS SERVICE SERVER
    bool is_ros_service_server = findElementByTagAndAttValue(root, std::string("ros_service_server"), std::string("service_name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_ros_service_server: " + std::to_string(is_ros_service_server) + " at line " + std::to_string(__LINE__));
    if (is_ros_service_server) {
        if (!getElementAttValue(element, std::string("type"), eventData.messageInterfaceType))
        {
            std::cerr << "No type attribute found in ros_service_server element for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        
        eventData.interfaceName = eventData.messageInterfaceType.substr(0, eventData.messageInterfaceType.find_last_of("/"));
        add_to_log("interfaceName: " + eventData.interfaceName + " at line " + std::to_string(__LINE__));
        eventData.interfaceType = "async-service";
        eventData.rosInterfaceType = "service-server"; // type of the interface in ROS
        // eventData.serverName = "/" + eventData.componentName + "/" + eventData.functionName;
        // eventData.clientName = "/" + eventData.componentName + "/" + eventData.functionName;


        // handle request fields
        tinyxml2::XMLElement* fieldParent;
        findElementByTagAndAttValue(root, std::string("ros_service_handle_request"), std::string("name"), std::string(eventData.serverName), fieldParent);
        if (!fieldParent) {
            std::cerr << "No ros_service_handle_request element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            // return false;
        }
        if (!getInterfaceFieldsFromAssignTag(fieldParent, eventData.interfaceRequestFields)) {
            std::cerr << "Failed to get interface ros_service_handle_request fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            // return false;
        }


        // handle response fields
        tinyxml2::XMLElement* responseParent;
        findElementByTagAndAttValue(root, std::string("ros_service_send_response"), std::string("name"), std::string(eventData.clientName), responseParent);
        if (!responseParent) {
            std::cerr << "No ros_service_send_response element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            // return false;
        }  
        if (!getInterfaceFieldsFromFieldTag(responseParent, eventData.interfaceResponseFields)) {
            std::cerr << "Failed to get interface ros_service_send_response fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            // return false;
        }
        add_to_log("fieldParent: " + std::string(fieldParent->Name()) + " at line " + std::to_string(__LINE__));
        return true;
    } 


    // replace all dots in the component name with slash
    std::string componentName = eventData.componentName;
    size_t pos = 0;
    while ((pos = componentName.find('.')) != std::string::npos) {
        componentName.replace(pos, 1, "/");
        pos += 1; // Move past the replaced character
    }
    eventData.componentName = componentName;
    // replace all dots in the function name with slash
    std::string functionName = eventData.functionName;
    pos = 0;
    while ((pos = functionName.find('.')) != std::string::npos) {
        functionName.replace(pos, 1, "/");
        pos += 1; // Move past the replaced character
    }
    eventData.functionName = functionName;
    // ROS SERVICE CLIENT
    
    bool is_ros_service_client = findElementByTagAndAttValue(root, std::string("ros_service_client"), std::string("service_name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_ros_service_client: " + std::to_string(is_ros_service_client) + " at line " + std::to_string(__LINE__));
    if (is_ros_service_client) {
        if (!getElementAttValue(element, std::string("type"), eventData.messageInterfaceType))
        {
            std::cerr << "No type attribute found in ros_service_client element for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        eventData.rosInterfaceType = "service-client"; // type of the interface in ROS
        eventData.interfaceName = eventData.messageInterfaceType.substr(0, eventData.messageInterfaceType.find_last_of("/"));
        add_to_log("interfaceName: " + eventData.interfaceName + " at line " + std::to_string(__LINE__));
        
        // Extract the service type name from messageInterfaceType for correct template generation
        size_t lastSlash = eventData.messageInterfaceType.find_last_of("/");
        if (lastSlash != std::string::npos) {
            eventData.serviceTypeName = eventData.messageInterfaceType.substr(lastSlash + 1);
            turnToSnakeCase(eventData.serviceTypeName, eventData.serviceTypeNameSnakeCase);
            add_to_log("serviceTypeName: " + eventData.serviceTypeName + " (snake_case: " + eventData.serviceTypeNameSnakeCase + ") at line " + std::to_string(__LINE__));
        }
        
        eventData.interfaceType = "async-service";
        // eventData.clientName = "/" + eventData.componentName + "/" + eventData.functionName;
        // eventData.serverName = "/" + eventData.componentName + "/" + eventData.functionName;        // handle request fields
        tinyxml2::XMLElement* fieldParent;
        if (!findElementByTagAndAttValue(root, std::string("ros_service_send_request"), std::string("name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), fieldParent)) 
        {
            std::cerr << "No ros_service_send_request element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            // return false;
        }
        if (fieldParent) {
            add_to_log("fieldParent: " + std::string(fieldParent->Name()) + " at line " + std::to_string(__LINE__));
        }
        if (!getInterfaceFieldsFromFieldTag(fieldParent, eventData.interfaceRequestFields))
        {
            std::cerr << "Failed to get interface ros_service_send_request fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            // return false;
        }


        // handle response fields
        tinyxml2::XMLElement* responseParent;
        if (!findElementByTagAndAttValue(root, std::string("ros_service_handle_response"), std::string("name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), responseParent)) 
        {
            std::cerr << "No ros_service_handle_response element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            // return false;
        }
        if(responseParent) 
        {
            add_to_log("responseParent: " + std::string(responseParent->Name()) + " at line " + std::to_string(__LINE__));
        }
        if (!getInterfaceFieldsFromAssignTag(responseParent, eventData.interfaceResponseFields))
        {
            std::cerr << "Failed to get interface ros_service_handle_response fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            // return false;
        }

        return true;
    }



    // ROS TOPIC PUBLISHER
    bool is_topic_publisher = findElementByTagAndAttValue(root, std::string("ros_topic_publisher"), std::string("topic"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_topic_publisher: " + std::to_string(is_topic_publisher) + " at line " + std::to_string(__LINE__));
    if (is_topic_publisher) {

        if (!getElementAttValue(element, std::string("type"), eventData.messageInterfaceType))
        {
            std::cerr << "No type attribute found in ros_topic_publisher element for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        
        eventData.interfaceName = eventData.messageInterfaceType.substr(0, eventData.messageInterfaceType.find_last_of("/"));

        add_to_log("interfaceName: " + eventData.interfaceName + " at line " + std::to_string(__LINE__));
        
        eventData.interfaceType = "topic";
        eventData.rosInterfaceType = "topic-publisher"; // type of the interface in ROS
        getElementAttValue(element, std::string("name"), eventData.scxmlInterfaceName);
        getElementAttValue(element, std::string("topic"), eventData.topicName);
        tinyxml2::XMLElement* fieldParent;

        //get the fields from the ros_topic_publish element
        findElementByTagAndAttValue(root, std::string("ros_topic_publish"), std::string("name"), std::string(eventData.scxmlInterfaceName), fieldParent);
        if (!fieldParent) {
            std::cerr << "No ros_topic_publish element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        eventData.interfaceName = eventData.messageInterfaceType.substr(0, eventData.messageInterfaceType.find_last_of("/"));
        add_to_log("interfaceName: " + eventData.interfaceName + " at line " + std::to_string(__LINE__));

        if (!getInterfaceFieldsFromFieldTag(fieldParent, eventData.interfaceTopicFields)) {
            std::cerr << "Failed to get interface topic fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        return true;
    }



    // ROS TOPIC SUBSCRIBER
    // For topic subscribers, we need to reconstruct the full topic name from the event
    // Event format: componentName.functionName.topicPart.Sub
    // We need to extract the topicPart and reconstruct: /componentName/functionName/topicPart
    std::string fullTopicName = "/" + eventData.componentName + "/" + eventData.functionName;
    if (eventData.eventName.find(".Sub") != std::string::npos) {
        std::string topicPart = eventData.eventName.substr(0, eventData.eventName.find(".Sub"));
        if (!topicPart.empty()) {
            fullTopicName += "/" + topicPart;
        }
    }
    
    bool is_topic_subscriber = findElementByTagAndAttValue(root, std::string("ros_topic_subscriber"), std::string("topic"), fullTopicName, element);
    add_to_log("is_topic_subscriber: " + std::to_string(is_topic_subscriber) + " for topic: " + fullTopicName + " at line " + std::to_string(__LINE__));
    if (is_topic_subscriber) {
        eventData.interfaceType = "topic";
        eventData.rosInterfaceType = "topic-subscriber"; // type of the interface in ROS
        getElementAttValue(element, std::string("type"), eventData.messageInterfaceType);
        getElementAttValue(element, std::string("topic"), eventData.topicName);
        getElementAttValue(element, std::string("name"), eventData.scxmlInterfaceName);
        tinyxml2::XMLElement* fieldParent;

        // For topic subscribers, derive a better function name from the topic name
        // Extract the last part of the topic path as the function identifier
        std::string topicFunctionName = eventData.topicName;
        if (!topicFunctionName.empty() && topicFunctionName[0] == '/') {
            topicFunctionName = topicFunctionName.substr(1); // Remove leading slash
        }
        size_t lastSlash = topicFunctionName.find_last_of("/");
        if (lastSlash != std::string::npos) {
            topicFunctionName = topicFunctionName.substr(lastSlash + 1);
        }
        // Update functionName to use the derived topic function name for better naming
        if (!topicFunctionName.empty()) {
            eventData.functionName = topicFunctionName;
            turnToSnakeCase(eventData.functionName, eventData.functionNameSnakeCase);
            // Update other derived fields that depend on functionName
            eventData.nodeName = "node" + eventData.functionName;
            eventData.clientName = "client" + eventData.functionName;
        }

        eventData.interfaceName = eventData.messageInterfaceType.substr(0, eventData.messageInterfaceType.find_last_of("/"));
        add_to_log("interfaceName: " + eventData.interfaceName + " at line " + std::to_string(__LINE__));

        //get the fields from the ros_topic_subscribe element
        if (!findElementByTagAndAttValue(root, std::string("ros_topic_callback"), std::string("name"), std::string(eventData.topicName), fieldParent)){
            std::cerr << "No ros_topic_callback element found for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }
        if(!getInterfaceFieldsFromAssignTag(fieldParent, eventData.interfaceTopicFields)) {
            std::cerr << "Failed to get interface topic fields for component '" << eventData.componentName << "' and function '" << eventData.functionName << "' in file '" << fileData.inputFileName << "'."<< std::endl;
            return false;
        }


        return true;
    }




    bool is_action_server = findElementByTagAndAttValue(root, std::string("ros_action_server"), std::string("action_name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_action_server: " + std::to_string(is_action_server) + " at line " + std::to_string(__LINE__));
    if (is_action_server) {
        getElementAttValue(element, std::string("type"), eventData.messageInterfaceType);
        eventData.rosInterfaceType = "action-server"; // type of the interface in ROS
        eventData.interfaceName = eventData.messageInterfaceType.substr(0, eventData.messageInterfaceType.find_last_of("/"));
        
        add_to_log("interfaceName: " + eventData.interfaceName + " at line " + std::to_string(__LINE__));
        eventData.interfaceType = "action";
        // eventData.serverName = "/" + eventData.componentName + "/" + eventData.functionName;
        return true;
    }




    bool is_action_client = findElementByTagAndAttValue(root, std::string("ros_action_client"), std::string("action_name"), std::string("/" + eventData.componentName + "/" + eventData.functionName), element);
    add_to_log("is_action_client: " + std::to_string(is_action_client) + " at line " + std::to_string(__LINE__));
    if (is_action_client) {
        getElementAttValue(element, std::string("type"), eventData.messageInterfaceType);
        eventData.rosInterfaceType = "action-client"; // type of the interface in ROS
        eventData.interfaceName = eventData.messageInterfaceType.substr(0, eventData.messageInterfaceType.find_last_of("/"));
        add_to_log("interfaceName: " + eventData.interfaceName + " at line " + std::to_string(__LINE__));
        eventData.interfaceType = "action";
        eventData.clientName = "/" + eventData.componentName + "/" + eventData.functionName;
        return true;
    }

    return false; // if no interface type is found, return false
}


bool getInterfaceFieldsFromAssignTag(tinyxml2::XMLElement* element, std::vector<std::string>& interfaceFields)
{
    if (!element) {
        std::cerr << "Element is null" << std::endl;
        return false;
    }
    
    tinyxml2::XMLElement* fieldElement = element->FirstChildElement("assign");
    if (!fieldElement) {
        std::cerr << "No assign element found in the provided element" << std::endl;
        return false;
    }
    while (fieldElement) {
        const char* fieldName = fieldElement->Attribute("expr");
        add_to_log("fieldName: " + std::string(fieldName) + " at line " + std::to_string(__LINE__));
        if (fieldName) {
            std::cerr << "Found field: " << fieldName << std::endl;
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


bool getInterfaceFieldsFromFieldTag(tinyxml2::XMLElement* element, std::vector<std::string>& interfaceFields)
{
    if (element == nullptr) {
        std::cerr << "Element is null" << std::endl;
        return false;
    }
    
    // get the first child element
    tinyxml2::XMLElement* fieldElement = element->FirstChildElement("field");
    if (!fieldElement) {
        std::cerr << "No field element found in the provided element" << std::endl;
        return false;
    }
    while (fieldElement) {
        const char* fieldName = fieldElement->Attribute("name");
        add_to_log("fieldName: " + std::string(fieldName) + " at line " + std::to_string(__LINE__));
        if (fieldName) {
            std::cerr << "Found field: " << fieldName << std::endl;
            interfaceFields.push_back(fieldName);
        }
        fieldElement = fieldElement->NextSiblingElement("expr");
    }
    return true;
}

bool parseInterfaceFile(eventDataStr& eventData)
{
    // Construct the path to the .srv or .msg file based on messageInterfaceType
    // messageInterfaceType format: "scheduler_interfaces/GetCurrentPoi"
    
    if (eventData.messageInterfaceType.empty()) {
        std::cerr << "messageInterfaceType is empty" << std::endl;
        return false;
    }
    
    // Find the last slash to separate package from interface name
    size_t lastSlash = eventData.messageInterfaceType.find_last_of("/");
    if (lastSlash == std::string::npos) {
        std::cerr << "Invalid messageInterfaceType format: " << eventData.messageInterfaceType << std::endl;
        return false;
    }
    
    std::string packageName = eventData.messageInterfaceType.substr(0, lastSlash);
    std::string interfaceName = eventData.messageInterfaceType.substr(lastSlash + 1);
    
    // Determine interface file type and construct path
    std::string srvFilePath;
    if (eventData.interfaceType == "async-service" || eventData.interfaceType == "sync-service") {
        srvFilePath = "./test_compilation/interfaces/" + packageName + "/srv/" + interfaceName + ".srv";
    } else if (eventData.interfaceType == "topic") {
        srvFilePath = "./test_compilation/interfaces/" + packageName + "/msg/" + interfaceName + ".msg";
    } else {
        std::cerr << "Unknown interface type: " << eventData.interfaceType << std::endl;
        return false;
    }
    
    // Open and parse the interface file
    std::ifstream file(srvFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open interface file: " << srvFilePath << std::endl;
        return false;
    }
    
    std::string line;
    bool inResponseSection = false;
    
    // For .srv files, we want the response section (after "---")
    // For .msg files, we want all fields
    bool isServiceFile = (eventData.interfaceType == "async-service" || eventData.interfaceType == "sync-service");
    
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line.substr(0, 2) == "//") {
            continue;
        }
        
        // For .srv files, look for the response separator
        if (isServiceFile && line == "---") {
            inResponseSection = true;
            continue;
        }
        
        // Parse field lines (for .msg files, parse all; for .srv files, parse only response section)
        if (!isServiceFile || inResponseSection) {
            // Expected format: "type field_name"
            std::istringstream iss(line);
            std::string fieldType, fieldName;
            
            if (iss >> fieldType >> fieldName) {
                eventData.interfaceData[fieldName] = fieldType;
                add_to_log("Parsed field: " + fieldName + " -> " + fieldType + " at line " + std::to_string(__LINE__));
            }
        }
    }
    
    file.close();
    return true;
}


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