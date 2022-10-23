import json
import sys

preprocess_list = [
    "#include <string>\n",
    "#include \"table.h\"\n\n"
]

with open(sys.argv[1], "r") as table_file:
	table_dict : dict = json.load(table_file)

with open(sys.argv[2], "w") as header_file:
    header_file.writelines(preprocess_list)
    #header_file.writelines(structure_def)

    header_file.write("ValveTable testTable = {\n")
    for table_key in table_dict:

        if (table_key == "ra1" or
            table_key == "ra2" or
            table_key == "lobe_sep"):
            header_file.write("\t" + str(table_dict[table_key]) + ",\n")

        elif (table_key == "duration" or
              table_key == "open" or
              table_key == "close" or
              table_key == "lobe_cl"):
            #arr_len = len(table_dict[table_key])

            value_str = " { "
            for value in table_dict[table_key]:
                value_str += str(value) + ","
            value_str += " },\n"
            header_file.write("\tnew int[]" + value_str)

        elif table_key == "lift":
            value_str = " { "
            for value in table_dict[table_key]:
                value_str += str(value) + ","
            value_str += " },\n"
            header_file.write("\tnew double[]" + value_str)

    header_file.write("};\n")
