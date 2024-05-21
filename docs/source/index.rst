CONVINCE model2code Documentation
==================================
This is the documentation for the CONVINCE model2code tool. The tool is developed as part of the CONVINCE project.
The tool is used mainly to generate skill level code starting from an SCXML model.
The inputs of the program are: 
- an SCXML model that describes the behavior of the skill,
- an XML file that describes the full model of the program, which includes the behavior tree, the skills and the components used in the system,
- an XML file that describes the interfaces used between behavior tree and skills, and between skills and components.

The output of the program is composed of:

- a C++ file that contains the code of the skill,
- an header file that contains the declaration of the skill.

the output above is only if the datamodel is ecmascript, otherwise you will need to add the parameter ``--datamodel_mode`` in order to generate also the C++ datamodel files. 
In this case you will get 2 other files:

- a C++ file that contains the code of the datamodel,
- an header file that contains the declaration of the datamodel. 

In order to complete the code generation you need to write a CMakeLists.txt file that includes the generated files, a main file that includes the skill header and a package.xml in order to create a ROS package.


The parameters required to run the program are:

- ``--input_filename`` : the path to the SCXML file that describes the behavior of the skill,
- ``--model_filename`` : the path to the XML file that describes the full model of the program,
- ``--interface_filename`` : the path to the XML file that describes the interfaces used.

by default the program will generate the code in the directory where the SCXML file passed by the parameter ``--input_filename`` is located, but you can specify a different directory by using the parameter ``--output_path``.


Contents
----------
.. toctree::
   :maxdepth: 2

   tutorials
   API/api