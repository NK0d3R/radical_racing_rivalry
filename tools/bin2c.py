import argparse
import sys
import os

def main():

    parser = argparse.ArgumentParser(description="Binary to C declaration converter")
    parser.add_argument("-i", "--input", help="Input file to process", required=True)
    parser.add_argument("-o", "--output", help="Output C file (no ext)", required=True)
    parser.add_argument("-m", "--maxvals", help="Max number of values per line", 
                        type=int, default=16, required=False)
    options = parser.parse_args()
    
    buffer_size = 2048
    file_size = 0
    
    try:
        file_size = os.path.getsize(options.input)
    except OSError:
        print("Could not stat file")
        sys.exit(-1)
      
    filename = os.path.basename(options.output)
    basename = os.path.splitext(filename)[0]
    
    variable_name_base = basename.upper().replace(" ", "_").replace(".","_")
    variable_name_size = variable_name_base + "_SIZE"
    variable_name_data = variable_name_base + "_DATA"
    
    with open(options.input, "rb") as input:
        with open(options.output, "wt") as output:
            output.write("#ifndef __%s_H__\n#define __%s_H__\n\n" %
                         (variable_name_base, variable_name_base))
            output.write("#define %s\t%d\n" % (
                         variable_name_size, file_size))
                         
            output.write("PROGMEM const uint8_t %s[] = {\n\t" %
                         variable_name_data)
            
            crt_line_byte = 0
            new_line_finish = True
    
            while True:
                chunk = input.read(buffer_size)
            
                if chunk:
                    new_line_finish = False
                    for byte in chunk:
                        output.write("%s,"%
                                     format(ord(byte), "#04x"))
                        crt_line_byte += 1
                        if crt_line_byte == options.maxvals:
                            output.write("\n\t")
                            crt_line_byte = 0
                            new_line_finish = True
                else:
                    break
                
                if new_line_finish is False:
                    output.write("\n")
                output.write("};\n\n#endif //__%s_H__" % (variable_name_base))
                

if __name__ == "__main__":
    main()