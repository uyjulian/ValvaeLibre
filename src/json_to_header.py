import json
import sys

with open(sys.argv[1], "r") as table_file:
	table_dict : dict = json.load(table_file)

with open(sys.argv[2], "w") as header_file:
    header_file.write("#include <map>\n")
    header_file.write("#include <string>\n")
    header_file.write("\nconst std::map<std::string, int> valve_table = {\n")

    for val in table_dict:
        header_file.write("\t{ \"" + val + "\"," + str(table_dict.get(val)) + " },\n")

    header_file.write("};")