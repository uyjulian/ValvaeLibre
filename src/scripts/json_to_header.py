import json
import sys

def set_bit(number : int, bit : int) -> int:
    return number | (1<<bit)

def clear_bit(number : int, bit : int) -> int:
    return number & ~(1<<bit)

# -------------- BITS FOR VALVES --------------
INTAKE_1 = 31
INTAKE_2 = 30
INTAKE_3 = 29
INTAKE_4 = 28

EXHAUST_1 = 15
EXHAUST_2 = 14
EXHAUST_3 = 13
EXHAUST_4 = 12

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

# -------------- COMPUTING THE VALUES --------------------
for table_key in table_dict:
    if table_key in extract_keys:
        extract_keys[table_key] = table_dict[table_key]

for i in range(0, extract_keys["intake_valve_duration"]):
    index_1 = (extract_keys["intake_valve_opens"] + i) % 720
    index_3 = (extract_keys["intake_valve_opens"] + i + 180) % 720
    index_4 = (extract_keys["intake_valve_opens"] + i + 360) % 720
    index_2 = (extract_keys["intake_valve_opens"] + i + 540) % 720

    my_table[index_1] = set_bit(my_table[index_1], INTAKE_1)
    my_table[index_3] = set_bit(my_table[index_3], INTAKE_3)
    my_table[index_4] = set_bit(my_table[index_4], INTAKE_4)
    my_table[index_2] = set_bit(my_table[index_2], INTAKE_2)

for i in range(0, extract_keys["exhaust_valve_duration"]):
    index_1 = (extract_keys["exhaust_valve_opens"] + i) % 720
    index_3 = (extract_keys["exhaust_valve_opens"] + i + 180) % 720
    index_4 = (extract_keys["exhaust_valve_opens"] + i + 360) % 720
    index_2 = (extract_keys["exhaust_valve_opens"] + i + 540) % 720

    my_table[index_1] = set_bit(my_table[index_1], EXHAUST_1)
    my_table[index_3] = set_bit(my_table[index_3], EXHAUST_3)
    my_table[index_4] = set_bit(my_table[index_4], EXHAUST_4)
    my_table[index_2] = set_bit(my_table[index_2], EXHAUST_2)

# bin_table = list(map(lambda val: bin(val), my_table))
# print(bin_table)

# -------------- CREATING STRINGS FOR CPP FILE -----------
array_init = "static const unsigned int table[] = {\n"
for i, val in enumerate(my_table):
    array_init += str(val)
    if i % 10 == 0:
        array_init += ",\n"
    else:
        array_init += ", "
array_init = array_init[:-1]
array_init += "};\n"

table_init = [
    "\nValveTable testTable = {\n",
    "\ttable,\n",
    "\t720,\n",
    "\t"+ str(extract_keys["number_of_cylinders"]) + "\n",
    "};\n"
]
# -------------- WRITING THE table.cpp FILE --------------
with open(sys.argv[2], "w") as header_file:
    header_file.writelines(preprocess_list)
    header_file.write(array_init)
    header_file.writelines(table_init)
