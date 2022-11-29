import json
import sys

def set_bit(number : int, bit : int) -> int:
    return number | (1<<bit)

# -------------- BITS FOR VALVES -----------------------
INTAKE_BITS = (31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16)

EXHAUST_BITS = (15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

# -------------- OFFSETS FOR DIFFERENT ENGINES ----------
offsets = {
    4: 180,
    6: 120,
    8: 90
}
# -------------- TABLE -----------------------------------
TABLE_SIZE = 720
my_table = [0b00000000000000000000000000000000] * TABLE_SIZE

# -------------- PREPROCESS includes ---------------------
preprocess_list = [
    "#include \"table.h\"\n\n"
]

# -------------- VALUES to EXTRACT from JSON --------------
extract_keys = {
    "intake_valve_opens" : None,
    "exhaust_valve_opens" : None,
    "intake_valve_duration" : None,
    "exhaust_valve_duration" : None,
    "firing_order" : None,
    "number_of_cylinders" : None
}

with open(sys.argv[1], "r") as table_file:
	table_dict : dict = json.load(table_file)

for table_key in table_dict:
    if table_key in extract_keys:
        extract_keys[table_key] = table_dict[table_key]

# -------------- COMPUTING THE VALUES --------------------
for i in range(0, extract_keys["intake_valve_duration"]):
    current_offset = 0
    for j in range(0, extract_keys["number_of_cylinders"]):
        index_angle = (extract_keys["intake_valve_opens"] + i + current_offset) % 720
        bit = INTAKE_BITS[extract_keys["firing_order"][j] - 1]
        my_table[index_angle] = set_bit(my_table[index_angle], bit) # need to iterate through bits accordingly
        current_offset += offsets[extract_keys["number_of_cylinders"]]

for i in range(0, extract_keys["exhaust_valve_duration"]):
    current_offset = 0
    for j in range(0, extract_keys["number_of_cylinders"]):
        index_angle = (extract_keys["exhaust_valve_opens"] + i + current_offset) % 720
        bit = EXHAUST_BITS[extract_keys["firing_order"][j] - 1]
        my_table[index_angle] = set_bit(my_table[index_angle], bit)
        current_offset += offsets[extract_keys["number_of_cylinders"]]

bin_table = list(map(lambda val: bin(val), my_table))
print(bin_table)

# -------------- CREATING STRINGS FOR TABLE -----------
array_init = "static const unsigned int table[] = {\n"

for i, val in enumerate(my_table):
    array_init += str(val)
    if i % 10 == 0:
        array_init += ",\n"
    else:
        array_init += ", "

array_init = array_init[:-2]
array_init += "\n};\n"

table_init = [
    "\nValveTable test_table = {\n",
    "\ttable,\n",
    f"\t{TABLE_SIZE},\n",
    "\t"+ str(extract_keys["number_of_cylinders"]) + "\n",
    "};\n"
]
# -------------- WRITING THE table.cpp FILE --------------
with open(sys.argv[2], "w") as header_file:
    header_file.writelines(preprocess_list)
    header_file.write(array_init)
    header_file.writelines(table_init)
