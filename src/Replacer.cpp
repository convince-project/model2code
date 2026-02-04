// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file Replacer.cpp
 * @brief This file contains the functions to replace the placeholders in the template files with the data from the input file.
 * @version 0.1
 * @date 2024-05-24
 * 
 */
#include "Replacer.h"
#include "Data.h"
#include <filesystem>

#define m2cVersion M2C_VERSION

/**
 * @brief Get the Event Data from the model and interface files 
 * 
 * @param fileData file data structure passed by reference from which the event data is extracted
 * @param eventData event data structure passed by reference where the event data is stored
 */
bool getEventData(fileDataStr fileData, eventDataStr& eventData)
{
    if(eventsMap.find(eventData.event) != eventsMap.end()){
        add_to_log("Event already processed: " + eventData.event);
        return true;
    } 
    add_to_log("Processing event: " + eventData.event);
    eventsMap[eventData.event];

    if(eventData.event != cmdTick && eventData.event != cmdHalt && eventData.event != rspTick && eventData.event != rspHalt)
    {
        getDataFromEvent(eventData);
        
        eventData.nodeName = "node" + eventData.functionName;
        eventData.clientName = "client"+ eventData.functionName;
        turnToSnakeCase(eventData.functionName,eventData.functionNameSnakeCase);
        eventData.serverName = "\"/"+ eventData.componentName +"/" + eventData.functionName + "\"";


        // need to extract the interface name and type from the model file
        if(!extractInterfaceData(fileData, eventData))
        {
            std::cerr << "Error extracting interface data for event: " << eventData.event << std::endl;
            return false;
        } 

        // printEventData(eventData);
        // printEventData(eventData);
        // if(extractInterfaceName(fileData.modelFileName, eventData))
        // {
        //     if(!extractInterfaceType(fileData.interfaceFileName, eventData))
        //     {
        //         return false;
        //     }
        //     printEventData(eventData);
        // }
        // else
        // {
        //     return false;
        // }        
    }
    eventsMap[eventData.event] = eventData;
    return true;
}

/**
 * @brief Get the Event Data from the vector of event elements
 * 
 * @param fileData file data structure passed by reference where the file data is stored
 * @param elementsTransition vector of transition event elements
 * @param elementsSend vector of send event elements
 */
bool getEventsVecData(fileDataStr fileData, const std::vector<tinyxml2::XMLElement*> elementsTransition, const std::vector<tinyxml2::XMLElement*> elementsSend)
{
    for (const auto& element : elementsTransition) {
        const char* event = element->Attribute("event");
        const char* target = element->Attribute("target");
    
        if (event && target) 
        {

            add_to_log("Transition: event=" + std::string(event) + ", target=" + std::string(target));
            eventDataStr eventData;
            eventData.target = target;
            eventData.event = event;
            eventData.eventType = "transition";
            if(!getEventData(fileData, eventData))
            {
                return false;
            }
        } 
        else
        {
            std::cerr << "\tMissing attribute in <transition> tag" << std::endl;
        }
    }
    for (const auto& element : elementsSend) {
        const char* event = element->Attribute("event");

        if (event) 
        {
            add_to_log("Send: event=" + std::string(event));
            eventDataStr eventData;
            eventData.event = event;
            eventData.eventType = "send";
            std::vector<tinyxml2::XMLElement*> elementsParam;
            findElementVectorByTag(element, "param", elementsParam);
            for (const auto& element : elementsParam) {
                std::string paramName, paramExpr;
                getElementAttValue(element, "name", paramName);
                getElementAttValue(element, "expr", paramExpr);
                eventData.paramMap[paramName] = paramExpr;
                add_to_log("\tparamName=" + paramName + ", paramExpr=" + eventData.paramMap[paramName]);
            }
            if(!getEventData(fileData, eventData))
            {
                return false;
            }
        } 
        else{
            std::cerr << "\tMissing attribute in <send> tag" << std::endl;
        }
    }
    return true;

}

/**
 * @brief function to keep or delete the sections of the code related to the Command Tick event
 * 
 * @param code string of code to be modified
 * @param keepFlag flag to keep or delete the sections
 */
void handleCmdTickEvent(std::string& code,const bool keepFlag)
{
    if(keepFlag){
        keepSection(code, "/*TICK_CMD*/", "/*END_TICK_CMD*/");
        keepSection(code, "/*TICK*/", "/*END_TICK*/");
        keepSection(code, "#TICK#", "#END_TICK#");
        keepSection(code, "<!--TICK-->", "<!--END_TICK-->");
    } else{
        deleteSection(code, "/*TICK_CMD*/", "/*END_TICK_CMD*/");
        deleteSection(code, "/*TICK*/", "/*END_TICK*/");
        deleteSection(code, "#TICK#", "#END_TICK#");
        deleteSection(code, "<!--TICK-->", "<!--END_TICK-->");
    }
}

/**
 * @brief function to keep or delete the sections of the code related to the Command Halt event
 * 
 * @param code string of code to be modified
 * @param keepFlag flag to keep or delete the sections
 */
void handleCmdHaltEvent(std::string& code,const bool keepFlag)
{
    if(keepFlag){
        keepSection(code, "/*HALT_CMD*/", "/*END_HALT_CMD*/");
        keepSection(code, "/*HALT*/", "/*END_HALT*/");
        keepSection(code, "#HALT#", "#END_HALT#");
        keepSection(code, "<!--HALT-->", "<!--END_HALT-->");
    } else{
        deleteSection(code, "/*HALT_CMD*/", "/*END_HALT_CMD*/");
        deleteSection(code, "/*HALT*/", "/*END_HALT*/");
        deleteSection(code, "#HALT#", "#END_HALT#");
        deleteSection(code, "<!--HALT-->", "<!--END_HALT-->");
    }
}

/**
 * @brief function to keep or delete the sections of the code related to the Response Tick event
 * 
 * @param code string of code to be modified
 * @param keepFlag flag to keep or delete the sections
 */
void handleRspTickEvent(std::string& code,const bool keepFlag)
{
    if(keepFlag){
        keepSection(code, "/*TICK_RESPONSE*/", "/*END_TICK_RESPONSE*/");
        keepSection(code, "#TICK_RESPONSE#", "#END_TICK_RESPONSE#");
        keepSection(code, "<!--TICK_RESPONSE-->", "<!--END_TICK_RESPONSE-->");
    } else{
        deleteSection(code, "/*TICK_RESPONSE*/", "/*END_TICK_RESPONSE*/");
        deleteSection(code, "#TICK_RESPONSE#", "#END_TICK_RESPONSE#");
        deleteSection(code, "<!--TICK_RESPONSE-->", "<!--END_TICK_RESPONSE-->");
    } 
}

/**
 * @brief function to keep or delete the sections of the code related to the Response Halt event
 * 
 * @param code string of code to be modified
 * @param keepFlag flag to keep or delete the sections
 */
void handleRspHaltEvent(std::string& code,const bool keepFlag)
{
    if(keepFlag){
        keepSection(code, "/*HALT_RESPONSE*/", "/*END_HALT_RESPONSE*/");
        keepSection(code, "#HALT_RESPONSE#", "#END_HALT_RESPONSE#");
        keepSection(code, "<!--HALT_RESPONSE-->", "<!--END_HALT_RESPONSE-->");
    } else{
        deleteSection(code, "/*HALT_RESPONSE*/", "/*END_HALT_RESPONSE*/");
        deleteSection(code, "#HALT_RESPONSE#", "#END_HALT_RESPONSE#");
        deleteSection(code, "<!--HALT_RESPONSE-->", "<!--END_HALT_RESPONSE-->");
    } 
}

/**
 * @brief function that event placeholders in the code
 * 
 * @param code string of code to be modified
 * @param eventData event data structure containing event information
 */
void replaceCommonEventPlaceholders(std::string& code, const eventDataStr& eventData) {
    replaceAll(code, "$eventData.event$", eventData.event);
    replaceAll(code, "$eventData.componentName$", eventData.componentName);
    replaceAll(code, "$eventData.functionName$", eventData.functionName);
    replaceAll(code, "$eventData.serviceTypeName$", eventData.serviceTypeName);
    replaceAll(code, "$eventData.serviceTypeNameSnakeCase$", eventData.serviceTypeNameSnakeCase);
    replaceAll(code, "$eventData.nodeName$", eventData.nodeName);
    replaceAll(code, "$eventData.serverName$", eventData.serverName);
    replaceAll(code, "$eventData.clientName$", eventData.clientName);
    replaceAll(code, "$eventData.interfaceName$", eventData.interfaceName);
    replaceAll(code, "$eventData.topicName$", eventData.topicName);
}


/**
 * @brief function to write the generic event code in the output string
 * 
 * @param eventData event data structure containing event information
 * @param savedCode saved code structure containing the saved code to be written
 * @param str output string to be modified
 */
void handleGenericEvent(const eventDataStr eventData, const savedCodeStr savedCode, std::string& str)
{
    printEventData(eventData);
    if(eventData.eventType == "send"){
        std::string interfaceCodeH = savedCode.interfaceH;
        std::string actionInterfaceH = savedCode.actionInterfaceH;
        std::string interfaceCodeCMake = savedCode.interfaceCMake;
        std::string packageCodeCMake = savedCode.packageCMake;
        std::string interfaceCodeXML = savedCode.interfaceXML;
        if(eventData.interfaceType == "async-service" || eventData.interfaceType == "sync-service")
        {
            std::string eventCodeC = savedCode.eventC;
            //CPP
            replaceCommonEventPlaceholders(eventCodeC, eventData);
            for (auto itParam =  eventData.paramMap.begin(); itParam != eventData.paramMap.end(); ++itParam) 
            {
                std::string paramCode = savedCode.sendParam;
                replaceAll(paramCode, "$IT->FIRST$", itParam->first);
                writeAfterCommand(eventCodeC, "/*PARAM_LIST*/", paramCode);
            }

            // Process response fields using the mapping to datamodel variables
            for (auto responseField : eventData.interfaceResponseFields) 
            {
                std::string paramCode = savedCode.returnParam;
                
                // Use the response field name directly
                std::string fieldName = responseField;

                // Look up the datamodel variable for this response field
                std::string datamodelVar;
                auto mappingIt = eventData.responseFieldToDatamodelMap.find(responseField);
                if (mappingIt != eventData.responseFieldToDatamodelMap.end()) {
                    datamodelVar = mappingIt->second;
                } else {
                    // Fallback to the old logic if mapping is not found
                    datamodelVar = "m_" + fieldName;
                    std::cerr << "Warning: No mapping found for response field '" << responseField << "', using fallback '" << datamodelVar << "'" << std::endl;
                }

                // Get the type from interfaceData using the datamodel variable name
                std::string fieldType = "string";
                auto pos = eventData.interfaceData.find(datamodelVar);

                if (pos == eventData.interfaceData.end()) {
                    std::cerr << "Warning: Datamodel variable '" << datamodelVar << "' not found in interface data fields. No type info is available. Using default string" << std::endl;
                }
                else {
                    fieldType = pos->second;
                }
                
                // Create the field access expression
                std::string fieldAccess;
                if (fieldType == "string") {
                    fieldAccess = "response->" + fieldName + ".c_str()";
                } else {
                    fieldAccess = "response->" + fieldName;
                }

                // Replace both placeholders - first the field access, then the field name
                // We need to replace in a specific order to avoid conflicts
                replaceAll(paramCode, "response->$eventData.interfaceDataField$", fieldAccess);
                replaceAll(paramCode, "$eventData.interfaceDataField$", fieldName);
                
                writeAfterCommand(eventCodeC, "/*RETURN_PARAM_LIST*/", paramCode);
            }
            // if(eventData.rosInterfaceType == "service-server")
            // {

            // } else if (eventData.rosInterfaceType == "service-client")
            // {
            
            for (auto itParam = eventData.interfaceRequestFields.begin(); itParam != eventData.interfaceRequestFields.end(); ++itParam) 
            {
                std::string paramCode = savedCode.returnParam;
                replaceAll(paramCode, "$eventData.interfaceDataField$", *itParam);
                writeAfterCommand(eventCodeC, "/*SEND_PARAM_LIST*/", paramCode);
            }
            // }


            writeAfterCommand(str, "/*SEND_EVENT_LIST*/", eventCodeC);
            
            // Service client header code
            std::string serviceClientH = savedCode.serviceClientH;
            replaceCommonEventPlaceholders(serviceClientH, eventData);
            writeAfterCommand(str, "/*SERVICE_CLIENTS_LIST*/", serviceClientH);
            
            //H
            replaceAll(interfaceCodeH, "$eventData.interfaceName$", eventData.interfaceName);
            replaceAll(interfaceCodeH, "$eventData.functionNameSnakeCase$", eventData.functionNameSnakeCase);
            replaceAll(interfaceCodeH, "$eventData.serviceTypeNameSnakeCase$", eventData.serviceTypeNameSnakeCase);
            writeAfterCommand(str, "/*INTERFACES_LIST*/", interfaceCodeH);
            
            //CMakeLists.txt
            replaceAll(interfaceCodeCMake, "$interfaceName$", eventData.interfaceName);
            if(!checkIfStrPresent(str, interfaceCodeCMake)){
                writeAfterCommand(str, "#INTERFACE_LIST#", interfaceCodeCMake);
            }
            replaceAll(packageCodeCMake, "$interfaceName$", eventData.interfaceName);
            if(!checkIfStrPresent(str, packageCodeCMake)){
                writeAfterCommand(str, "#PACKAGE_LIST#", packageCodeCMake);
            }

            //package.xml
            replaceAll(interfaceCodeXML, "$interfaceName$", eventData.interfaceName);
            if(!checkIfStrPresent(str, interfaceCodeXML)){
                writeAfterCommand(str, "<!--INTERFACE_LIST-->", interfaceCodeXML);
            }

        }
        else if(eventData.interfaceType == "action")
        {
            //If event is Send Goal replace tags in savedCode.actionSendGoalLambda
            //If event is Send Cancel replace tags in savedCode.actionSendCancelLambda
            std::string eventSendGoal = "SendGoal";
            if (std::string(eventData.eventName).find(eventSendGoal) != std::string::npos) {
                std::string actionSendGoalLambda = savedCode.actionSendGoalLambda;
                replaceCommonEventPlaceholders(actionSendGoalLambda, eventData);
                writeAfterCommand(str, "/*ACTION_LAMBDA_LIST*/", actionSendGoalLambda);

                std::string actionSendGoalFnc = savedCode.actionSendGoalFnc;
                replaceCommonEventPlaceholders(actionSendGoalFnc, eventData);
                writeAfterCommand(str, "/*ACTION_FNC_LIST*/", actionSendGoalFnc);

                std::string actionCodeH = savedCode.actionH;
                replaceCommonEventPlaceholders(actionCodeH, eventData);
                writeAfterCommand(str, "/*ACTION_LIST_H*/", actionCodeH);

                std::string actionCodeC = savedCode.actionC;
                replaceCommonEventPlaceholders(actionCodeC, eventData);
                writeAfterCommand(str, "/*ACTION_LIST_C*/", actionCodeC);

                //H
                replaceAll(actionInterfaceH, "$eventData.interfaceName$", eventData.interfaceName);
                replaceAll(actionInterfaceH, "$eventData.functionNameSnakeCase$", eventData.functionNameSnakeCase);
                writeAfterCommand(str, "/*INTERFACES_LIST*/", actionInterfaceH);
                //CMakeLists.txt
                replaceAll(interfaceCodeCMake, "$interfaceName$", eventData.interfaceName);
                if(!checkIfStrPresent(str, interfaceCodeCMake)){
                    writeAfterCommand(str, "#INTERFACE_LIST#", interfaceCodeCMake);
                }
                replaceAll(packageCodeCMake, "$interfaceName$", eventData.interfaceName);
                if(!checkIfStrPresent(str, packageCodeCMake)){
                    writeAfterCommand(str, "#PACKAGE_LIST#", packageCodeCMake);
                }

                //package.xml
                replaceAll(interfaceCodeXML, "$interfaceName$", eventData.interfaceName);
                if(!checkIfStrPresent(str, interfaceCodeXML)){
                    writeAfterCommand(str, "<!--INTERFACE_LIST-->", interfaceCodeXML);
                }

                for (auto itParam =  eventData.paramMap.begin(); itParam != eventData.paramMap.end(); ++itParam) 
                {
                    std::string paramCode = savedCode.actionSendParam;
                    replaceAll(paramCode, "$IT->FIRST$", itParam->first);
                    writeAfterCommand(str, "/*SEND_PARAM_LIST*/", paramCode);
                }
                
            }
        }
    }
    else if(eventData.eventType == "transition")
    {
        if(eventData.interfaceType == "sync-service" || eventData.interfaceType == "async-service")
        {
            // in this case I have a service response
        }
        if(eventData.interfaceType == "topic")
        {
            std::string topicCallbackC = savedCode.topicCallbackC;
            std::string topicCallbackH = savedCode.topicCallbackH;
            std::string topicSubscriptionC = savedCode.topicSubscriptionC;
            std::string topicSubscriptionH = savedCode.topicSubscriptionH;
            std::string topicInterfaceH = savedCode.topicInterfaceH;
            std::string interfaceCodeH = savedCode.interfaceH;
            std::string interfaceCodeCMake = savedCode.interfaceCMake;
            std::string packageCodeCMake = savedCode.packageCMake;
            std::string interfaceCodeXML = savedCode.interfaceXML;
            //CPP
            // replaceAll(topicCallbackC, "$eventData.interfaceData[interfaceDataField]$", eventData.interfaceData.begin()->first);
            for(auto it = eventData.interfaceTopicFields.begin(); it != eventData.interfaceTopicFields.end(); ++it)
            {
                std::string topicFieldCode = savedCode.topicParamList;
                replaceAll(topicFieldCode, "$eventData.interfaceDataField$", *it);
                writeAfterCommand(topicCallbackC, "/*TOPIC_PARAM_LIST*/", topicFieldCode);
            }
            replaceAll(topicSubscriptionC, "$eventData.interfaceName$", eventData.interfaceName);
            replaceAll(topicSubscriptionC, "$eventData.messageNameSnakeCase$", eventData.messageInterfaceType.substr(eventData.messageInterfaceType.find_last_of("/") + 1));
            replaceAll(topicCallbackC, "$eventData.interfaceName$", eventData.interfaceName);
            replaceAll(topicCallbackC, "$eventData.messageNameSnakeCase$", eventData.messageInterfaceType.substr(eventData.messageInterfaceType.find_last_of("/") + 1));
            replaceAll(topicCallbackC, "$eventData.functionName$", eventData.functionName);
            replaceAll(topicSubscriptionC, "$eventData.functionName$", eventData.functionName);
            replaceAll(topicSubscriptionC, "$eventData.topicName$", eventData.topicName);
            replaceAll(topicCallbackC, "$eventData.componentName$", eventData.componentName);
            writeAfterCommand(str, "/*TOPIC_SUBSCRIPTIONS_LIST*/", topicSubscriptionC);
            writeAfterCommand(str, "/*TOPIC_CALLBACK_LIST*/", topicCallbackC);
            //H
            std::string interface;
            getDataTypePath(eventData.interfaceName, interface);
            replaceAll(topicInterfaceH, "$eventData.interfaceName$", eventData.interfaceName);
            replaceAll(topicInterfaceH, "$eventData.messageNameSnakeCase$", turnToSnakeCase(eventData.messageInterfaceType.substr(eventData.messageInterfaceType.find_last_of("/") + 1)));
            replaceAll(topicCallbackH, "$eventData.interfaceName$", eventData.interfaceName);
            replaceAll(topicCallbackH, "$eventData.messageNameSnakeCase$", eventData.messageInterfaceType.substr(eventData.messageInterfaceType.find_last_of("/") + 1));
            replaceAll(topicSubscriptionH, "$eventData.interfaceName$", eventData.interfaceName);
            replaceAll(topicSubscriptionH, "$eventData.messageNameSnakeCase$", eventData.messageInterfaceType.substr(eventData.messageInterfaceType.find_last_of("/") + 1));
            replaceAll(topicCallbackH, "$eventData.functionName$", eventData.functionName);
            replaceAll(topicSubscriptionH, "$eventData.functionName$", eventData.functionName);
            writeAfterCommand(str, "/*INTERFACES_LIST*/", topicInterfaceH);
            writeAfterCommand(str, "/*TOPIC_SUBSCRIPTIONS_LIST_H*/", topicSubscriptionH);
            writeAfterCommand(str, "/*TOPIC_CALLBACK_LIST_H*/", topicCallbackH);

            //CMakeLists.txt
            // printEventData(eventData);
            // if (eventData.virtualInterface)
            // {

                // for(auto it =  eventData.interfaceData.begin(); it != eventData.interfaceData.end(); ++it)
                // {
                //     if (it->second.find("msg") != std::string::npos) 
                //     {
                //         std::string ros_package = it->second.substr(0, it->second.find("::msg::"));
                //         add_to_log("ROS package: " + ros_package);
                //         replaceAll(interfaceCodeCMake, "$interfaceName$", ros_package);
                //         if(!checkIfStrPresent(str, interfaceCodeCMake)){
                //             writeAfterCommand(str, "#INTERFACE_LIST#", interfaceCodeCMake);
                //         }
                //         replaceAll(packageCodeCMake, "$interfaceName$", ros_package);
                //         if(!checkIfStrPresent(str, packageCodeCMake)){
                //             writeAfterCommand(str, "#PACKAGE_LIST#", packageCodeCMake);
                //         }
                //     }
                // }
            // }
            
            //CMakeLists.txt
            replaceAll(interfaceCodeCMake, "$interfaceName$", eventData.interfaceName);
            if(!checkIfStrPresent(str, interfaceCodeCMake)){
                writeAfterCommand(str, "#INTERFACE_LIST#", interfaceCodeCMake);
            }

            replaceAll(packageCodeCMake, "$interfaceName$", eventData.interfaceName);
            if(!checkIfStrPresent(str, packageCodeCMake)){
                writeAfterCommand(str, "#PACKAGE_LIST#", packageCodeCMake);
            }

            //package.xml
            replaceAll(interfaceCodeXML, "$interfaceName$", eventData.interfaceName);
            if(!checkIfStrPresent(str, interfaceCodeXML)){
                writeAfterCommand(str, "<!--INTERFACE_LIST-->", interfaceCodeXML);
            }
            // replaceAll(interfaceCodeCMake, "$interfaceName$", eventData.interfaceName);
            // if(!checkIfStrPresent(str, interfaceCodeCMake)){
            //     writeAfterCommand(str, "#INTERFACE_LIST#", interfaceCodeCMake);
            // }
            // replaceAll(packageCodeCMake, "$interfaceName$", eventData.interfaceName);
            // if(!checkIfStrPresent(str, packageCodeCMake)){
            //     writeAfterCommand(str, "#PACKAGE_LIST#", packageCodeCMake);
            // }

            //package.xml
            // replaceAll(interfaceCodeXML, "$interfaceName$", eventData.interfaceName);
            // if(!checkIfStrPresent(str, interfaceCodeXML)){
            //     writeAfterCommand(str, "<!--INTERFACE_LIST-->", interfaceCodeXML);
            // }
        }
        else if(eventData.interfaceType == "action")
        {
            //If event is Goal Response replace tags in savedCode.actionGoalResponseFnc 
            //If event is Feedback Return replace tags in savedCode.actionFeedbackCallback, e quelli di actionFeedbackLambda?
            //If event is Result Response replace tags in savedCode.actionResultCallbackFnc
            std::string eventFeedbackReturn = "FeedbackReturn";
            std::string eventGoalResponse = "GoalResponse";
            std::string eventResultResponse = "ResultResponse";
            if (std::string(eventData.eventName).find(eventFeedbackReturn) != std::string::npos)
            {
                std::string actionFeedbackCallback = savedCode.actionFeedbackCallback;
                replaceCommonEventPlaceholders(actionFeedbackCallback, eventData);
                writeAfterCommand(str, "/*ACTION_FNC_LIST*/", actionFeedbackCallback);

                std::string actionFeedbackLambda = savedCode.actionFeedbackLambda;
                replaceCommonEventPlaceholders(actionFeedbackLambda, eventData);
                writeAfterCommand(str, "/*ACTION_LAMBDA_LIST*/", actionFeedbackLambda);
                for (auto itParam =  eventData.interfaceData.begin(); itParam != eventData.interfaceData.end(); ++itParam) 
                {
                    std::string feedbackParamCode = savedCode.actionFeedbackParam;
                    replaceAll(feedbackParamCode, "$eventData.interfaceDataField$", itParam->first);
                    writeAfterCommand(str, "/*FEEDBACK_PARAM_LIST*/", feedbackParamCode);
                }
                for (auto itParam =  eventData.interfaceData.begin(); itParam != eventData.interfaceData.end(); ++itParam) 
                {
                    std::string feedbackParamCodeFnc = savedCode.actionFeedbackParamFnc;
                    replaceAll(feedbackParamCodeFnc, "$eventData.interfaceDataField$", itParam->first);
                    writeAfterCommand(str, "/*FEEDBACK_PARAM_LIST_FNC*/", feedbackParamCodeFnc);
                }
                for (auto itParam =  eventData.interfaceData.begin(); itParam != eventData.interfaceData.end(); ++itParam) 
                {
                    std::string feedbackData = savedCode.actionFeedbackData;
                    replaceAll(feedbackData, "$eventData.interfaceDataField$", itParam->first);
                    replaceAll(feedbackData, "$eventData.interfaceDataType$", itParam->second);
                    writeAfterCommand(str, "/*FEEDBACK_DATA_LIST*/", feedbackData);
                }

            }
            else if(std::string(eventData.eventName).find(eventGoalResponse) != std::string::npos)
            {
                std::string actionGoalResponseFnc = savedCode.actionGoalResponseFnc;
                replaceCommonEventPlaceholders(actionGoalResponseFnc, eventData);
                writeAfterCommand(str, "/*ACTION_FNC_LIST*/", actionGoalResponseFnc);
            }
            else if(std::string(eventData.eventName).find(eventResultResponse) != std::string::npos)
            {
                std::string actionResultCallbackFnc = savedCode.actionResultCallbackFnc;
                replaceCommonEventPlaceholders(actionResultCallbackFnc, eventData);
                writeAfterCommand(str, "/*ACTION_FNC_LIST*/", actionResultCallbackFnc);

                std::string actionResultRequestLambda = savedCode.actionResultRequestLambda;
                replaceCommonEventPlaceholders(actionResultRequestLambda, eventData);
                writeAfterCommand(str, "/*ACTION_LAMBDA_LIST*/", actionResultRequestLambda);

            }
        }
    }

}

/**
 * @brief function to save specific sections of the code in the saved code structure
 * 
 * @param savedCode saved code structure passed by reference where the saved code is stored
 * @param code code string where the code is extracted and then deleted
 */
void saveCode(savedCodeStr& savedCode, std::string& code)
{
    //CPP
    saveSection(code, "/*PARAM*/", "/*END_PARAM*/", savedCode.sendParam);
    deleteSection(code, "/*PARAM*/", "/*END_PARAM*/");
    saveSection(code, "/*RETURN_PARAM*/", "/*END_RETURN_PARAM*/", savedCode.returnParam);
    deleteSection(code, "/*RETURN_PARAM*/", "/*END_RETURN_PARAM*/");
    saveSection(code, "/*TOPIC_PARAM*/", "/*END_TOPIC_PARAM*/", savedCode.topicParamList);
    deleteSection(code, "/*TOPIC_PARAM*/", "/*END_TOPIC_PARAM*/");
    saveSection(code, "/*SEND_EVENT_SRV*/", "/*END_SEND_EVENT_SRV*/", savedCode.eventC);
    deleteSection(code, "/*SEND_EVENT_SRV*/", "/*END_SEND_EVENT_SRV*/");
    saveSection(code, "/*TOPIC_CALLBACK*/", "/*END_TOPIC_CALLBACK*/", savedCode.topicCallbackC);
    deleteSection(code, "/*TOPIC_CALLBACK*/", "/*END_TOPIC_CALLBACK*/");
    saveSection(code, "/*TOPIC_SUBSCRIPTION*/", "/*END_TOPIC_SUBSCRIPTION*/", savedCode.topicSubscriptionC);
    deleteSection(code, "/*TOPIC_SUBSCRIPTION*/", "/*END_TOPIC_SUBSCRIPTION*/");
    saveSection(code, "/*ACTION_C*/", "/*END_ACTION_C*/", savedCode.actionC);
    deleteSection(code, "/*ACTION_C*/", "/*END_ACTION_C*/");
    saveSection(code, "/*SEND_PARAM*/", "/*END_SEND_PARAM*/", savedCode.actionSendParam);
    deleteSection(code, "/*SEND_PARAM*/", "/*END_SEND_PARAM*/");
    saveSection(code, "/*FEEDBACK_PARAM*/", "/*END_FEEDBACK_PARAM*/", savedCode.actionFeedbackParam);
    deleteSection(code, "/*FEEDBACK_PARAM*/", "/*END_FEEDBACK_PARAM*/");
    saveSection(code, "/*FEEDBACK_PARAM_FNC*/", "/*END_FEEDBACK_PARAM_FNC*/", savedCode.actionFeedbackParamFnc);
    deleteSection(code, "/*FEEDBACK_PARAM_FNC*/", "/*END_FEEDBACK_PARAM_FNC*/");
    saveSection(code, "/*FEEDBACK_DATA*/", "/*END_FEEDBACK_DATA*/", savedCode.actionFeedbackData);
    deleteSection(code, "/*FEEDBACK_DATA*/", "/*END_FEEDBACK_DATA*/");
    saveSection(code, "/*ACTION_SEND_GOAL*/", "/*END_ACTION_SEND_GOAL*/", savedCode.actionSendGoalLambda);
    deleteSection(code, "/*ACTION_SEND_GOAL*/", "/*END_ACTION_SEND_GOAL*/");
    saveSection(code, "/*ACTION_RESULT_REQUEST*/", "/*END_ACTION_RESULT_REQUEST*/", savedCode.actionResultRequestLambda);
    deleteSection(code, "/*ACTION_RESULT_REQUEST*/", "/*END_ACTION_RESULT_REQUEST*/");
    saveSection(code, "/*ACTION_FEEDBACK*/", "/*END_ACTION_FEEDBACK*/", savedCode.actionFeedbackLambda);
    deleteSection(code, "/*ACTION_FEEDBACK*/", "/*END_ACTION_FEEDBACK*/");
    saveSection(code, "/*ACTION_SEND_GOAL_FNC*/", "/*END_ACTION_SEND_GOAL_FNC*/", savedCode.actionSendGoalFnc);
    deleteSection(code, "/*ACTION_SEND_GOAL_FNC*/", "/*END_ACTION_SEND_GOAL_FNC*/");
    saveSection(code, "/*ACTION_RESULT_CALLBACK_FNC*/", "/*END_ACTION_RESULT_CALLBACK_FNC*/", savedCode.actionResultCallbackFnc);
    deleteSection(code, "/*ACTION_RESULT_CALLBACK_FNC*/", "/*END_ACTION_RESULT_CALLBACK_FNC*/");
    saveSection(code, "/*ACTION_FEEDBACK_FNC*/", "/*END_ACTION_FEEDBACK_FNC*/", savedCode.actionFeedbackCallback);
    deleteSection(code, "/*ACTION_FEEDBACK_FNC*/", "/*END_ACTION_FEEDBACK_FNC*/");
    saveSection(code, "/*ACTION_RESPONSE_CALLBACK_FNC*/", "/*END_ACTION_RESPONSE_CALLBACK_FNC*/", savedCode.actionGoalResponseFnc);
    deleteSection(code, "/*ACTION_RESPONSE_CALLBACK_FNC*/", "/*END_ACTION_SEND_GOAL_FNC*/");

    //H
    saveSection(code, "/*INTERFACE*/", "/*END_INTERFACE*/", savedCode.interfaceH);
    deleteSection(code, "/*INTERFACE*/", "/*END_INTERFACE*/");
    saveSection(code, "/*TOPIC_INTERFACE*/", "/*END_TOPIC_INTERFACE*/", savedCode.topicInterfaceH);
    deleteSection(code, "/*TOPIC_INTERFACE*/", "/*END_TOPIC_INTERFACE*/");
    saveSection(code, "/*TOPIC_CALLBACK_H*/", "/*END_TOPIC_CALLBACK_H*/", savedCode.topicCallbackH);
    deleteSection(code, "/*TOPIC_CALLBACK_H*/", "/*END_TOPIC_CALLBACK_H*/");
    saveSection(code, "/*TOPIC_SUBSCRIPTION_H*/", "/*END_TOPIC_SUBSCRIPTION_H*/", savedCode.topicSubscriptionH);
    deleteSection(code, "/*TOPIC_SUBSCRIPTION_H*/", "/*END_TOPIC_SUBSCRIPTION_H*/");
    saveSection(code, "/*SERVICE_CLIENT*/", "/*END_SERVICE_CLIENT*/", savedCode.serviceClientH);
    deleteSection(code, "/*SERVICE_CLIENT*/", "/*END_SERVICE_CLIENT*/");
    saveSection(code, "/*ACTION_H*/", "/*END_ACTION_H*/", savedCode.actionH);
    deleteSection(code, "/*ACTION_H*/", "/*END_ACTION_H*/");
    saveSection(code, "/*ACTION_INTERFACE*/", "/*END_ACTION_INTERFACE*/", savedCode.actionInterfaceH);
    deleteSection(code, "/*ACTION_INTERFACE*/", "/*END_ACTION_INTERFACE*/");
    //CMakeLists.txt
    saveSection(code, "#INTERFACE#", "#END_INTERFACE#", savedCode.interfaceCMake);
    deleteSection(code, "#INTERFACE#", "#END_INTERFACE#");
    saveSection(code, "#PACKAGE#", "#END_PACKAGE#", savedCode.packageCMake);
    deleteSection(code, "#PACKAGE#", "#END_PACKAGE#");
    //package.xml
    saveSection(code, "<!--INTERFACE-->", "<!--END_INTERFACE-->", savedCode.interfaceXML);
    deleteSection(code, "<!--INTERFACE-->", "<!--END_INTERFACE-->");
}

/**
 * @brief function to replace the event code in the code map
 * 
 * @param codeMap code map where the code is stored
 */
void replaceEventCode(std::map <std::string, std::string>& codeMap, fileDataStr fileData){
    savedCodeStr savedCode;
    for (auto it = codeMap.begin(); it != codeMap.end(); it++) {
        handleCmdTickEvent(it->second, eventsMap.find(cmdTick) != eventsMap.end());
        handleCmdHaltEvent(it->second, eventsMap.find(cmdHalt) != eventsMap.end());
        handleRspTickEvent(it->second, eventsMap.find(rspTick) != eventsMap.end());
        handleRspHaltEvent(it->second, eventsMap.find(rspHalt) != eventsMap.end());
        saveCode(savedCode, it->second);
        for (auto itEv = eventsMap.begin(); itEv != eventsMap.end(); itEv++)
        { 
            if(itEv->first != cmdTick && itEv->first != cmdHalt && itEv->first != rspTick && itEv->first != rspHalt){
                eventDataStr eventData = itEv->second;
                if(eventData.interfaceName == "blackboard_interfaces")
                {
                    eventData.functionNameSnakeCase += "_blackboard";
                }
                handleGenericEvent(eventData, savedCode, it->second);
            } 
        }
        //CPP
        deleteCommand(it->second, "/*SEND_EVENT_LIST*/");
        deleteCommand(it->second, "/*PARAM_LIST*/");
        deleteCommand(it->second, "/*RETURN_PARAM_LIST*/");
        deleteCommand(it->second, "/*TOPIC_SUBSCRIPTIONS_LIST*/");
        deleteCommand(it->second, "/*TOPIC_CALLBACK_LIST*/"); 
        deleteCommand(it->second, "/*TOPIC_PARAM_LIST*/");
        deleteCommand(it->second, "/*ACTION_LIST_C*/"); 
        deleteCommand(it->second, "/*SEND_PARAM_LIST*/");
        deleteCommand(it->second, "/*FEEDBACK_PARAM_LIST*/");
        deleteCommand(it->second, "/*FEEDBACK_PARAM_LIST_FNC*/");
        deleteCommand(it->second, "/*FEEDBACK_DATA_LIST*/");
        deleteCommand(it->second, "/*ACTION_LAMBDA_LIST*/"); 
        deleteCommand(it->second, "/*ACTION_FNC_LIST*/"); 
        //H
        deleteCommand(it->second, "/*INTERFACES_LIST*/");
        deleteCommand(it->second, "/*SERVICE_CLIENTS_LIST*/");
        deleteCommand(it->second, "/*TOPIC_SUBSCRIPTIONS_LIST_H*/");
        deleteCommand(it->second, "/*TOPIC_CALLBACK_LIST_H*/");
        deleteCommand(it->second, "/*ACTION_LIST_H*/");
        //CMakeLists.txt
        deleteCommand(it->second, "#INTERFACE_LIST#");
        deleteCommand(it->second, "#PACKAGE_LIST#");
        //package.xml
        deleteCommand(it->second, "<!--INTERFACE_LIST-->");
    }

}

/**
 * @brief function to read the template files and store the code in the code map
 * 
 * @param templateFileData template file data structure passed by reference where the template file data is stored
 * @param codeMap code map where the code is stored
 * @return true 
 * @return false 
 */
bool readTemplates(templateFileDataStr& templateFileData, std::map <std::string, std::string>& codeMap)
{
    templateFileData.hFile              = templateFileData.templatePath + hTemplateFileRelativePath;
    templateFileData.cppFile            = templateFileData.templatePath + cppTemplateFileRelativePath;
    templateFileData.hDatamodelFile     = templateFileData.templatePath + hDataModelTemplateFileRelativePath;
    templateFileData.cppDatamodelFile   = templateFileData.templatePath + cppDataModelTemplateFileRelativePath;
    templateFileData.CMakeListsFile     = templateFileData.templatePath + cMakeTemplateFileRelativePath;
    templateFileData.packageFile        = templateFileData.templatePath + pkgTemplateFileRelativePath;
    templateFileData.mainFile           = templateFileData.templatePath + mainTemplateFileRelativePath;
    bool res = true;
    res &= readTemplateFile(templateFileData.hFile, codeMap["hCode"]);
    res &= readTemplateFile(templateFileData.cppFile, codeMap["cppCode"]);
    res &= readTemplateFile(templateFileData.hDatamodelFile, codeMap["hDataModelCode"]);
    res &= readTemplateFile(templateFileData.cppDatamodelFile, codeMap["cppDataModelCode"]);
    res &= readTemplateFile(templateFileData.CMakeListsFile, codeMap["cmakeCode"]);
    res &= readTemplateFile(templateFileData.packageFile, codeMap["packageCode"]);
    res &= readTemplateFile(templateFileData.mainFile, codeMap["mainCode"]);
    return res;
}

/**
 * @brief function to create a directory
 * 
 * @param path path to the directory
 * @return true if the directory is created successfully
 */
bool createDirectory(const std::string& path) {
    namespace fs = std::filesystem;
    try {
        // Create the directory (and any intermediate directories, if necessary)
        if (fs::create_directories(path)) {
            add_to_log("Directory created successfully: " + path);
            return true;
        } else {
            add_to_log("Directory already exists or failed to create: " + path);
            return false;
        }
    } catch (const fs::filesystem_error& err) {
        std::cerr << "Filesystem error: " << err.what() << std::endl;
        return false;
    }
}




/**
 * @brief main function to get the code from template files and replace the placeholders with the data from the input file
 * 
 * @param fileData file data structure passed by reference where the file data is stored
 * @param templateFileData template file data structure passed by reference where the template file data is stored
 * @return true if the generation is successful
 */
bool Replacer(fileDataStr& fileData, templateFileDataStr& templateFileData)
{ 
    std::string rootName;
    skillDataStr skillData;
    std::map <std::string, std::string> codeMap;
    std::vector<tinyxml2::XMLElement *> elementsTransition, elementsSend;
    tinyxml2::XMLDocument doc;
    add_to_log("-----------");
    if(!extractFromSCXML(doc, fileData.inputFileNameGeneration, rootName, elementsTransition, elementsSend)){
        return 0;
    }
    
    if(!getDataFromRootName(rootName, skillData)){
        return 0;
    }
    printSkillData(skillData);
    setFileData(fileData, skillData);
    if(!readTemplates(templateFileData, codeMap))
    {
        return false;
    }

    // Get current datetime for autogenerated file headers
    std::string currentDatetime = getCurrentDatetime();
    
    for (auto it = codeMap.begin(); it != codeMap.end(); it++) {
        replaceAll(it->second, "$className$", skillData.className);
        replaceAll(it->second, "$projectName$", skillData.classNameSnakeCase);
        replaceAll(it->second, "$SMName$", skillData.SMName);
        replaceAll(it->second, "$skillName$", skillData.skillName);
        std::cout << "=================================skillType: " << skillData.skillType << std::endl;
        replaceAll(it->second, "$skillTypeLC$", skillData.skillTypeLC);
        replaceAll(it->second, "$skillType$", skillData.skillType);
        replaceAll(it->second, "$datetime$", currentDatetime);
        replaceAll(it->second, "$m2cVersion$", m2cVersion);

        if(fileData.is_action_skill){
            keepSection(it->second, "/*ACTION*/", "/*END_ACTION*/");
        }else{
            deleteSection(it->second, "/*ACTION*/", "/*END_ACTION*/");
        }
        if(fileData.datamodel_mode){
            keepSection(it->second, "/*DATAMODEL*/", "/*END_DATAMODEL*/");
            keepSection(it->second, "#DATAMODEL#", "#END_DATAMODEL#");
            std::string dataModelClassName = skillData.className + "DataModel";
            replaceAll(it->second, "$dataModelClassName$", dataModelClassName);
        }else{
            deleteSection(it->second, "/*DATAMODEL*/", "/*END_DATAMODEL*/");
            deleteSection(it->second, "#DATAMODEL#", "#END_DATAMODEL#");
        }
    }
    if (!getEventsVecData(fileData, elementsTransition, elementsSend))
    {
        return false;
    }
    replaceEventCode(codeMap, fileData);

    add_to_log("-----------");
    if(fileData.datamodel_mode)
    {
        writeFile(fileData.outputPathInclude, fileData.outputDatamodelFileNameH, codeMap["hDataModelCode"]);
        writeFile(fileData.outputPathSrc, fileData.outputDatamodelFileNameCPP, codeMap["cppDataModelCode"]);
    }
    add_to_log("-----------");
    createDirectory(fileData.outputPath);
    createDirectory(fileData.outputPathInclude);
    createDirectory(fileData.outputPathSrc);
    add_to_log("-----------");
    writeFile(fileData.outputPathInclude, fileData.outputFileNameH, codeMap["hCode"]);
    writeFile(fileData.outputPathSrc, fileData.outputFileNameCPP, codeMap["cppCode"]);
    writeFile(fileData.outputPath, fileData.outputCMakeListsFileName, codeMap["cmakeCode"]);
    writeFile(fileData.outputPath, fileData.outputPackageXMLFileName, codeMap["packageCode"]);
    writeFile(fileData.outputPathSrc, fileData.outputMainFileName, codeMap["mainCode"]);

    return true;
}