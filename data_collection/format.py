import sys

data_format = "time(msec),fuel_pressure(psi),chamber_pressure(psi),\
                oxygen_pressure(psi),loadcell(lbs)\n"

def main():
    if (len(sys.argv) != 2):
        print("Usage: python format.py <input_file>")
        return
    input_file = sys.argv[1].rstrip(".txt").split('_')
    date = input_file[0]
    type_of_run = input_file[1]
    run_number = input_file[2]
    output_file_name = date + "_" + type_of_run + "_" + run_number + "_formatted" + ".csv"
    
    out_file = open(output_file_name, "w")
    try:
        in_file = open(sys.argv[1], "r")
    except:
        print("Error: File not found OR invalid file format: <date>_<type_of_run>_<run_number>.txt")
        return
    start_time = int(in_file.readline().split(",")[0])
    out_file.write(data_format)


    while True:
        line = in_file.readline().split(",")
        try:
            time = int(line[0])
        except:
            break
        line[0] = time - start_time
        out_file.write(','.join([str(i) for i in line]))
    in_file.close()
    out_file.close()
    return

if __name__ == "__main__":
    main()