/**
 * @file Generator.cpp
 * @brief this file contains the functions to generate code.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include "Generator.h"

/**
 * @brief Get component and function data from event name
 * 
 * @param eventData 
 */
void getDataFromEvent(eventDataStr& eventData) 
{
    std::string firstWord, secondWord, thirdWord;
    std::string event = eventData.event;
    if (event == ""){
        std::cerr << "Event has no value" << std::endl;
        return;
    }
    size_t firstDotPos = event.find('.');
    if (firstDotPos != std::string::npos) {
        firstWord = event.substr(0, firstDotPos);
        size_t secondDotPos = event.find('.', firstDotPos + 1);
        if (secondDotPos != std::string::npos) {
            secondWord = event.substr(firstDotPos+1, secondDotPos - firstDotPos - 1);
            thirdWord = event.substr(secondDotPos + 1);
        }
        else{
            std::cerr << "Error in name format for event: "<< event << std::endl;
        }
    }
    else{
        std::cerr << "Error in name format for event: "<< event << std::endl;
    }
    eventData.componentName = firstWord;
    eventData.functionName   = secondWord;
    eventData.eventName     = thirdWord;
}

/**
 * @brief Get the skill Data From SCXML Root Name object
 * 
 * @param attributeName 
 * @param skillData 
 * @return bool 
 */
bool getDataFromRootName(const std::string attributeName, skillDataStr& skillData)
{
    if (attributeName != ""){
        std::cout << "Root attribute name: " << attributeName << std::endl;
        size_t dotPos = attributeName.find("Skill");
        if (dotPos != std::string::npos){
            skillData.SMName = attributeName;
            skillData.skillName = attributeName.substr(0, dotPos);
            skillData.className = attributeName.substr(0, dotPos + 5);
            skillData.skillType = attributeName.substr(dotPos + 5);
            if(skillData.skillType == ""){
                std::cerr << "Skill type not found" << std::endl;
                return false;
            }
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
 * \defgroup GenerateCode Generate code
 * Functions to generate code.
 * @{
 */

/**
 * \defgroup printData Print Data
 * \ingroup GenerateCode
 * Print Data
 * @{
 */
/**
 * @brief Prints the event data
 * 
 * @param eventData 
 */
void printEventData(eventDataStr eventData)
{
    std::cout << "\tcomponent=" << eventData.componentName << ", service=" << eventData.functionName << ", eventName=" << eventData.eventName << std::endl;
    std::cout<< "\tinterface=" << eventData.interfaceName << ", type=" << eventData.interfaceType;
    if(eventData.interfaceDataType != "")
    {
        std::cout << ", dataType=" << eventData.interfaceDataType;
    }
    if(eventData.interfaceDataField != "")
    {
        std::cout << ", dataField=" << eventData.interfaceDataField;
    }
    std::cout << std::endl;
    
}

/**
 * @brief Prints the skill data
 * 
 * @param skillData 
 */
void printSkillData(skillDataStr skillData)
{
    std::cout << "-----------" << std::endl;
    std::cout << "Class name: " << skillData.className << std::endl << "Skill name: " << skillData.skillName << std::endl << "Skill type: " << skillData.skillType << std::endl;
    std::cout << "-----------" << std::endl;
}
/** @} */ // end of printData subgroup

/**
 * \defgroup processEvent Process Event
 * \ingroup GenerateCode
 * Process Event
 * @{
 */

/**
 * @brief Process the event data
 * 
 * @param eventData 
 * @param skillData 
 * @param target 
 * @param hCode 
 * @param cppCode 
 * @param topicCode 
 */
void processEvent(fileDataStr fileData, eventDataStr eventData, const skillDataStr skillData, std::string target, hCodeStr& hCode, cppCodeStr& cppCode, topicCodeStr& topicCode)
{
    const std::string className = skillData.className;
    const std::string skillName = skillData.skillName;
    const std::string skillType = skillData.skillType;

    if(eventsMap.find(eventData.event) != eventsMap.end()){
        std::cout << "Event already processed: " << eventData.event << std::endl;
        return;
    } 
    eventsMap[eventData.event];

    if(eventData.event == cmdTick)
    {
        hCode.publicCode += "\tvoid tick( [[maybe_unused]] const std::shared_ptr<bt_interfaces::srv::Tick" + skillType + "::Request> request,\n"
            "\t\t\t   std::shared_ptr<bt_interfaces::srv::Tick" + skillType + "::Response>      response);\n";
    }
    else if(eventData.event == cmdHalt)
    {
        hCode.publicCode += "\tvoid halt( [[maybe_unused]] const std::shared_ptr<bt_interfaces::srv::Halt" + skillType + "::Request> request,\n"
            "\t\t\t   [[maybe_unused]] std::shared_ptr<bt_interfaces::srv::Halt" + skillType + "::Response> response);\n";
    }
    else if(eventData.event == rspHalt)
    {
        cppCode.handlersCode += 
            "\tm_stateMachine.connectToEvent(\"" + eventData.event + "\", [this]([[maybe_unused]]const QScxmlEvent & event){\n"
            "\t\tRCLCPP_INFO(m_node->get_logger(), \""+ className +"::haltresponse\");\n"
            "\t\tm_haltResult.store(true);\n"
            "\t});\n\n";
    }
    else if(eventData.event == rspTick)
    {
        cppCode.handlersCode += 
            "\tm_stateMachine.connectToEvent(\"" + eventData.event + "\", [this]([[maybe_unused]]const QScxmlEvent & event){\n"
            "\t\tRCLCPP_INFO(m_node->get_logger(), \"" + className + "::tickReturn %s\", event.data().toMap()[\"result\"].toString().toStdString().c_str());\n"
            "\t\tstd::string result = event.data().toMap()[\"result\"].toString().toStdString();\n"
            "\t\tif (result == \"SUCCESS\" )\n"
            "\t\t{\n" 
            "\t\t\tm_tickResult.store(Status::success);\n"
            "\t\t}\n";

        if(skillType == "Action")
        {
            cppCode.handlersCode +=
                "\t\telse if (result == \"RUNNING\" )\n"
                "\t\t{\n"                  
                "\t\t\tm_tickResult.store(Status::running);\n"
                "\t\t}\n";
        }

        cppCode.handlersCode +=
            "\t\telse if (result == \"FAILURE\" )\n"
            "\t\t{ \n"
            "\t\t\tm_tickResult.store(Status::failure);\n"
            "\t\t}\n"
            "\t});\n\n";

    }
    else
    {
        getDataFromEvent(eventData);
        
        std::string nodeName = "node" + eventData.functionName;
        std::string clientName = "client"+ eventData.functionName;

        if(extractInterfaceName(fileData.modelFileName, eventData))
        {
            extractInterfaceType(fileData.interfaceFileName, eventData);
            printEventData(eventData);
        }
        
        if(eventData.eventType == "send")
        {
            cppCode.handlersCode +=
            "    m_stateMachine.connectToEvent(\"" + eventData.event + "\", [this]([[maybe_unused]]const QScxmlEvent & event){\n"
            "        std::shared_ptr<rclcpp::Node> " + nodeName + " = rclcpp::Node::make_shared(m_name + \"SkillNode" + eventData.functionName + "\");\n";

            if(eventData.interfaceType == "async-service" || eventData.interfaceType == "sync-service" )
            {
                std::string functionNameSnakeCase, serverName;
                turnToSnakeCase(eventData.functionName,functionNameSnakeCase);
                serverName = "\"/"+ eventData.componentName +"/" + eventData.functionName + "\"";
                hCode.includeCode += "#include <" + eventData.interfaceName + "/srv/" + functionNameSnakeCase + ".hpp>\n";
                cppCode.handlersCode +=
                "        std::shared_ptr<rclcpp::Client<"+ eventData.interfaceName +"::srv::" + eventData.functionName + ">> "+ clientName +" = "+ nodeName +"->create_client<"+ eventData.interfaceName +"::srv::" + eventData.functionName + ">(" + serverName +");\n"
                "        auto request = std::make_shared<"+ eventData.interfaceName +"::srv::" + eventData.functionName + "::Request>();\n";
                bool hasParam = false;
                for (auto it =  eventData.paramMap.begin(); it != eventData.paramMap.end(); ++it) {
                    if(!hasParam){
                        cppCode.handlersCode += "        auto eventParams = event.data().toMap();\n";
                        hasParam = true;
                    }    
                    cppCode.handlersCode +=
                        "        request->" + it->first + " = convert<decltype(request->" + it->first + ")>(eventParams[\"" + it->first + "\"].toString().toStdString());\n";
                }
                
                cppCode.handlersCode +=
                "        bool wait_succeded{true};\n"
                "        int retries = 0;\n"
                "        while (!"+ clientName +"->wait_for_service(std::chrono::seconds(1))) {\n"
                "            if (!rclcpp::ok()) {\n"
                "                RCLCPP_ERROR(rclcpp::get_logger(\"rclcpp\"), \"Interrupted while waiting for the service '" + eventData.functionName + "'. Exiting.\");\n"
                "                wait_succeded = false;\n"
                "                break;\n"
                "            } \n"
                "            retries++;\n"
                "            if(retries == SERVICE_TIMEOUT) {\n"
                "               RCLCPP_ERROR(rclcpp::get_logger(\"rclcpp\"), \"Timed out while waiting for the service '"+ eventData.functionName +"'.\");\n"
                "               wait_succeded = false;\n"
                "               break;\n"
                "            }\n"
                "        }\n"
                
                "        if (wait_succeded) {\n"
                "            // send the request                                                                    \n"
                "            auto result = "+ clientName +"->async_send_request(request);\n"
                "            const std::chrono::seconds timeout_duration(SERVICE_TIMEOUT);\n"
                "            auto futureResult = rclcpp::spin_until_future_complete("+ nodeName +", result, timeout_duration);\n"
                "            if (futureResult == rclcpp::FutureReturnCode::SUCCESS) \n"
                "            {\n"
                "               auto response = result.get();\n"
                "               if( response->is_ok ==true) {\n"
                "                   QVariantMap data;\n"
                "                   data.insert(\"result\", \"SUCCESS\");\n";
                if(eventData.interfaceDataField != "")
                {
                    cppCode.handlersCode +=
                    "                    data.insert(\""+ eventData.interfaceDataField +"\", response->" + eventData.interfaceDataField;
                    if(eventData.interfaceDataField == "status")
                    {
                        cppCode.handlersCode += ".status";
                    }
                    cppCode.handlersCode +=
                    ");\n";
                }
                cppCode.handlersCode +=
                "                   m_stateMachine.submitEvent(\"" + eventData.componentName + "."+ eventData.functionName +".Return\", data);\n"
                "                   RCLCPP_INFO(rclcpp::get_logger(\"rclcpp\"), \"" + eventData.componentName + "."+ eventData.functionName +".Return\");\n"
                "                   return;\n"
                "               }\n"
                "           }\n"
                "           else if(futureResult == rclcpp::FutureReturnCode::TIMEOUT){\n"
                "               RCLCPP_ERROR(rclcpp::get_logger(\"rclcpp\"), \"Timed out while future complete for the service '"+ eventData.functionName +"'.\");\n"
                "           }\n"
                "        }\n"
                "       QVariantMap data;\n"
                "       data.insert(\"result\", \"FAILURE\");\n"
                "       m_stateMachine.submitEvent(\"" + eventData.componentName + "." + eventData.functionName +".Return\", data);\n"
                "       RCLCPP_INFO(rclcpp::get_logger(\"rclcpp\"), \"" + eventData.componentName + "."+ eventData.functionName +".Return\");\n";
            }

            cppCode.handlersCode +="    });\n\n";
        }
        else if(eventData.eventType == "transition")
        {
            if(eventData.interfaceType =="topic")
            {
                topicCode.publicCode += "\tvoid topic_callback(const " + eventData.interfaceDataType +"::SharedPtr msg);\n";
                topicCode.privateCode += "\trclcpp::Subscription<" + eventData.interfaceDataType +">::SharedPtr m_subscription;\n";
                topicCode.servicesCode += "\tm_subscription = m_node->create_subscription<" + eventData.interfaceDataType +">(\n"
                        "\t\t\"/battery_status\", 10, std::bind(&"+ skillData.className +"::topic_callback, this, std::placeholders::_1));\n\n";
                
                if(eventData.interfaceDataType != "")
                {
                    std::string dataPath;
                    getDataTypePath(eventData.interfaceDataType, dataPath);
                    hCode.includeCode += "#include <" + dataPath +".hpp>\n";
                } 

                topicCode.callbacksCode +=
                    "void "+ skillData.className +"::topic_callback(const " + eventData.interfaceDataType +"::SharedPtr msg) {\n"
                "    QVariantMap data;\n"
                "    data.insert(\""+ eventData.interfaceDataField +"\", msg->" + eventData.interfaceDataField + ");\n"
                "    m_stateMachine.submitEvent(\""+ eventData.event +"\", data);\n"
                "}\n\n";
            }
            // else if(eventData.interfaceType == "async-service" || eventData.interfaceType == "sync-service")
            // {
            // }
        }
        
    }

}

/**
 * @brief Generate the code for the events
 * 
 * @param elementsTransition 
 * @param elementsSend 
 * @param skillData 
 * @param hCode 
 * @param cppCode 
 * @param topicCode 
 */
void getEventsCode(fileDataStr fileData, const std::vector<tinyxml2::XMLElement*> elementsTransition, const std::vector<tinyxml2::XMLElement*> elementsSend, skillDataStr skillData, hCodeStr& hCode, cppCodeStr& cppCode, topicCodeStr& topicCode)
{
    for (const auto& element : elementsTransition) {
        const char* event = element->Attribute("event");
        const char* target = element->Attribute("target");
    
        if (event && target) 
        {

            std::cout << "\nTransition: event=" << event << ", target=" << target << std::endl;
            eventDataStr eventData;
            eventData.target = target;
            eventData.event = event;
            eventData.eventType = "transition";

            processEvent(fileData, eventData, skillData, target, hCode, cppCode, topicCode);
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
            std::cout << "\nSend: event=" << event << std::endl;
            eventDataStr eventData;
            eventData.event = event;
            eventData.eventType = "send";
            std::vector<tinyxml2::XMLElement*> elementsParam;
            findElementVectorByTag(element, "param", elementsParam);
            for (const auto& element : elementsParam) {
                std::string paramName, paramEvent;
                getElementAttValue(element, "name", paramName);
                getElementAttValue(element, "expr", paramEvent);
                eventData.paramMap[paramName] = paramEvent;
                std::cout << "\tparamName=" << paramName << ", paramExpr=" << eventData.paramMap[paramName] << std::endl;
        
            }

            processEvent(fileData, eventData, skillData, "", hCode, cppCode, topicCode);
        } 
        else
        {
            std::cerr << "\tMissing attribute in <send> tag" << std::endl;
        }
    }

}
/** @} */ // end of processEvent subgroup

/**
 * \defgroup writeCode Write Code
 * \ingroup GenerateCode
 * Write Code
 * @{
 */
/**
 * @brief Write the header code
 * 
 * @param skillData 
 * @param code 
 * @param datamodel_mode 
 */
void writeHCode(const skillDataStr skillData, hCodeStr& code, bool datamodel_mode){
    const std::string className = skillData.className;
    const std::string SMName = skillData.SMName;
    const std::string skillType = skillData.skillType;
    const std::string skillName = skillData.skillName;
    std::string skillTypeLC = skillType;
    for (char &c : skillTypeLC) 
    { 
        c = std::tolower(c); 
    } 
    
    code.includeCode = 
        "/******************************************************************************\n"
        " *                                                                            *\n"
        " * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *\n"
        " * All Rights Reserved.                                                       *\n"
        " *                                                                            *\n"
        " ******************************************************************************/\n\n"
        "# pragma once\n\n"
        "#include <mutex>\n"
        "#include <thread>\n"
        "#include <rclcpp/rclcpp.hpp>\n"
        "#include \"" + className + "SM.h\"\n"
        "#include <bt_interfaces/msg/" + skillTypeLC +"_response.hpp>\n";   
    
    code.statusCode = 
        "#define SERVICE_TIMEOUT 8\n\n"
        "enum class Status{\n"
        "\tundefined,\n";
        if(skillType == "Action")
        {
            code.statusCode += "\trunning,\n";
        }
    code.statusCode += 
        "\tsuccess,\n"
        "\tfailure\n"
        "};\n\n";

    code.publicCode += 
        "class " + className + "\n"
        "{\n"
        "public:\n"
        "\t" + className +"(std::string name );\n"
        "\tbool start(int argc, char * argv[]);\n"
        "\tstatic void spin(std::shared_ptr<rclcpp::Node> node);\n";
    code.privateCode = 
        "\nprivate:\n"
        "\tstd::shared_ptr<std::thread> m_threadSpin;\n"
        "\tstd::shared_ptr<rclcpp::Node> m_node;\n"
        "\tstd::mutex m_requestMutex;\n"
        "\tstd::string m_name;\n"
        "\t" + SMName +" m_stateMachine;\n"
        "\t\n";
    code.closeClassCode = 
        "\t\n"
        "};\n\n";

    if(datamodel_mode)
    {
        code.includeCode += "#include \""  + skillName + "SkillDataModel.h\"\n";
        code.privateCode += skillName + "SkillDataModel m_dataModel;\n";
    }
    
}
/**
 * @brief Write the cpp code
 * 
 * @param skillData 
 * @param code 
 * @param datamodel_mode 
 */
void writeCppCode(const skillDataStr skillData, cppCodeStr& code, bool datamodel_mode){
    const std::string className = skillData.className;
    const std::string skillType = skillData.skillType;
    code.includeCode = 
        "/******************************************************************************\n"
        " *                                                                            *\n"
        " * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *\n"
        " * All Rights Reserved.                                                       *\n"
        " *                                                                            *\n"
        " ******************************************************************************/\n"
        "\n"
        "#include \"" + className + ".h\"\n"
        "#include <future>\n"
        "#include <QTimer>\n"
        "#include <QDebug>\n"
        "#include <QTime>\n"
        "#include <iostream>\n"
        "#include <QStateMachine>\n\n";

        code.converterCode = "#include <type_traits>\n\n"

            "template<typename T>\n"
            "T convert(const std::string& str) {\n"
            "    if constexpr (std::is_same_v<T, int>) {\n"
            "        return std::stoi(str);\n"
            "    } else if constexpr (std::is_same_v<T, double>) {\n"
            "        return std::stod(str);\n"
            "    } else if constexpr (std::is_same_v<T, float>) {\n"
            "        return std::stof(str);\n"
            "    } \n"
            "    else if constexpr (std::is_same_v<T, bool>) { \n"
            "        if (str == \"true\" || str == \"1\") { \n"
            "            return true; \n"
            "        } else if (str == \"false\" || str == \"0\") { \n"
            "            return false; \n"
            "        } else { \n"
            "            throw std::invalid_argument(\"Invalid boolean value\"); \n"
            "        } \n"
            "    } \n"
            "    else if constexpr (std::is_same_v<T, std::string>) {\n"
            "        return str;\n"
            "    }\n"
            "    else {\n"
            "        // Handle unsupported types\n"
            "        throw std::invalid_argument(\"Unsupported type conversion\");\n"
            "    }\n"
            "}\n\n";

    code.constructorCode = 
        className + "::"+ className +"(std::string name ) :\n"
        "\t\tm_name(std::move(name))\n"
        "{\n";
     if(datamodel_mode)
     {
        code.constructorCode +=
        "\tm_stateMachine.setDataModel(&m_dataModel);\n";
     }
    code.constructorCode +=
        "}\n\n";
    code.spinCode =
        "void "+ className +"::spin(std::shared_ptr<rclcpp::Node> node)\n"
        "{\n"
        "\trclcpp::spin(node);\n"
        "\trclcpp::shutdown();\n" 
        "}\n\n";
    code.startCode =    
        "bool "+ className +"::start(int argc, char*argv[])\n"
        "{\n"
        "\tif(!rclcpp::ok())\n"
        "\t{\n"
        "\t\trclcpp::init(/*argc*/ argc, /*argv*/ argv);\n"
        "\t}\n\n"
        "\tm_node = rclcpp::Node::make_shared(m_name + \"Skill\");\n"
        "\tRCLCPP_DEBUG_STREAM(m_node->get_logger(), \""+ className +"::start\");\n"
        "\tstd::cout << \""+ className +"::start\";\n\n";

    code.servicesCode= "";

    code.endstartCode =         
        "\tm_stateMachine.start();\n"
        "\tm_threadSpin = std::make_shared<std::thread>(spin, m_node);\n\n"
        "\treturn true;\n"
        "}\n\n";

}

/**
 * @brief Write the header code for the data model
 * 
 * @param skillData 
 * @param code 
 */
void writeDataModelHCode(const skillDataStr skillData, hCodeStr& code){
    const std::string className = skillData.className;
    
    code.includeCode = 
        "/******************************************************************************\n"
        " *                                                                            *\n"
        " * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *\n"
        " * All Rights Reserved.                                                       *\n"
        " *                                                                            *\n"
        " ******************************************************************************/\n\n"
        "# pragma once\n\n"
        "#include <QScxmlCppDataModel>\n"
        "#include <QVariant>\n"
        "#include <string>\n"
        "#include <thread>\n"
        "#include <rclcpp/rclcpp.hpp>\n";   

    code.publicCode += 
        "class " + className + "DataModel: public QScxmlCppDataModel\n"
        "{\n"
        "    Q_SCXML_DATAMODEL\n\n"
        "public:\n"
        "   " + className +"DataModel() = default;\n"
        "   bool setup(const QVariantMap& initialDataValues) override;\n"
        "   void log(std::string to_log);\n"
        "   //void topic_callback(const " "::SharedPtr msg);\n"
        "   static void spin(std::shared_ptr<rclcpp::Node> node);\n";
    code.privateCode = 
        "\nprivate:\n"
        "   uint m_status;\n"
        "   //rclcpp::Subscription<" ">::SharedPtr m_subscription;\n"
        "   std::shared_ptr<std::thread> m_threadSpin;\n"
        "   std::shared_ptr<rclcpp::Node> m_node;\n";
    code.closeClassCode = 
        "\t\n"
        "};\n\n"
        "Q_DECLARE_METATYPE(::" + className +"SkillDataModel*)";
    
}
/**
 * @brief Write the cpp code for the data model
 * 
 * @param skillData 
 * @param code 
 */
void writeDataModelCppCode(const skillDataStr skillData, cppDataModelCodeStr& code){
    const std::string className = skillData.className;
    code.includeCode = 
        "/******************************************************************************\n"
        " *                                                                            *\n"
        " * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *\n"
        " * All Rights Reserved.                                                       *\n"
        " *                                                                            *\n"
        " ******************************************************************************/\n"
        "\n"
        "#include \"" + className + "DataModel.h\"\n"
        "#include <QDebug>\n\n";
    code.spinCode =
        "void "+ className +"DataModel::spin(std::shared_ptr<rclcpp::Node> node)\n"
        "{\n"
        "\trclcpp::spin(node);\n"
        "\trclcpp::shutdown();\n" 
        "}\n\n";
    code.logCode =
        "void "+ className +"DataModel::log(std::string to_log)\n"
        "{\n"
        "\tqInfo(to_log.c_str());\n"
        "}\n\n";
    code.setupCode =    
        "bool "+ className +"DataModel::setup(const QVariantMap& initialDataValues)\n"
        "{\n"
        "\tif(!rclcpp::ok())\n"
        "\t{\n"
        "\t\trclcpp::init(/*argc*/ 0, /*argv*/ nullptr);\n"
        "\t}\n\n"
        "\tm_node = rclcpp::Node::make_shared(\""+ className +"DataModelNode\");\n"
        "\tm_threadSpin = std::make_shared<std::thread>(spin, m_node);\n";

        "\tstd::cout << \""+ className +"::start\";\n\n";

    code.subscriptionCode= "";

    code.endsetupCode =         
        "\treturn true;\n"
        "}\n\n";

    code.setupCode += "\t//m_subscription = m_node->create_subscription<" ">(\n"
    "\t//\t\"/\", 10, std::bind(&""::topic_callback, this, std::placeholders::_1));\n\n";

    code.includeCode += "//#include <"".hpp>\n\n";

    code.callbacksCode +=
        "//void "+ skillData.className +"::topic_callback(const " "::SharedPtr msg) {\n"
    "//}\n\n";

}
/** @} */ // end of writeCode group
/** @} */ // end of GenerateCode group

/**
 * \defgroup GenerateOutputFiles Generate output files
 * Functions to generate output files.
 * @{
 */

/**
 * @brief Generate the data model header file
 * 
 * @param outputPath 
 * @param outputFileName 
 * @param code 
 */
void generateDataModelHFile(const std::string outputPath, const std::string outputFileName, hCodeStr code)
{

    
    static std::ofstream outputFile(outputPath + outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << outputPath + outputFileName << std::endl;
        return;
    }
    outputFile << code.includeCode;
    outputFile << "\n";
    outputFile << code.openClassCode;
    outputFile << code.publicCode;
    outputFile << code.privateCode;
    outputFile << code.closeClassCode;
    outputFile.close();
    std::cout << outputFileName + " file generated" << std::endl;
}
/**
 * @brief Generate the data model cpp file
 * 
 * @param outputPath 
 * @param outputFileName 
 * @param code 
 */
void generateDataModelCppFile(const std::string outputPath, const std::string outputFileName, cppDataModelCodeStr code) 
{
    static std::ofstream outputFile(outputPath + outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << outputPath + outputFileName << std::endl;
        return;
    }
    
    outputFile << code.includeCode; 
    outputFile << code.spinCode;
    outputFile << code.logCode;
    outputFile << code.setupCode; 
    outputFile << code.subscriptionCode;
    outputFile << code.endsetupCode;
    outputFile << code.callbacksCode; 
    
    outputFile.close();
    std::cout << outputFileName + " file generated" << std::endl;
}

/**
 * @brief Generate the header file
 * 
 * @param outputPath 
 * @param outputFileName 
 * @param skillData 
 * @param code 
 */
void generateHFile(const std::string outputPath, const std::string outputFileName, const skillDataStr skillData, hCodeStr code) 
{
    const std::string className = skillData.className;
    const std::string skillType = skillData.skillType;
    std::string skillTypeLC = skillType;
    for (char &c : skillTypeLC) 
    { 
        c = std::tolower(c); 
    } 
    
    static std::ofstream outputFile(outputPath + outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << outputPath + outputFileName << std::endl;
        return;
    }
    if(eventsMap.find("CMD_TICK") != eventsMap.end())
    {
        code.includeCode += "#include <bt_interfaces/srv/tick_" + skillTypeLC +".hpp>\n";
        code.privateCode += 
            "\tstd::atomic<Status> m_tickResult{Status::undefined};\n"
            "\trclcpp::Service<bt_interfaces::srv::Tick" + skillType + ">::SharedPtr m_tickService;\n";
    }
    if(eventsMap.find("CMD_HALT") != eventsMap.end())
    {
        code.includeCode += "#include <bt_interfaces/srv/halt_" + skillTypeLC +".hpp>\n";
        code.privateCode += 
            "\tstd::atomic<bool> m_haltResult{false};\n"
            "\trclcpp::Service<bt_interfaces::srv::Halt" + skillType + ">::SharedPtr m_haltService;\n";
    }

    outputFile << code.includeCode;
    outputFile << "\n"; 
    outputFile << code.statusCode;
    outputFile << code.openClassCode;
    outputFile << code.publicCode;
    outputFile << code.privateCode;
    outputFile << code.closeClassCode;
    outputFile.close();
    std::cout << outputFileName + " file generated" << std::endl;

}
/**
 * @brief Generate the cpp file
 * 
 * @param outputPath 
 * @param outputFileName 
 * @param skillData 
 * @param code 
 */
void generateCppFile(const std::string outputPath, const std::string outputFileName, const skillDataStr skillData, cppCodeStr code) 
{
    const std::string className = skillData.className;
    const std::string skillType = skillData.skillType;
    
    if(eventsMap.find("CMD_TICK") != eventsMap.end())
    {
        code.servicesCode +=
        "\tm_tickService = m_node->create_service<bt_interfaces::srv::Tick" + skillType + ">(m_name + \"Skill/tick\",\n"  
        "                                                                           \tstd::bind(&"+ className +"::tick,\n"
        "                                                                           \tthis,\n"
        "                                                                           \tstd::placeholders::_1,\n"
        "                                                                           \tstd::placeholders::_2));\n\n";
        code.callbacksCode +=
            "void " + className + "::tick( [[maybe_unused]] const std::shared_ptr<bt_interfaces::srv::Tick" + skillType +"::Request> request,\n"
            "                                std::shared_ptr<bt_interfaces::srv::Tick" + skillType +"::Response>      response)\n"
            "{\n"
            "    std::lock_guard<std::mutex> lock(m_requestMutex);\n"
            "    RCLCPP_INFO(m_node->get_logger(), \"" + className + "::tick\");\n"
            "    auto message = bt_interfaces::msg::" + skillType +"Response();\n"
            "    m_tickResult.store(Status::undefined); //here we can put a struct\n"
            "    m_stateMachine.submitEvent(\""+ cmdTick +"\");\n"
            "   \n"
            "    while(m_tickResult.load()== Status::undefined) \n"
            "    {\n"
            "        std::this_thread::sleep_for (std::chrono::milliseconds(100));\n"
            "        // qInfo() <<  \"active names\" << m_stateMachine.activeStateNames();\n"
            "    }\n"
            "    switch(m_tickResult.load()) \n"
            "    {\n";
            if(skillType == "Action")
            {
                code.callbacksCode +=
                "        case Status::running:\n"
                "            response->status.status = message.SKILL_RUNNING;\n"
                "            break;\n";
            }

            code.callbacksCode +=
            "        case Status::failure:\n"
            "            response->status.status = message.SKILL_FAILURE;\n"
            "            break;\n"
            "        case Status::success:\n"
            "            response->status.status = message.SKILL_SUCCESS;\n"
            "            break;            \n"
            "    }\n"
            "    RCLCPP_INFO(m_node->get_logger(), \"" + className + "::tickDone\");\n"
            "   \n"
            "    response->is_ok = true;\n"
            "}\n\n";
    }
    if (eventsMap.find("CMD_HALT") != eventsMap.end())
    {
        code.servicesCode +=
        "\tm_haltService = m_node->create_service<bt_interfaces::srv::Halt" + skillType + ">(m_name + \"Skill/halt\",\n"  
        "                                                                            \tstd::bind(&"+ className +"::halt,\n"
        "                                                                            \tthis,\n"
        "                                                                            \tstd::placeholders::_1,\n"
        "                                                                            \tstd::placeholders::_2));\n\n";   

        code.callbacksCode += 
        "void " + className + "::halt( [[maybe_unused]] const std::shared_ptr<bt_interfaces::srv::Halt" + skillType +"::Request> request,\n"
        "    [[maybe_unused]] std::shared_ptr<bt_interfaces::srv::Halt" + skillType +"::Response> response)\n"
        "{\n"
        "    std::lock_guard<std::mutex> lock(m_requestMutex);\n"
        "    RCLCPP_INFO(m_node->get_logger(), \"" + className + "::halt\");\n"
        "    m_haltResult.store(false); //here we can put a struct\n"
        "    m_stateMachine.submitEvent(\""+ cmdHalt +"\");\n"
        "   \n"
        "    while(!m_haltResult.load()) \n"
        "    {\n"
        "        std::this_thread::sleep_for (std::chrono::milliseconds(100));\n"
        "        // qInfo() <<  \"active names\" << m_stateMachine.activeStateNames();\n"
        "    }\n"
        "    RCLCPP_INFO(m_node->get_logger(), \"" + className + "::haltDone\");\n"
        "   \n"
        "    response->is_ok = true;\n"
        "}\n";  
    }    

    static std::ofstream outputFile(outputPath + outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << outputPath + outputFileName << std::endl;
        return;
    }
    
    outputFile << code.includeCode; 
    outputFile << code.converterCode; 
    outputFile << code.constructorCode; 
    outputFile << code.spinCode; 
    outputFile << code.startCode;
    outputFile << code.servicesCode;
    outputFile << code.handlersCode;
    outputFile << code.endstartCode;
    outputFile << code.callbacksCode; 
    
    outputFile.close();
    std::cout << outputFileName + " file generated" << std::endl;
}
/** @} */ // end of GenerateOutputFiles group

/**
 * @brief Print the help message
 * 
 */
void print_help()
{
    std::cout << "Welcome to model2code tool.\n";
    std::cout << "Syntax:\n";
    std::cout << "model2code --input_filename \"inputFileName.scxml\" \t";
    std::cout << "--model_filename \"modelFileName.scxml\"t";
    std::cout << "--interface_filename \"interfaceFileName.scxml\"\t";
    std::cout << "--output_path \"path/to/output/directory\"\n";
}

/**
 * @brief Generator function
 * 
 * @param fileData 
 * @return true 
 * @return false 
 */
bool generator(fileDataStr fileData){

    std::string rootName;
    skillDataStr skillData;
    std::vector<tinyxml2::XMLElement *> elementsTransition, elementsSend;
    tinyxml2::XMLDocument doc;
    std::cout << "-----------" << std::endl;
    if(!extractFromSCXML(doc, fileData.inputFileName, rootName, elementsTransition, elementsSend)){
        return 0;
    }
    
    if(!getDataFromRootName(rootName, skillData)){
        return 0;
    }
    printSkillData(skillData);
 
    hCodeStr hCode;
    cppCodeStr cppCode;
    topicCodeStr topicCode;
    writeHCode(skillData, hCode, fileData.datamodel_mode);
    writeCppCode(skillData, cppCode, fileData.datamodel_mode);
    getEventsCode(fileData, elementsTransition, elementsSend, skillData, hCode, cppCode, topicCode);
    std::cout << "-----------" << std::endl;
    std::string outputFileNameH = skillData.className + ".h";
    std::string outputFileNameCPP = skillData.className + ".cpp";
    
    if(fileData.datamodel_mode)
    {
        hCodeStr hDataModelCode;
        cppDataModelCodeStr cppDataModelCode;
        std::string outputFileNameDataModelH = skillData.className + "DataModel.h";
        std::string outputFileNameDataModelCPP = skillData.className + "DataModel.cpp";   
        writeDataModelHCode(skillData, hDataModelCode);
        writeDataModelCppCode(skillData, cppDataModelCode);
        hDataModelCode.includeCode += topicCode.includeCode;
        hDataModelCode.publicCode += topicCode.publicCode;
        hDataModelCode.privateCode += topicCode.privateCode;
        cppDataModelCode.subscriptionCode += topicCode.servicesCode;
        cppDataModelCode.callbacksCode += topicCode.callbacksCode;
        generateDataModelHFile(fileData.outputPathHFile, outputFileNameDataModelH, hDataModelCode);
        generateDataModelCppFile(fileData.outputPathCppFile, outputFileNameDataModelCPP, cppDataModelCode);
    }
    else
    {
        hCode.includeCode += topicCode.includeCode;
        hCode.publicCode += topicCode.publicCode;
        hCode.privateCode += topicCode.privateCode;
        cppCode.servicesCode += topicCode.servicesCode;
        cppCode.callbacksCode += topicCode.callbacksCode;
    }

    generateHFile(fileData.outputPathHFile, outputFileNameH, skillData, hCode);  
    generateCppFile(fileData.outputPathCppFile, outputFileNameCPP, skillData, cppCode);

    return true;
}
