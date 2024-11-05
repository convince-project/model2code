CONVINCE model2code Documentation
==================================
The model2code tool is developed as part of the CONVINCE project and is used to generate skill-level code from the high-level SCXML model of a skill.

Program inputs: 

- An SCXML model that describes the behavior of the skill,
- An XML file that outlines the full model of the program, including the behavior tree, skills, and components used in the system.
- An XML file that specifies the interfaces between the behavior tree and skills, as well as between skills and components.
- A directory containing templates for the files to be generated.

Program outputs:

- A C++ file containing skill code.
- A header file declaring the skill.
- A main C++ file that includes the main function of the skill.
- A package.xml file with information about the ROS package.
- A CMakeLists.txt file that incorporates the generated files.


Required parameters:

- ``--input_filename``: The path to the SCXML file that describes the behavior of the skill.
- ``--model_filename``: The path to the XML file that describes the full model of the program.
- ``--interface_filename``: The path to the XML file that describes the interfaces used.

By default, the program generates the code in the same directory as the SCXML file specified by the ``--input_filename`` parameter. However, you can select a different output directory by using the ``--output_path`` parameter.

Additionally, the program uses files from the ``templates`` directory by default to generate the code, but you can specify a different directory with the ``--templates_path`` parameter.

The generated skills are based on a behavior tree structure. Skills defined as conditions will have a ROS2 tick service, while skills defined as actions will have both tick and halt services.

Contents
----------
.. toctree::
   :maxdepth: 2

   tutorials
   API/api