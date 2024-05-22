/*
 * SPDX-FileCopyrightText: 2024-2024 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file main.cpp
 * @brief this file contains the main function.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

#include "Generator.h"

/**
 * @brief Main function
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[])
{
    fileDataStr fileData;
    // Default values
    fileData.modelFileName      = modelFilePath;
    fileData.interfaceFileName  = interfaceFilePath;
    fileData.debug_mode         = false;
    fileData.datamodel_mode     = false;

    #if 0 //debug only!
        std::cout << "Invocation command " << argc << ":";
        for (size_t i = 0; i < argc; ++i)
        {
                std::cout << argv[i] << " ";
        }
        std::cout << "<EOL>"<<std::endl;
    #endif

    if (argc == 1)
    {
        print_help();
        return RETURN_CODE_ERROR;
    }

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--help") {
            print_help();
            return RETURN_CODE_ERROR;
        }
        else if (arg == "--input_filename"  && i+1 < argc && argv[i+1][0] != '-') {
            fileData.inputFileName = argv[i+1];
            i++;
        }
        else if (arg == "--output_path" && i+1 < argc && argv[i+1][0] != '-') {
            fileData.outputPath = argv[i + 1];
            i++;
        }
        else if (arg == "--model_filename" && i+1 < argc && argv[i+1][0] != '-') {
            fileData.modelFileName = argv[i + 1];
            i++;
        }
        else if (arg == "--interface_filename" && i+1 < argc && argv[i+1][0] != '-') {
            fileData.interfaceFileName = argv[i + 1];
            i++;
        }
        else if (arg == "--datamodel_mode") {
            fileData.datamodel_mode = true;
        }
        else if (arg == "--debug_mode") {
            fileData.debug_mode = true;
        }
    }
    
    if(fileData.inputFileName == "")
    {
        std::cout << "-----------" << std::endl;
        std::cerr << "Input file name not provided" << std::endl;
        std::cerr << "Run 'model2code --help' for instructions" << std::endl;
        return RETURN_CODE_ERROR;
    }

    if(fileData.outputPath == "")
    {
        std::cout << "-----------" << std::endl;
        std::cerr << "Output path not provided" << std::endl;
        getPath(fileData.inputFileName, fileData.outputPath);
        if(fileData.outputPath == "")
        {
            std::cerr << "Not able to get output path from input file name" << std::endl;
            std::cerr << "Run 'model2code --help' for instructions" << std::endl;
            return RETURN_CODE_ERROR;
        }
        else
        {
            std::cout << "Got output path from input file name: " << fileData.outputPath << std::endl;
        }
    }

    fileData.outputPathHFile = fileData.outputPath + "/include/";
    fileData.outputPathCppFile = fileData.outputPath + "/src/";

    if(!generator(fileData))
    {
        std::cout << "Error in code generation" << std::endl;
    }
    
    return 0;
};