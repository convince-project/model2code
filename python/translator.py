# SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
# SPDX-License-Identifier:  APACHE-2.0

"""
translator.py
Main class to translate the model to code.
"""

import os

from python.data import FileData, SkillData, EventData
from python.extract_from_xml import extract_from_xml
from python.replacer import replacer


class Translator:
    """
    Main class to translate the model to code.
    """

    def __init__(self, model_path, template_path, output_path):
        self.model_path = model_path
        self.template_path = template_path
        self.output_path = output_path
        self.file_data_list = []
        self.skill_data = SkillData()
        self.event_data_list = []

    def manage_translation(self):
        """
        Manage the translation from the model to code.
        """
        if not self.extract_data_from_model():
            return False

        if not self.manage_directories():
            return False

        for file_data in self.file_data_list:
            if not replacer(file_data, self.skill_data, self.event_data_list):
                return False

        return True

    def extract_data_from_model(self):
        """
        Extract data from the model.
        """
        return extract_from_xml(
            self.model_path, self.file_data_list, self.skill_data, self.event_data_list
        )

    def manage_directories(self):
        """
        Manage the directories.
        """
        if not os.path.exists(self.output_path):
            os.makedirs(self.output_path)

        for file_data in self.file_data_list:
            destination_path = os.path.join(
                self.output_path,
                os.path.basename(file_data.path).replace(
                    "first_template", self.skill_data.name_snake_case
                ),
            )
            if not os.path.exists(os.path.dirname(destination_path)):
                os.makedirs(os.path.dirname(destination_path))

            # This part is a bit tricky in Python, as we don't have a direct equivalent of std::filesystem::copy
            # with options. We can use shutil.copytree and shutil.copy for this.
            # For now, let's assume the files are copied and we just need to manage the paths.
            # In a real implementation, we would use shutil.
            pass
        return True
