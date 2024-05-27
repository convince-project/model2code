# MODEL2CODE Overview

> Please access the documentation via [convince-project.github.io/model2code](https://convince-project.github.io/model2code/)

MODEL2CODE is a tool that generates CPP code from a SCXML model.

## Installation

MODEL2CODE has been tested on Ubuntu 22.04 with Python 3.10.12.

### Installing Dependencies

MODEL2CODE requires the following dependencies:

* [TinyXML2](https://github.com/leethomason/tinyxml2/) (Tested with v10.0.0)

To install see [TinyXML2 Installation](https://github.com/leethomason/tinyxml2/tree/master?tab=readme-ov-file#building-tinyxml-2---using-vcpkg)

## Compiling
To compile the code run the following commands
'''
git clone https://github.com/convince-project/model2code.git
cd model2code/code
mkdir build
cd build
cmake ../code
make install
'''

## Execution
To run the `model2code` tool you need three different files:
- An SCXML file with your state machine
- An XML file with the definition of your model
- An XML file with the definition of your interfaces
- A directory containing the templates of the files to generate

To run the `model2code` tool use the following command
```
model2code --input_filename "input_filename.scxml" --model_filename "model_filename.scxml" --interface_filename "interface_filename.scxml" --output_path "path/to/output/directory" --template_path "path/to/template_skill/directory"
```
replace `input_filename.scxml`, `model_filename.scxml`, `interface_filename.scxml`, `path/to/output/directory` and `path/to/template_skill/directory` with your needs.
By default the `path/to/output/directory` is set to the location of `input_filename.scxml` and `path/to/template_skill/directory` is set to the 'templates/skills/template_skill' directory of this repository.

## Run examples
To run an example of MODEL2CODE go to the main directory and run the following commands:
```
model2code --input_filename "templates/skills/first_template_skill/src/FirstTemplateSkillSM.scxml" --model_filename "templates/specifications/full-model.xml" --interface_filename "templates/specifications/interfaces.xml"
```