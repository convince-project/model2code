# SPDX-FileCopyrightText: 2024 Humanoid Sensing and Perception, Istituto Italiano di Tecnologia
# SPDX-License-Identifier:  APACHE-2.0

"""
main.py
This file contains the main function.
"""

import argparse
from python.translator import Translator

def main():
    """
    Main function.
    """
    parser = argparse.ArgumentParser(description="Welcome to model2code tool.")
    parser.add_argument("--model_path", required=True, help="path/to/model.xml")
    parser.add_argument("--template_path", required=True, help="path/to/template_skill/directory")
    parser.add_argument("--output_path", required=True, help="path/to/output/directory")

    args = parser.parse_args()

    translator = Translator(args.model_path, args.template_path, args.output_path)
    if translator.manage_translation():
        print("Translation completed successfully.")
    else:
        print("Translation failed.")

if __name__ == "__main__":
    main()
