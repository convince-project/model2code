/*
 * SPDX-FileCopyrightText: 2024-2024 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * @file Data.h
 * @brief This file contains the structures used in the code generation.
 */
#include <string>
#include <map>

struct skillDataStr{
    std::string SMName;
    std::string className;
    std::string skillName;
    std::string skillType;
};

struct eventDataStr{
    std::string event;
    std::string eventType;
    std::string target;
    std::map<std::string, std::string> paramMap;
    std::string paramExpr;
    std::string componentName;
    std::string functionName;
    std::string eventName;
    std::string interfaceName;
    std::string interfaceType;
    std::string interfaceDataType;
    std::string interfaceDataField;
};

struct fileDataStr{
    std::string inputFileName;
    std::string modelFileName;
    std::string interfaceFileName;
    std::string outputPath;
    std::string outputPathHFile;
    std::string outputPathCppFile;
    bool debug_mode;
    bool datamodel_mode;   
};