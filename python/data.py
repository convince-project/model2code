# SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
# SPDX-License-Identifier:  APACHE-2.0

import sys

log_str = ""

class EventData:
    def __init__(self):
        self.event = ""
        self.target = ""
        self.eventName = ""
        self.eventType = ""
        self.paramMap = {}
        self.componentName = ""
        self.functionName = ""
        self.functionNameSnakeCase = ""
        self.nodeName = ""
        self.serverName = ""
        self.clientName = ""
        self.topicName = ""
        self.interfaceName = ""
        self.interfaceType = ""
        self.scxmlInterfaceName = ""
        self.virtualInterface = False
        self.interfaceTopicFields = []
        self.interfaceRequestFields = []
        self.interfaceResponseFields = []
        self.interfaceData = []

class SkillData:
    def __init__(self):
        self.SMName = ""
        self.skillName = ""
        self.className = ""
        self.classNameSnakeCase = ""
        self.skillType = ""
        self.skillTypeLC = ""

class FileData:
    def __init__(self):
        self.outputFileNameCPP = ""
        self.outputFileNameH = ""
        self.outputDatamodelFileNameH = ""
        self.outputDatamodelFileNameCPP = ""
        self.outputMainFileName = ""
        self.outputCMakeListsFileName = ""
        self.outputPackageXMLFileName = ""

def get_data_from_event(event_data):
    event = event_data.event
    if not event:
        print("Event has no value", file=sys.stderr)
        return

    parts = event.split('.')
    if len(parts) == 3:
        event_data.componentName = parts[0]
        event_data.functionName = parts[1]
        event_data.eventName = parts[2]
    else:
        print(f"Error in name format for event: {event}", file=sys.stderr)

def get_data_from_root_name(attribute_name, skill_data):
    if attribute_name:
        add_to_log(f"Root attribute name: {attribute_name}")
        if "Skill" in attribute_name:
            skill_data.SMName = attribute_name
            skill_data.skillName = attribute_name.split("Skill")[0]
            skill_data.className = skill_data.skillName + "Skill"
            skill_data.classNameSnakeCase = to_snake_case(skill_data.className)
            skill_data.skillType = attribute_name.split("Skill")[1]
            if not skill_data.skillType:
                print("Skill type not found", file=sys.stderr)
                return False
            skill_data.skillTypeLC = skill_data.skillType.lower()
        else:
            print("Skill name not found", file=sys.stderr)
            return False
    else:
        print("Attribute 'name' not found or has no value", file=sys.stderr)
        return False
    return True

def to_snake_case(s):
    return ''.join(['_' + c.lower() if c.isupper() else c for c in s]).lstrip('_')

def print_event_data(event_data):
    add_to_log("-----------")
    add_to_log("Event data:")
    add_to_log(f"\tevent={event_data.event}, target={event_data.target}")
    add_to_log(f"\teventName={event_data.eventName}, eventType={event_data.eventType}")
    add_to_log("\tparamMap:")
    for key, value in event_data.paramMap.items():
        add_to_log(f"\t\t{key}: {value}")
    add_to_log(f"\tcomponentName={event_data.componentName}")
    add_to_log(f"\tfunctionName={event_data.functionName}")
    add_to_log(f"\tfunctionNameSnakeCase={event_data.functionNameSnakeCase}")
    add_to_log(f"\tnodeName={event_data.nodeName}")
    add_to_log(f"\tserverName={event_data.serverName}")
    add_to_log(f"\tclientName={event_data.clientName}")
    add_to_log(f"\ttopicName={event_data.topicName}")
    add_to_log(f"\tinterfaceName={event_data.interfaceName}")
    add_to_log(f"\tinterfaceType={event_data.interfaceType}")
    add_to_log(f"\tscxmlInterfaceName={event_data.scxmlInterfaceName}")
    add_to_log(f"\tvirtualInterface={event_data.virtualInterface}")
    add_to_log("\tinterfaceTopicFields:")
    for field in event_data.interfaceTopicFields:
        add_to_log(f"\t\t{field}")
    add_to_log("\tinterfaceRequestFields:")
    for field in event_data.interfaceRequestFields:
        add_to_log(f"\t\t{field}")
    add_to_log("\tinterfaceResponseFields:")
    for field in event_data.interfaceResponseFields:
        add_to_log(f"\t\t{field}")
    add_to_log("\tinterfaceData:")
    for data in event_data.interfaceData:
        add_to_log(f"\t\t{data[1]} {data[0]}")
    add_to_log("-----------")

def print_event_data_to_cerr(event_data):
    print("-----------", file=sys.stderr)
    print("Event data: ", file=sys.stderr)
    print(f"\tevent={event_data.event}, target={event_data.target}", file=sys.stderr)
    print(f"\teventName={event_data.eventName}, eventType={event_data.eventType}", file=sys.stderr)
    print("\tparamMap:", file=sys.stderr)
    for key, value in event_data.paramMap.items():
        print(f"\t\t{key}: {value}", file=sys.stderr)
    print(f"\tcomponentName={event_data.componentName}", file=sys.stderr)
    print(f"\tfunctionName={event_data.functionName}", file=sys.stderr)
    print(f"\tfunctionNameSnakeCase={event_data.functionNameSnakeCase}", file=sys.stderr)
    print(f"\tnodeName={event_data.nodeName}", file=sys.stderr)
    print(f"\tserverName={event_data.serverName}", file=sys.stderr)
    print(f"\tclientName={event_data.clientName}", file=sys.stderr)
    print(f"\ttopicName={event_data.topicName}", file=sys.stderr)
    print(f"\tinterfaceName={event_data.interfaceName}", file=sys.stderr)
    print(f"\tinterfaceType={event_data.interfaceType}", file=sys.stderr)
    print(f"\tscxmlInterfaceName={event_data.scxmlInterfaceName}", file=sys.stderr)
    print(f"\tvirtualInterface={event_data.virtualInterface}", file=sys.stderr)
    print("\tinterfaceTopicFields:", file=sys.stderr)
    for field in event_data.interfaceTopicFields:
        print(f"\t\t{field}", file=sys.stderr)
    print("\tinterfaceRequestFields:", file=sys.stderr)
    for field in event_data.interfaceRequestFields:
        print(f"\t\t{field}", file=sys.stderr)
    print("\tinterfaceResponseFields:", file=sys.stderr)
    for field in event_data.interfaceResponseFields:
        print(f"\t\t{field}", file=sys.stderr)
    print("\tinterfaceData:", file=sys.stderr)
    for data in event_data.interfaceData:
        print(f"\t\t{data[1]} {data[0]}", file=sys.stderr)
    print("-----------", file=sys.stderr)

def print_skill_data(skill_data):
    add_to_log("-----------")
    add_to_log(f"Class name: {skill_data.className}\nSkill name: {skill_data.skillName}\nSkill type: {skill_data.skillType}")
    add_to_log("-----------")

def set_file_data(file_data, skill_data):
    file_data.outputFileNameCPP = f"{skill_data.className}.cpp"
    file_data.outputFileNameH = f"{skill_data.className}.h"
    file_data.outputDatamodelFileNameH = f"{skill_data.className}DataModel.h"
    file_data.outputDatamodelFileNameCPP = f"{skill_data.className}DataModel.cpp"
    file_data.outputMainFileName = "main.cpp"
    file_data.outputCMakeListsFileName = "CMakeLists.txt"
    file_data.outputPackageXMLFileName = "package.xml"

def add_to_log(message):
    global log_str
    log_str += message + "\n"

def print_log():
    print(log_str)
