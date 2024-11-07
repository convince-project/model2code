Tutorials
=========

This section provides a tutorial on the model2code tool and how to use it to generate skill-level code from the high-level SCXML model of a skill.

The output code is a ROS package that can be compiled and run on a ROS2 system, that handles the SCXML events and translate them into ROS2 service calls or responses.

Prerequisites
--------------
MODEL2CODE has been tested on Ubuntu 22.04 with Python 3.10.12.

Before you can use the model2code tool, you need to follow the dependency installation and compiling steps in the model2code repository `README <https://github.com/convince-project/model2code/blob/dev/README.md>`_.

Code generation guide
--------------------------------------------

Input and Output Models
```````````````
The main input is an SCXML model, extended with ROS-specific features, that describes the behavior of the skill. Visit the `(ROS) SCXML Model Implementation <https://convince-project.github.io/AS2FM/howto.html#creating-an-scxml-model-of-a-ros-node>`_ description page for more information on how to create an SCXML model of a ROS node.
The required inputs are also the files defining the system model and interfaces. The `full-model.xml <https://github.com/convince-project/model2code/blob/main/tutorials/specifications/full-model.xml>`_ file contains a list of the components and specifies their interfaces, which are described in the `interfaces.xml <https://github.com/convince-project/model2code/blob/main/tutorials/specifications/interfaces.xml>`_ file. 
The output is a ROS2 package containing the generated code. The package includes the following files: a standard SCXML file, C++ source and header files, a main C++ file, a package.xml file, and a CMakeLists.txt file. The templates for the generated files are located in the `templates <https://github.com/convince-project/model2code/tree/main/template_skill>`_ folder.

The `tutorial_skills <https://github.com/convince-project/model2code/blob/main/tutorials/skills>`_ are written for a system based on a behavior tree structure. The leaf nodes of the behavior tree are conditions or actions handled by plugins that propagate the tick from the behavior tree to the skills.
Skills corresponding to condition nodes will have a ROS2 tick service, while skills corresponding to action nodes will have both tick and halt services. Skills can interface with components through ROS2 services and topics. 
The interface of the 'TemplateComponent' is the 'template_interfaces', which includes two functions 'Function1' and 'Function2', respectively a sample service and a topic.

Parameters
```````````````
To run the model2code use the following parameters:
 - `--input_filename` (required): The path to the SCXML file that describes the behavior of the skill.
 - `--model_filename` (required): The path to the XML file that describes the full model of the program.
 - `--interface_filename` (required): The path to the XML file that describes the interfaces used.
 - `--template_path`: The path to the directory containing the templates for the files to be generated. By default, the program uses the `templates` directory.
 - `--output_path`: The path to the directory where the generated files will be placed. By default, the program generates the code in the same directory as the SCXML file specified by the `--input_filename` parameter.

First example
```````````````
The `first_tutorial_skill <https://github.com/convince-project/model2code/blob/main/tutorials/skills/first_tutorial_skill/src/FirstTutorialSkill.scxml>`_ is a simple skill corresponding to an action node of the behavior tree.
It provides ROS2 services for tick and halt, and it is a client for the 'Function1' service provided by the 'TemplateComponent'.
The model is composed of two states. In the 'idle' state, when the skill receives a tick request it sends a request to the 'Function1' service and goes to the 'start' state. In the 'start' state it waits for the response from the 'Function1' service and eventually responds to the tick request. In both states when the skill receives a halt request it responds right away.

To generate the code for this skill, move to the model2code folder and run the following command:

.. code-block:: bash
    
    model2code --input_filename "tutorials/skills/first_tutorial_skill/src/FirstTutorialSkill.scxml" --model_filename "tutorials/specifications/full-model.xml" --interface_filename "tutorials/specifications/interfaces.xml" --output_path "tutorials/skills/first_tutorial_skill"

The generated code will be placed in the `tutorials/skills/first_tutorial_skill` folder. 
The 'FirstTutorialSkillSM.scxml' file is the translation of the input SCXML model in standard SCXML, so that it is runnable with any SCXML compiler.
The 'FirstTutorialSkill.cpp' and 'FirstTutorialSkill.h' files contain the C++ code of the skill that handles the SCXML events and ROS2 communication.
The 'main.cpp' file contains the main function of the skill.
The CMakeLists.txt file incorporates the generated files.
The package.xml file with information about the ROS package and the inclusion of the 'template_interfaces' package as a dependency.


Second example
```````````````
The `second_tutorial_skill <https://github.com/convince-project/model2code/blob/main/tutorials/skills/second_tutorial_skill/src/SecondTutorialSkill.scxml>`_ corresponds to a condition node of the behavior tree, therefore it does not provide a halt service.
It provides ROS2 services for tick, and is a subscriber for the 'Function2' topic provided by the 'TemplateComponent'.

To generate the code for this skill, move to the model2code folder and run the following command:

.. code-block:: bash
    
    model2code --input_filename "tutorials/skills/second_tutorial_skill/src/SecondTutorialSkill.scxml" --model_filename "tutorials/specifications/full-model.xml" --interface_filename "tutorials/specifications/interfaces.xml" --output_path "tutorials/skills/second_tutorial_skill"

The generated code will be placed in the `tutorials/skills/second_tutorial_skill` folder. The structure of generated files follows that of the first example, with the only difference being that 'SecondTutorialSkill.cpp' omits the halt service and includes a subscriber for 'Function2'.