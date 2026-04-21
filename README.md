# model2code

`model2code` is a command-line tool that takes a ROS-flavoured SCXML skill model and generates a ROS 2 package from it.

## What it does

You give `model2code`:

- an SCXML skill file,
- an output folder,
- a template folder.

It generates a ROS 2 package containing the source files and build files for that skill.

## What you need

To build `model2code` itself you need:

- CMake 3.16+
- a C++20 compiler
- TinyXML2

To build the generated package later, you will also need the ROS 2 and Qt dependencies required by that package.

For the tutorial packages in this repository, you will typically need:

- ROS 2
- `colcon`
- Qt 6 development packages, including SCXML

On Ubuntu/Debian, the missing Qt packages that blocked tutorial execution were fixed with:

```bash
sudo apt update
sudo apt install qt6-base-dev qt6-scxml-dev
```

## Build the tool

From the repository root:

```bash
mkdir -p build
cd build
cmake ..
make
cd ..
```

After this, the executable is available as:

```bash
./build/model2code
```

### Optional install

If you want to install it system-wide:

```bash
cd build
sudo make install
```

If you do not want to use `sudo`, install it in your home directory instead:

```bash
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local
make install
```

## Basic usage

Run `model2code` from the repository root like this:

```bash
./build/model2code \
  --input_filename "path/to/YourSkill.scxml" \
  --output_path "path/to/output_skill" \
  --template_path "./template_skill"
```

### Main options

- `--input_filename`: path to the SCXML skill file. Required.
- `--output_path`: folder where the generated package will be written.
- `--template_path`: template folder to use. In this repository, use `./template_skill`.
- `--verbose_mode`: prints more information while the tool runs.
- `--debug_mode`: adds extra log entries in the generated state machine.
- `--datamodel_mode`: also generates datamodel-related files.

In normal use, the basic three paths are enough:

- input file,
- output path,
- template path.

## Typical workflow

1. Build the tool.
2. Choose an SCXML skill file.
3. Run `./build/model2code` from the repository root.
4. Open the generated package in the output folder.
5. Add that package to your ROS 2 workspace and build it there.

## What gets generated

The output folder typically contains:

- `include/`
- `src/`
- `CMakeLists.txt`
- `package.xml`
- `src/main.cpp`
- `<SkillName>.cpp` and `<SkillName>.h`
- `<SkillName>SM.scxml`

If you use `--datamodel_mode`, extra datamodel files are generated as well.

## Fastest way to try it

This repository already contains tutorial examples. The commands below are meant to be executed from the repository root.

### Example 1: `FirstTutorialSkill`

Input file: [tutorials/skills/first_tutorial_skill/src/FirstTutorialSkill.scxml](/home/bsquitieri-iit.local/model2code/tutorials/skills/first_tutorial_skill/src/FirstTutorialSkill.scxml)

Generate the package:

```bash
./build/model2code \
  --input_filename "tutorials/skills/first_tutorial_skill/src/FirstTutorialSkill.scxml" \
  --output_path "tutorials/skills/first_tutorial_skill" \
  --template_path "./template_skill"
```

This example is useful for generation, but it is not self-contained for execution. To build and run it you would still need:

- the generated package `first_tutorial_skill`,
- `bt_interfaces_dummy`,
- a ROS package called `template_interfaces`,
- a running node that provides the service `/TemplateComponent/Function1`.

- `template_interfaces` is not included in this repository,
- the service server for `/TemplateComponent/Function1` is not included in this repository either.

Because of that, `FirstTutorialSkill` should be treated here as a generation example, not as the recommended end-to-end execution example.

If you already have your own `template_interfaces` package and a node exposing `/TemplateComponent/Function1`, you can build it with:

```bash
mkdir -p tutorial_ws/src
ln -s "$(pwd)/tutorials/skills/first_tutorial_skill" tutorial_ws/src/
ln -s "$(pwd)/tests/test_compilation/interfaces/bt_interfaces_dummy" tutorial_ws/src/
# Add your template_interfaces package to tutorial_ws/src/ as well

cd tutorial_ws
colcon build --packages-select bt_interfaces_dummy first_tutorial_skill template_interfaces
source install/setup.bash
ros2 run first_tutorial_skill first_tutorial_skill
```

Once the skill is running, you can interact with it with:

```bash
ros2 service call /FirstTutorialSkill/tick bt_interfaces_dummy/srv/TickAction "{}"
ros2 service call /FirstTutorialSkill/halt bt_interfaces_dummy/srv/HaltAction "{}"
```

### Example 2: `SecondTutorialSkill`

Input file: [tutorials/skills/second_tutorial_skill/src/SecondTutorialSkill.scxml](/home/bsquitieri-iit.local/model2code/tutorials/skills/second_tutorial_skill/src/SecondTutorialSkill.scxml)

Generate the package:

```bash
./build/model2code \
  --input_filename "tutorials/skills/second_tutorial_skill/src/SecondTutorialSkill.scxml" \
  --output_path "tutorials/skills/second_tutorial_skill" \
  --template_path "./template_skill"
```

This is the recommended tutorial to run end-to-end because it only needs:

- the generated package `second_tutorial_skill`,
- `bt_interfaces_dummy`,
- `std_msgs`.

Build and run it:

```bash
mkdir -p tutorial_ws/src
ln -s "$(pwd)/tutorials/skills/second_tutorial_skill" tutorial_ws/src/
ln -s "$(pwd)/tests/test_compilation/interfaces/bt_interfaces_dummy" tutorial_ws/src/

cd tutorial_ws
colcon build --packages-select bt_interfaces_dummy second_tutorial_skill
source install/setup.bash
ros2 run second_tutorial_skill second_tutorial_skill
```

In another terminal:

```bash
cd ~/model2code/tutorial_ws
source install/setup.bash
ros2 topic pub --once /TemplateComponent/Function2 std_msgs/msg/Bool "{data: true}"
ros2 service call /SecondTutorialSkill/tick bt_interfaces_dummy/srv/TickCondition "{}"
```

## Notes for everyday use

- The examples above assume you are in the repository root.
- If `./build/model2code` does not exist, build the project first.
- If `make install` fails with `Permission denied`, use `sudo make install` or run the binary directly from `./build/model2code`.
- If you just want to try the tool, you do not need to install it.
- Use `--verbose_mode` only if you want internal debug output from the generator.

## Minimal example

```bash
./build/model2code \
  --input_filename "my_skill/src/MySkill.scxml" \
  --output_path "generated_skills/my_skill" \
  --template_path "./template_skill"
```

## Summary

Use `model2code` like this:

1. build it,
2. run it from the repository root,
3. pass your SCXML file, output folder, and template folder,
4. take the generated package and build it in your ROS 2 workspace.
