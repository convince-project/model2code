# SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
# SPDX-License-Identifier:  APACHE-2.0

"""
str_manipulation.py
Functions to manipulate strings.
"""

import re
import os
from pathlib import Path

def to_snake_case(input_str):
    """
    Convert string to snake case.
    
    :param input_str: string to be converted
    :return: string converted to snake case
    """
    if not input_str:
        return ""
    # Add underscore before uppercase letters, but not at the start
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', input_str)
    # Add underscore before a sequence of uppercase letters followed by a lowercase letter
    s2 = re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1)
    return s2.lower()

def get_data_type_path(input_str):
    """
    Get data type path from data type name 
    (e.g. from "sensor_msgs::msg::BatteryState" to "sensor_msgs/msg/battery_state")
    
    :param input_str: string of the data type name
    :return: string of the data type path
    """
    if not input_str:
        print("Input DataType has no value")
        return ""
    
    temp = input_str.replace("::", "/")
    return to_snake_case(temp)

def get_path(file_path):
    """
    Get path of the file main folder from the file path
    (e.g. "templates/skills/first_template_skill/src/FirstTemplateSkillSM.scxml" -> "templates/skills/first_template_skill/")
    
    :param file_path: file path
    :return: string of the path
    """
    try:
        # Two levels up from the file path
        path = Path(file_path)
        # .parent.parent gives the grandparent directory. We add a trailing slash.
        return str(path.parent.parent) + "/"
    except IndexError:
        return ""

def read_template_file(file_path):
    """
    function to read the template file
    
    :param file_path: constant string of the file path
    :return: file content as a string, or None if an error occurs
    """
    try:
        with open(file_path, 'r') as read_file:
            return read_file.read()
    except IOError as e:
        print(f"Failed to open or read template file: {file_path}")
        print(f"Error: {e}")
        return None
