import json
import sys

preprocess_list = [
    "#include <string>\n",
    "#include \"table.h\"\n\n"
]

with open(sys.argv[1], "r") as table_file:
	table_dict : dict = json.load(table_file)

with open(sys.argv[2], "w") as header_file:

    primitive_values = ""
    array_values = ""
    array_sizes = ""
    reference_to_arr = ""

    for table_key in table_dict:

        if (table_key == "ra1" or
            table_key == "ra2" or
            table_key == "lobe_sep"):
            primitive_values += "\t" + str(table_dict[table_key]) + ",\n"

        elif (table_key == "duration" or
              table_key == "open" or
              table_key == "close" or
              table_key == "lobe_cl"):
            array_sizes += "\t" + str(len(table_dict[table_key])) + ",\n"
            reference_to_arr += "\t" + table_key + ",\n"

            value_str = "static const int " + table_key + "[] = { "
            for value in table_dict[table_key]:
                value_str += str(value) + ","
            value_str = value_str[:-1]
            value_str += " };\n"
            array_values += value_str

        elif table_key == "lift":
            array_sizes += "\t" + str(len(table_dict[table_key])) + ",\n"
            reference_to_arr += "\t" + table_key + ",\n"

            value_str = "static const double " + table_key + "[] = { "
            for value in table_dict[table_key]:
                value_str += str(value) + ","
            value_str = value_str[:-1]
            value_str += " };\n"
            array_values += value_str

    initialization = "\nValveTable testTable = {\n"
    array_sizes = array_sizes[:-2]
    array_sizes += "\n"

    header_file.writelines(preprocess_list)
    header_file.write(array_values)
    header_file.write(initialization)
    header_file.write(primitive_values)
    header_file.write(reference_to_arr)
    header_file.write(array_sizes)
    header_file.write("};\n")
