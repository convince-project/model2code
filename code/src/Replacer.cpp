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

/**
 * @brief Get the Event Data from the model and interface files 
 * 
 * @param fileData file data structure passed by reference from which the event data is extracted
 * @param eventData event data structure passed by reference where the event data is stored
 */
void getEventData(fileDataStr fileData, eventDataStr& eventData)
{
    if(eventsMap.find(eventData.event) != eventsMap.end()){
        std::cout << "Event already processed: " << eventData.event << std::endl;
        return;
    } 
    eventsMap[eventData.event];

    if(eventData.event != cmdTick && eventData.event != cmdHalt && eventData.event != rspTick && eventData.event != rspHalt)
    {
        getDataFromEvent(eventData);
        
        eventData.nodeName = "node" + eventData.functionName;
        eventData.clientName = "client"+ eventData.functionName;
        turnToSnakeCase(eventData.functionName,eventData.functionNameSnakeCase);
        eventData.serverName = "\"/"+ eventData.componentName +"/" + eventData.functionName + "\"";

        if(extractInterfaceName(fileData.modelFileName, eventData))
        {
            extractInterfaceType(fileData.interfaceFileName, eventData);
            printEventData(eventData);
        }        
    }
    eventsMap[eventData.event] = eventData;
}

/**
 * @brief Get the Event Data from the vector of event elements
 * 
 * @param fileData file data structure passed by reference where the file data is stored
 * @param elementsTransition vector of transition event elements
 * @param elementsSend vector of send event elements
 */
void getEventsVecData(fileDataStr fileData, const std::vector<tinyxml2::XMLElement*> elementsTransition, const std::vector<tinyxml2::XMLElement*> elementsSend)
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
            getEventData(fileData, eventData);
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
                std::string paramName, paramExpr;
                getElementAttValue(element, "name", paramName);
                getElementAttValue(element, "expr", paramExpr);
                eventData.paramMap[paramName] = paramExpr;
                std::cout << "\tparamName=" << paramName << ", paramExpr=" << eventData.paramMap[paramName] << std::endl;
            }
            getEventData(fileData, eventData);
        } 
        else{
            std::cerr << "\tMissing attribute in <send> tag" << std::endl;
        }
    }

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
 * @brief function to write the generic event code in the output string
 * 
 * @param eventData event data structure containing event information
 * @param savedCode saved code structure containing the saved code to be written
 * @param str output string to be modified
 */
void handleGenericEvent(const eventDataStr eventData, const savedCodeStr savedCode, std::string& str)
{
    if(eventData.eventType == "send"){
        if(eventData.interfaceType == "async-service" || eventData.interfaceType == "sync-service")
        {
            std::string eventCodeC = savedCode.eventC;
            std::string interfaceCodeH = savedCode.interfaceH;
            std::string interfaceCodeCMake = savedCode.interfaceCMake;
            std::string packageCodeCMake = savedCode.packageCMake;
            std::string interfaceCodeXML = savedCode.interfaceXML;
            //CPP
            replaceAll(eventCodeC, "$eventData.event$", eventData.event);
            replaceAll(eventCodeC, "$eventData.componentName$", eventData.componentName);
            replaceAll(eventCodeC, "$eventData.functionName$", eventData.functionName);
            replaceAll(eventCodeC, "$eventData.nodeName$", eventData.nodeName);
            replaceAll(eventCodeC, "$eventData.serverName$", eventData.serverName);
            replaceAll(eventCodeC, "$eventData.clientName$", eventData.clientName);
            replaceAll(eventCodeC, "$eventData.interfaceName$", eventData.interfaceName);
            for (auto itParam =  eventData.paramMap.begin(); itParam != eventData.paramMap.end(); ++itParam) 
            {
                std::string paramCode = savedCode.sendParam;
                replaceAll(paramCode, "$IT->FIRST$", itParam->first);
                writeAfterCommand(eventCodeC, "/*PARAM_LIST*/", paramCode);
            }
            for (auto itParam =  eventData.interfaceData.begin(); itParam != eventData.interfaceData.end(); ++itParam) 
            {
                std::string paramCode = savedCode.returnParam;
                replaceAll(paramCode, "$eventData.interfaceDataField$", itParam->first);
                if(itParam->first == "status" ){
                    keepSection(paramCode, "/*STATUS*/", "/*END_STATUS*/");
                }else{
                    deleteSection(paramCode, "/*STATUS*/", "/*END_STATUS*/");
                }
                writeAfterCommand(eventCodeC, "/*RETURN_PARAM_LIST*/", paramCode);
            }
            writeAfterCommand(str, "/*SEND_EVENT_LIST*/", eventCodeC);
            
            //H
            replaceAll(interfaceCodeH, "$eventData.interfaceName$", eventData.interfaceName);
            replaceAll(interfaceCodeH, "$eventData.functionNameSnakeCase$", eventData.functionNameSnakeCase);
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
    saveSection(code, "/*SEND_EVENT_SRV*/", "/*END_SEND_EVENT_SRV*/", savedCode.eventC);
    deleteSection(code, "/*SEND_EVENT_SRV*/", "/*END_SEND_EVENT_SRV*/");
    //H
    saveSection(code, "/*INTERFACE*/", "/*END_INTERFACE*/", savedCode.interfaceH);
    deleteSection(code, "/*INTERFACE*/", "/*END_INTERFACE*/");
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
void replaceEventCode(std::map <std::string, std::string>& codeMap){
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
                handleGenericEvent(eventData, savedCode, it->second);
            } 
        }
        //CPP
        deleteCommand(it->second, "/*SEND_EVENT_LIST*/");
        deleteCommand(it->second, "/*PARAM_LIST*/");
        deleteCommand(it->second, "/*RETURN_PARAM_LIST*/");
        //H
        deleteCommand(it->second, "/*INTERFACES_LIST*/");
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
 * @brief main function to get the code from template files and replace the placeholders with the data from the input file
 * 
 * @param fileData file data structure passed by reference where the file data is stored
 * @param templateFileData template file data structure passed by reference where the template file data is stored
 * @return true 
 * @return false 
 */
bool Replacer(fileDataStr& fileData, templateFileDataStr& templateFileData)
{ 
    std::string rootName;
    skillDataStr skillData;
    std::map <std::string, std::string> codeMap;
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
    setFileData(fileData, skillData);
    if(!readTemplates(templateFileData, codeMap))
    {
        return false;
    }

    for (auto it = codeMap.begin(); it != codeMap.end(); it++) {
        // std::cout << it->first << ": " << it->second << std::endl;
        replaceAll(it->second, "$className$", skillData.className);
        replaceAll(it->second, "$projectName$", skillData.classNameSnakeCase);
        replaceAll(it->second, "$SMName$", skillData.SMName);
        replaceAll(it->second, "$skillName$", skillData.skillName);
        replaceAll(it->second, "$skillTypeLC$", skillData.skillTypeLC);
        replaceAll(it->second, "$skillType$", skillData.skillType);

        if(skillData.skillType == "Action"){
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
    getEventsVecData(fileData, elementsTransition, elementsSend);
    replaceEventCode(codeMap);
    std::cout << "-----------" << std::endl;
    if(fileData.datamodel_mode)
    {
        writeFile(fileData.outputPathInclude + fileData.outputDatamodelFileNameH, codeMap["hDataModelCode"]);
        writeFile(fileData.outputPathSrc + fileData.outputDatamodelFileNameCPP, codeMap["cppDataModelCode"]);
    }
    writeFile(fileData.outputPathInclude + fileData.outputFileNameH, codeMap["hCode"]);
    writeFile(fileData.outputPathSrc + fileData.outputFileNameCPP, codeMap["cppCode"]);
    writeFile(fileData.outputPath + fileData.outputCMakeListsFileName, codeMap["cmakeCode"]);
    writeFile(fileData.outputPath + fileData.outputPackageXMLFileName, codeMap["packageCode"]);
    writeFile(fileData.outputPathSrc + fileData.outputMainFileName, codeMap["mainCode"]);

    return true;
}