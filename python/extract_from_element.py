# SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
# SPDX-License-Identifier:  APACHE-2.0

import xml.etree.ElementTree as ET
import sys

def get_element_att_value(element, attribute):
    return element.get(attribute)

def get_element_text(element):
    return element.text

def find_element_by_tag_and_att_value(root, tag, attribute_name, attribute_value):
    for element in root.iter(tag):
        if element.get(attribute_name) == attribute_value:
            print(f"Found element with tag '{tag}' and attribute '{attribute_name}' with value '{attribute_value}'", file=sys.stderr)
            return element
    return None

def find_element_by_tag(root, tag):
    return root.find(f".//{tag}")

def find_element_vector_by_tag_and_attribute(root, tag, attribute):
    elements = []
    for element in root.iter(tag):
        if attribute in element.attrib:
            elements.append(element)
    return elements

def find_element_vector_by_tag(root, tag):
    return list(root.iter(tag))
