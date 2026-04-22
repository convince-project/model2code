Tutorials
=========

This page gives a practical introduction to ``model2code`` using the tutorial skills shipped with the repository.

The main idea is simple: you describe a skill as an SCXML state machine, and ``model2code`` generates the ROS2 package that wraps that state machine with the required ROS interfaces.

Prerequisites
-------------
``model2code`` has been tested on Ubuntu 22.04. Before running the tutorial, build and install the tool as described in the repository ``README``.

Current CLI Notes
-----------------
The current executable accepts the following options:

- ``--input_filename``: required path to the SCXML skill model.
- ``--output_path``: optional path where generated files are written.
- ``--template_path``: optional path to the template package directory.
- ``--verbose_mode``: print the internal log after execution.
- ``--debug_mode``: inject debug logging tags into the translated state machine.
- ``--translate_mode``: run only the translation stage.
- ``--generate_mode``: run only the generation stage.
- ``--datamodel_mode``: enable datamodel file generation.

At the moment, the executable uses built-in default paths for the system model and interface XML files instead of parsing ``--model_filename`` and ``--interface_filename`` from the command line.

The repository still provides the reference specification files in:

- ``tutorials/specifications/full-model.xml``
- ``tutorials/specifications/interfaces.xml``

If your local build expects those files in a different location, update your local setup before running the examples.

Tutorial 1: Action Skill with a Service Call
--------------------------------------------

The first example is ``FirstTutorialSkill``:

- it exposes ``/FirstTutorialSkill/tick``,
- it exposes ``/FirstTutorialSkill/halt``,
- it calls ``/TemplateComponent/Function1`` when ticked,
- it returns success or failure after the service response.

The input model is:

``tutorials/skills/first_tutorial_skill/src/FirstTutorialSkill.scxml``

Conceptually, the skill works like this:

1. In ``idle``, a tick request is received.
2. The skill sends a request to ``/TemplateComponent/Function1`` and moves to ``start``.
3. In ``start``, the skill waits for the service response.
4. The response is converted into a tick reply.
5. A halt request can be handled immediately from either state.

Generate the package
````````````````````

From the repository root, run:

.. code-block:: bash

    model2code \
      --input_filename tutorials/skills/first_tutorial_skill/src/FirstTutorialSkill.scxml \
      --output_path tutorials/skills/first_tutorial_skill

If you want extra logs while learning the flow, add ``--verbose_mode``.

What gets generated
```````````````````

The output folder will contain the main generated package files:

- ``src/FirstTutorialSkillSM.scxml``: the translated standard SCXML model.
- ``src/FirstTutorialSkill.cpp`` and ``include/FirstTutorialSkill.h``: the generated ROS/C++ wrapper.
- ``src/main.cpp``: the executable entry point.
- ``CMakeLists.txt`` and ``package.xml``: build and package metadata.

What to look for
````````````````

This example is useful because it shows the core contract of ``model2code``:

- the SCXML file contains the skill logic,
- the generated C++ files contain the ROS integration layer.

At runtime, a ROS request becomes an SCXML event, the state machine processes it, and the generated wrapper sends the corresponding ROS response or outgoing request.

Tutorial 2: Condition Skill with a Topic Subscription
-----------------------------------------------------

The second example is ``SecondTutorialSkill``:

- it exposes only ``/SecondTutorialSkill/tick``,
- it subscribes to ``/TemplateComponent/Function2``,
- it stores the latest topic value in the datamodel,
- it returns success or failure when ticked depending on that value.

The input model is:

``tutorials/skills/second_tutorial_skill/src/SecondTutorialSkill.scxml``

Generate the package
````````````````````

From the repository root, run:

.. code-block:: bash

    model2code \
      --input_filename tutorials/skills/second_tutorial_skill/src/SecondTutorialSkill.scxml \
      --output_path tutorials/skills/second_tutorial_skill \
      --verbose_mode

Why this example matters
````````````````````````

This second skill shows a different communication pattern:

- there is no halt service,
- the decision is based on subscribed topic data,
- the generated code contains a topic subscriber instead of a service client.

Taken together, the two tutorial skills show how the generated package changes when the SCXML model describes a different kind of ROS interaction.

Suggested Learning Path
-----------------------

If you are new to the project, this order works well:

1. Read ``FirstTutorialSkill.scxml`` to understand the behavior.
2. Generate the package and inspect the translated ``SM.scxml`` file.
3. Open the generated ``.cpp`` and ``.h`` files to see how ROS interfaces were created.
4. Repeat the same process with ``SecondTutorialSkill`` and compare the differences.

That comparison gives a solid first understanding of how ``model2code`` maps high-level SCXML models into concrete ROS2 packages.
