import argparse
import sys
import string
import os

def main():
    parser = argparse.ArgumentParser(
                description="Generate source file for a font mapping")
    parser.add_argument("-i", "--input", action="store",
                        required=True,
                        help="File containing font mapping definition")
    parser.add_argument("-o", "--output", action="store",
                        required=True, help="Output file")
    parse_data = parser.parse_args()
    data_lines = None
    with open(parse_data.input, 'r') as input:
        data_lines = input.readlines()
    if data_lines is None or len(data_lines) == 0:
        print("Could not read input file !")
        sys.exit(255)
    default_mapping = -1
    max_char = -1
    mapping = {}

    for index, line in enumerate(data_lines):
        elements = line.split()
        if len(elements) < 2:
            print("Skipping line %d" % (index))
            continue
        if elements[0] == "default":
            default_mapping = int(elements[-1])
        else:
            used_frame = int(elements[-1])
            for char in elements[:-1]:
                if len(char) == 1:
                    chr_value = ord(char)
                    if chr_value > max_char:
                        max_char = chr_value
                    mapping[chr_value] = used_frame
                else:
                    print("Invalid char sequence at line %d" % (index))

    filename = os.path.basename(parse_data.output)
    basename = os.path.splitext(filename)[0]
    variable_name_base = basename.upper().replace(" ", "_").replace(".","_")

    with open(parse_data.output, 'w') as output:
        output.write("#ifndef __%s_H__\n#define __%s_H__\n\n" %
                     (variable_name_base, variable_name_base))
        output.write("uint8_t nb_map_elems = %d;\n" % (max_char + 1))
        output.write("uint8_t default_frame = %d;\n" % (default_mapping))
        output.write("PROGMEM uint8_t const mapping[] = {\n")
        for char in range(0, 128):
            stop = False
            outstr = ""
            if char in mapping:
                if char == max_char:
                    outstr = " %d" % (mapping[char])
                    stop = True
                else:
                    outstr = " %d," % (mapping[char])
            else:
                outstr = " %d," % (default_mapping)
            crt_char = chr(char)
            if crt_char.isspace() is False and crt_char in string.printable:
                outstr += "    /* '%c\' */" % (crt_char)
            outstr += "\n"
            output.write(outstr)
            if stop:
                break
        output.write("};\n\n#endif //__%s_H__\n" % (variable_name_base))
if __name__=="__main__":
    main()