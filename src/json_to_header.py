import json
import sys

preprocess_list = [
    "#include <iostream>\n",
    "#include <string>\n"
]

structure_def = [
    "struct valveTable {\n",
    "\tdouble ra1;\n",
    "\tint ra2;\n",
    "\tint lobe_sep;\n",
    "\tint* duration;\n",
    "\tdouble* lift;\n",
    "\tint* open;\n",
    "\tint* close;\n",
    "\tint* lobe_close;\n",
    "};\n",
    "typedef struct valveTable ValveTable;\n"
]

with open(sys.argv[1], "r") as table_file:
	table_dict : dict = json.load(table_file)

with open(sys.argv[2], "w") as header_file:
    header_file.writelines(preprocess_list)
    header_file.writelines(structure_def)

    header_file.write("ValveTable testTable = {\n")
    for val in table_dict:

        if (val == "ra1" or
            val == "ra2" or
            val == "lobe_sep"):
            header_file.write("\t" + str(table_dict[val]) + ",\n")
        elif (val == "duration" or
              val == "open" or
              val == "close" or
              val == "lobe_cl"):
            arr_len = len(table_dict[val])
            header_file.write("\tnew int["+str(arr_len)+"],\n")
        elif val == "lift":
            arr_len = len(table_dict[val])
            header_file.write("\tnew double["+str(arr_len)+"],\n")
    header_file.write("};\n")