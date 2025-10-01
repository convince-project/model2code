# make a file that tests the code in the model2code project
# This file is used to test the code in the model2code project.
# It is used to test the code in the model2code project.

#!/bin/bash
# take each file in the correct_skills directory and run the model2code command on it
# then check if the output is correct against the files in the hl_scxml folder
# if the output is correct, print "Test passed" and exit with code 0
# if the output is not correct, print "Test failed" and exit with code 1 specifing thge file that failed 

# set the path to the model2code command
MODEL2CODE_CMD="model2code"
# set the path to the correct_skills directory
CORRECT_SKILLS_DIR="./correct_skills"
# set the path to the hl_scxml directory
HL_SCXML_DIR="./hl_scxml"   
# set the path to the output directory
OUTPUT_DIR="./test_compilation/output"
# create the output directory if it does not exist
mkdir -p $OUTPUT_DIR
# loop through each file in the correct_skills directory
for file in $HL_SCXML_DIR/*.scxml; do
    # get the base name of the file (without the path and .scxml extension)
    base_name=$(basename "$file" .scxml)
    # convert it from CamelCase to snake_case
    base_name=$(echo "$base_name" | sed -r 's/([a-z])([A-Z])/\1_\2/g' | tr '[:upper:]' '[:lower:]')
    # remove any leading or trailing whitespace
    base_name=$(echo "$base_name" | xargs)
    # print the base name
    # base_name=$(basename "$file")
    # run the model2code command on the file and redirect the output to the output directory
    echo $MODEL2CODE_CMD --input_filename "$file" --output_path "$OUTPUT_DIR/$base_name" --template_path "../template_skill" #--verbose_mode
    $MODEL2CODE_CMD --input_filename "$file" --output_path "$OUTPUT_DIR/$base_name" --template_path "../template_skill" --verbose_mode
    # check if the output directory exists

    if [ -d "$OUTPUT_DIR/$base_name" ]; then
        # compare the output file with the corresponding file in the hl_scxml directory
        if diff -r -u "$OUTPUT_DIR/$base_name" "$CORRECT_SKILLS_DIR/$base_name" --color; then
            # if the files are the same, print "Test passed for $base_name" and exit with code 0
            echo "Test passed for $base_name"
        else
            # if the files are not the same, print "Test failed for $base_name"
            echo "Test failed for $base_name"
            # exit with code 1
            exit 1
        fi
    else
        # if the output file does not exist, print "Test failed for $base_name"
        echo "Test failed for $base_name: Output folder does not exist"
        # exit with code 1
        exit 1
    fi 
done
