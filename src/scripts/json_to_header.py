import json
import sys
# -------------- BITS FOR VALVES ---------------------------
INTAKE_BITS = (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)
EXHAUST_BITS = (16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)

# -------------- VALVES OFFSETS FOR DIFFERENT ENGINES ----------
offsets = {
    4: 180,
    6: 120,
    8: 90
}

# -------------- SETTING BITS FUNCTION -----------------------
def set_bit(number : int, bit : int) -> int:
    return number | (1<<bit)

# -------------- TABLE -----------------------------------
TABLE_SIZE = 720
my_table = [0b00000000000000000000000000000000] * TABLE_SIZE

# -------------- READING JSON VALUES---------------------
def read_json_keys(extract_keys: dict):
    with open(sys.argv[1], "r") as table_file:
        table_dict : dict = json.load(table_file)

    for table_key in table_dict:
        if table_key in extract_keys:
            extract_keys[table_key] = table_dict[table_key]

# -------------- COMPUTING THE VALUES --------------------
def computing_values(my_table: list, extract_keys: dict):
    current_offset = 0
    for cylinder in extract_keys["firing_order"]:
        bit  = INTAKE_BITS[cylinder - 1]
        for angle in range(0, extract_keys["intake_valve_duration"]):
            angle_index = (extract_keys["intake_valve_opens"] + angle + current_offset) % 720
            my_table[angle_index] = set_bit(my_table[angle_index], bit)
        current_offset += offsets[extract_keys["number_of_cylinders"]]

    current_offset = 0
    for cylinder in extract_keys["firing_order"]:
        bit  = EXHAUST_BITS[cylinder - 1]
        for angle in range(0, extract_keys["exhaust_valve_duration"]):
            angle_index = (extract_keys["exhaust_valve_opens"] + angle + current_offset) % 720
            my_table[angle_index] = set_bit(my_table[angle_index], bit)
        current_offset += offsets[extract_keys["number_of_cylinders"]]

# -------------- CREATING STRINGS FOR TABLE -----------
def create_table(my_table:list) -> str:
    array_init = "static const unsigned int table[] = {\n"
    for i, val in enumerate(my_table):
        array_init += str(val)
        if i % 10 == 0:
            array_init += ",\n"
        else:
            array_init += ", "

    array_init = array_init[:-2]
    array_init += "\n};\n"
    return array_init

# -------------- WRITING THE table.cpp FILE --------------
def writting_strings(preprocess_list: list, array_init: str, table_init: str):
    with open(sys.argv[2], "w") as header_file:
        header_file.writelines(preprocess_list)
        header_file.write(array_init)
        header_file.writelines(table_init)

# -------------- FUNCTION FOR PRINTING TABLES -----------
def print_table(my_table: list):
    bin_table = list(map(lambda val: bin(val), my_table))
    print(bin_table)

if __name__ == "__main__":
    extract_keys = {
        "intake_valve_opens" : None,
        "exhaust_valve_opens" : None,
        "intake_valve_duration" : None,
        "exhaust_valve_duration" : None,
        "firing_order" : None,
        "number_of_cylinders" : None
    }

    preprocess_list = [
        "#include \"table.h\"\n\n"
    ]

    read_json_keys(extract_keys)

    table_init = [
        "\nValveTable test_table = {\n",
        "\ttable,\n",
        f"\t{TABLE_SIZE},\n",
        "\t"+ str(extract_keys["number_of_cylinders"]) + "\n",
        "};\n"
    ]

    computing_values(my_table, extract_keys)
    array_init = create_table(my_table)
    writting_strings(preprocess_list, array_init, table_init)
    print_table(my_table)
