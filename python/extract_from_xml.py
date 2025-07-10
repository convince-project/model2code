# SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
# SPDX-License-Identifier:  APACHE-2.0

import xml.etree.ElementTree as ET
import sys
from . import data
from . import extract_from_element as efe
from .data import add_to_log, print_event_data

def extract_interface_data(file_data, event_data):
    element = find_interface_type(file_data, event_data)
    add_to_log("******************************** event DATA AFTER ********************************\n")
    print_event_data(event_data)
    add_to_log("******************************** event DATA AFTER END ********************************\n")

    if element is None:
        return False
        
    return True


def find_interface_type(file_data, event_data): 
    try:
        tree = ET.parse(file_data.inputFileName)
        root = tree.getroot()
    except (ET.ParseError, FileNotFoundError):
        print(f"Failed to load '{file_data.inputFileName}' file", file=sys.stderr)
        return None

    component_name = event_data.componentName
    function_name = event_data.functionName
    
    # ROS SERVICE SERVER
    service_name = f"/{component_name}/{function_name}"
    element = efe.find_element_by_tag_and_att_value(root, "ros_service_server", "service_name", service_name)
    if element is not None:
        add_to_log(f"is_ros_service_server: True")
        event_data.messageInterfaceType = efe.get_element_att_value(element, "type")
        if not event_data.messageInterfaceType:
            print(f"No type attribute found in ros_service_server element for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        
        event_data.rosInterfaceType = "async-service"
        event_data.serverName = service_name
        event_data.clientName = service_name

        # handle request fields
        field_parent = efe.find_element_by_tag_and_att_value(root, "ros_service_handle_request", "name", event_data.serverName)
        if field_parent is None:
            print(f"No ros_service_handle_request element found for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        if not get_interface_fields_from_expr(field_parent, event_data.interfaceRequestFields):
            print(f"Failed to get interface request fields for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None

        # handle response fields
        response_parent = efe.find_element_by_tag_and_att_value(root, "ros_service_send_response", "name", event_data.clientName)
        if response_parent is None:
            print(f"No ros_service_send_response element found for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        if not get_interface_fields_from_name(response_parent, event_data.interfaceResponseFields):
            print(f"Failed to get interface response fields for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        
        add_to_log(f"fieldParent: {field_parent.tag}")
        return element

    # ROS SERVICE CLIENT
    element = efe.find_element_by_tag_and_att_value(root, "ros_service_client", "service_name", service_name)
    if element is not None:
        add_to_log(f"is_ros_service_client: True")
        event_data.messageInterfaceType = efe.get_element_att_value(element, "type")
        if not event_data.messageInterfaceType:
            print(f"No type attribute found in ros_service_client element for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None

        event_data.rosInterfaceType = "async-service"
        event_data.clientName = service_name
        event_data.serverName = service_name

        # handle request fields
        field_parent = efe.find_element_by_tag_and_att_value(root, "ros_service_send_request", "name", service_name)
        if field_parent is None:
            print(f"No ros_service_send_request element found for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        add_to_log(f"fieldParent: {field_parent.tag}")
        if not get_interface_fields_from_expr(field_parent, event_data.interfaceRequestFields):
            print(f"Failed to get interface request fields for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None

        # handle response fields
        response_parent = efe.find_element_by_tag_and_att_value(root, "ros_service_handle_response", "name", service_name)
        if response_parent is None:
            print(f"No ros_service_handle_response element found for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        add_to_log(f"responseParent: {response_parent.tag}")
        if not get_interface_fields_from_name(response_parent, event_data.interfaceResponseFields):
            print(f"Failed to get interface response fields for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        
        return element

    # ROS TOPIC PUBLISHER
    topic_name = f"/{component_name}/{function_name}"
    element = efe.find_element_by_tag_and_att_value(root, "ros_topic_publisher", "topic", topic_name)
    if element is not None:
        add_to_log(f"is_topic_publisher: True")
        event_data.rosInterfaceType = "topic"
        event_data.messageInterfaceType = efe.get_element_att_value(element, "type")
        event_data.scxmlInterfaceName = efe.get_element_att_value(element, "name")
        event_data.topicName = efe.get_element_att_value(element, "topic")
        
        field_parent = efe.find_element_by_tag_and_att_value(root, "ros_topic_publish", "name", event_data.scxmlInterfaceName)
        if field_parent is None:
            print(f"No ros_topic_publish element found for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        if not get_interface_fields_from_name(field_parent, event_data.interfaceTopicFields):
            print(f"Failed to get interface topic fields for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        return element

    # ROS TOPIC SUBSCRIBER
    element = efe.find_element_by_tag_and_att_value(root, "ros_topic_subscriber", "topic", topic_name)
    if element is not None:
        add_to_log(f"is_topic_subscriber: True")
        print(f"Found ros_topic_subscriber for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
        event_data.rosInterfaceType = "topic"
        event_data.messageInterfaceType = efe.get_element_att_value(element, "type")
        event_data.topicName = efe.get_element_att_value(element, "topic")
        print(f"Topic name: {event_data.topicName}", file=sys.stderr)

        field_parent = efe.find_element_by_tag_and_att_value(root, "ros_topic_callback", "name", event_data.topicName)
        if field_parent is None:
            print(f"No ros_topic_callback element found for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        print(f"fieldParent: {field_parent.tag}", file=sys.stderr)
        if not get_interface_fields_from_expr(field_parent, event_data.interfaceTopicFields):
            print(f"Failed to get interface topic fields for component '{component_name}' and function '{function_name}' in file '{file_data.inputFileName}'.", file=sys.stderr)
            return None
        
        return element

    # ROS ACTION SERVER
    action_name = f"/{component_name}/{function_name}"
    element = efe.find_element_by_tag_and_att_value(root, "ros_action_server", "action_name", action_name)
    if element is not None:
        add_to_log(f"is_action_server: True")
        event_data.messageInterfaceType = efe.get_element_att_value(element, "type")
        event_data.rosInterfaceType = "action"
        event_data.serverName = action_name
        return element

    # ROS ACTION CLIENT
    element = efe.find_element_by_tag_and_att_value(root, "ros_action_client", "action_name", action_name)
    if element is not None:
        add_to_log(f"is_action_client: True")
        event_data.messageInterfaceType = efe.get_element_att_value(element, "type")
        event_data.rosInterfaceType = "action"
        event_data.clientName = action_name
        return element

    return None

def get_interface_fields_from_name(element, interface_fields):
    if element is None:
        print("Element is null", file=sys.stderr)
        return False
    
    for field_element in element.findall("assign"):
        field_name = field_element.get("expr")
        if field_name:
            if '.' in field_name:
                field_name_str = field_name.split('.', 1)[1]
                interface_fields.append(field_name_str)
    return True

def get_interface_fields_from_expr(element, interface_fields):
    if element is None:
        print("Element is null", file=sys.stderr)
        return False
    
    for field_element in element.findall("field"):
        field_name = field_element.get("name")
        if field_name:
            print(f"Found field: {field_name}", file=sys.stderr)
            interface_fields.append(field_name)
    return True

def extract_interface_name(file_name, event_data):
    component_name = event_data.componentName
    try:
        tree = ET.parse(file_name)
        root = tree.getroot()
    except (ET.ParseError, FileNotFoundError):
        print(f"Failed to load '{file_name}' file", file=sys.stderr)
        return False

    element = efe.find_element_by_tag_and_att_value(root, "componentDeclaration", "id", component_name)
    if element is None:
        print(f"\nNo component '{component_name}' found in file '{file_name}'.", file=sys.stderr)
        return False
    
    interface_name = efe.get_element_att_value(element, "interface")
    if not interface_name:
        print(f"\nNo interface found for component '{component_name}'.", file=sys.stderr)
        return False
    
    event_data.interfaceName = interface_name
    return True

def extract_interface_type(file_name, event_data):
    interface_name = event_data.interfaceName
    function_name = event_data.functionName
    
    add_to_log(f"opening file: {file_name}")
    print(f"Extracting interface type from file: {file_name}", file=sys.stderr)
    
    try:
        tree = ET.parse(file_name)
        root = tree.getroot()
    except (ET.ParseError, FileNotFoundError):
        print(f"Failed to load '{file_name}' file", file=sys.stderr)
        return False

    element_interface = efe.find_element_by_tag_and_att_value(root, "interface", "id", interface_name)
    if element_interface is None:
        print(f"\nNo interface '{interface_name}' found in file '{file_name}'.", file=sys.stderr)
        return False

    event_data.virtualInterface = efe.get_element_att_value(element_interface, "virtual") == "true"

    element_function = efe.find_element_by_tag_and_att_value(element_interface, "function", "id", function_name)
    if element_function is None:
        print(f"No function '{function_name}' found in file '{file_name}'.", file=sys.stderr)
        return False

    element_interface_type = efe.find_element_by_tag(element_function, "interface")
    if element_interface_type is None:
        return False
        
    interface_type = efe.get_element_att_value(element_interface_type, "type")
    if not interface_type:
        return False
    
    event_data.interfaceType = interface_type

    element_data_field = efe.find_element_by_tag(element_function, "dataField")
    if element_data_field is None:
        add_to_log(f"No tag <dataField> for function '{function_name}'.")
        return True
        
    interface_data_field = efe.get_element_text(element_data_field)
    if not interface_data_field:
        add_to_log(f"No value in tag <dataField> for function '{function_name}'.")
        return True

    element_data_type = efe.find_element_by_tag(element_function, "dataType")
    if element_data_type is None:
        add_to_log(f"No tag <dataType> for function '{function_name}'.")
        return True
        
    return True

def extract_from_scxml(file_name):
    add_to_log(f"opening file: {file_name}")
    try:
        tree = ET.parse(file_name)
        doc = tree.getroot()
    except (ET.ParseError, FileNotFoundError):
        print(f"Failed to load '{file_name}' file", file=sys.stderr)
        return None, None, None

    root_name = doc.get("name")
    if not root_name:
        print("No root name found", file=sys.stderr)
        return None, None, None

    elements_transition = efe.find_element_vector_by_tag_and_attribute(doc, "transition", "event")
    if not elements_transition:
        add_to_log("No transition elements found.")
    else:
        add_to_log("Transition elements found.")

    elements_send = efe.find_element_vector_by_tag_and_attribute(doc, "send", "event")
    if not elements_send:
        add_to_log("No Send elements found.")
    else:
        add_to_log("Send elements found.")
        
    return root_name, elements_transition, elements_send
