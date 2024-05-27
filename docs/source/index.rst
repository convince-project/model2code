CONVINCE model2code Documentation
==================================
The tool is developed as part of the CONVINCE project and is mainly used to generate skill-level code starting from an SCXML model.

The inputs of the program are: 

- an SCXML model that describes the behavior of the skill,
- an XML file that describes the full model of the program, which includes the behavior tree, the skills and the components used in the system,
- an XML file that describes the interfaces used between behavior tree and skills, and between skills and components,
- a directory containing the templates of the files to generate.

The output of the program is composed of:

- a C++ file that contains the code of the skill,
- a header file that contains the declaration of the skill,
- a main C++ file that contains the main function of the skill,
- a package.xml file that contains the information of the ROS package,
- a CMakeLists.txt file that includes the generated files.

The output above is only if the data model is ECMAscript, otherwise, you will need to add the parameter ``--datamodel_mode`` to generate the C++ data model files. 
In this case, you will get 2 additional files:

- a C++ file that contains the code of the data model,
- a header file that contains the declaration of the data model. 


The parameters required to run the program are:

- ``--input_filename`` : the path to the SCXML file that describes the behavior of the skill,
- ``--model_filename`` : the path to the XML file that describes the full model of the program,
- ``--interface_filename`` : the path to the XML file that describes the interfaces used.

By default the program will generate the code in the directory where the SCXML file passed by the parameter ``--input_filename`` is located, but you can specify a different directory by using the parameter ``--output_path``.

The skills generated are based on a behavior tree structure and will have a ROS2 tick service in case they are a condition a ROS2 tick and halt services in case they are an action.

Contents
----------
.. toctree::
   :maxdepth: 2

   tutorials
   API/api