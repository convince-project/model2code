# MODEL2CODE Overview

> Please access the documentation via [convince-project.github.io/model2code](https://convince-project.github.io/model2code/)

MODEL2CODE is a tool that generates a ROS package with C++ code from a ROS-flavoured SCXML model.

## Installation

MODEL2CODE has been tested on Ubuntu 22.04 with Python 3.10.12.

### Installing Dependencies

MODEL2CODE requires the following dependencies:

* [TinyXML2](https://github.com/leethomason/tinyxml2/) (Tested with v10.0.0) for XML file handling.

To install see [TinyXML2 Installation](https://github.com/leethomason/tinyxml2/tree/master?tab=readme-ov-file#building-tinyxml-2---using-vcpkg)

## Compiling
To compile the code run the following commands:
```
git clone https://github.com/convince-project/model2code.git
cd model2code
mkdir build
cd build
cmake ../code
make install
```

## Execution
To run the `model2code` tool, you will need the following:
- An SCXML file with the state machine of the model;
- An XML file defining the model of your project;
- An XML file defining the interfaces of your project;
- A directory containing templates for the files to be generated.

To run the `model2code` tool use the following command
```
model2code --input_filename "input_model.scxml" --model_filename "project_model_definition.xml" --interface_filename "interface_definition.xml" --output_path "path/to/output/directory" --template_path "path/to/template_skill/directory"
```
replace `input_model.scxml`, `project_model_definition.scxml`, `interface_definition.scxml`, `path/to/output/directory` and `path/to/template_skill/directory` with your needs.

By default the `path/to/output/directory` is set to the location of `input_model.scxml`, and `path/to/template_skill/directory` is set to the 'templates/skills/template_skill' directory of this repository.

Example XML files with the required structure for defining the project's model and interfaces are available in the `template/specifications` folder.

## Run examples
To run an example of MODEL2CODE go to the main directory and run the following commands:
```
model2code --input_filename "templates/skills/first_template_skill/src/FirstTemplateSkillSM.scxml" --model_filename "templates/specifications/full-model.xml" --interface_filename "templates/specifications/interfaces.xml"
```
