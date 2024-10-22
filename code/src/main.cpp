// SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
// SPDX-License-Identifier:  APACHE-2.0

/**
 * @file main.cpp
 * @brief this file contains the main function.
 * @version 0.1
 * @date 2024-05-20
 * 
 */

// #include "Replacer.h"
#include "Translator.h"

/**
 * @brief Print the help message
 * 
 */
void print_help()
{
    std::cout << "Welcome to model2code tool.\n";
    std::cout << "Syntax:\n";
    std::cout << "model2code --input_filename \"inputFileName.scxml\" \n";
    std::cout << "--model_filename \"modelFileName.scxml\"\n";
    std::cout << "--interface_filename \"interfaceFileName.scxml\"\n";
    std::cout << "--output_path \"path/to/output/directory\"\n";
    std::cout << "--template_path \"path/to/template_skill/directory\"\n";
    // std::cout << "--datamodel_mode \n";
    std::cout << "--translate_mode \n";
    std::cout << "--generate_mode \n";
}

/**
 * @brief Handle the inputs
 * 
 * @param argc number of input arguments
 * @param argv vector of input arguments
 * @param fileData file data structure passed by reference where the file data is stored
 * @param templateFileData template file data structure passed by reference where the template file data is stored
 * @return true 
 * @return false 
 */
bool handleInputs(int argc, char* argv[], fileDataStr& fileData, templateFileDataStr& templateFileData)
{
    // Default values
    fileData.modelFileName          = modelFilePath;
    fileData.interfaceFileName      = interfaceFilePath;
    fileData.datamodel_mode         = false;
    fileData.translate_mode         = false;
    fileData.generate_mode          = false;
    templateFileData.templatePath   = templateFilePath;

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
        else if (arg == "--template_path" && i+1 < argc && argv[i+1][0] != '-') {
            templateFileData.templatePath = argv[i+1];
            i++;
        }
        else if (arg == "--datamodel_mode") {
            fileData.datamodel_mode = true;
        }
        else if (arg == "--translate_mode") {
            fileData.translate_mode = true;
        }
        else if (arg == "--generate_mode") {
            fileData.generate_mode = true;
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

    fileData.outputPath = fileData.outputPath + "/";
    fileData.outputPathInclude = fileData.outputPath + "/include/";
    fileData.outputPathSrc = fileData.outputPath + "/src/";
    return RETURN_CODE_OK;
}

/**
 * @brief Main function
 * 
 * @param argc number of input arguments
 * @param argv vector of input arguments
 * @return int 
 */
int main(int argc, char* argv[])
{
    fileDataStr fileData;
    templateFileDataStr templateFileData;

    if(handleInputs(argc, argv, fileData, templateFileData))
    {
        return RETURN_CODE_ERROR;
    }

    if(fileData.translate_mode)
    {   
        std::cout << "Translation request" << std::endl;
        if(!Translator(fileData))
        {
            std::cout << "Error in translation" << std::endl;
        }
        
    }
    else if(fileData.generate_mode)
    {
        std::cout << "No translation request" << std::endl;
        if(!Replacer(fileData, templateFileData))
        {
            std::cout << "Error in code generation" << std::endl;
        }
    }
    else
    {
        std::cout << "Translation request" << std::endl;
        if(!Translator(fileData))
        {
            std::cout << "Error in translation" << std::endl;
        }
        fileData.inputFileName = fileData.outputFileTranslatedSM;
        std::cout << "Code generation request" << std::endl;
        if(!Replacer(fileData, templateFileData))
        {
            std::cout << "Error in code generation" << std::endl;
        }
    }
    
    return RETURN_CODE_OK;
};