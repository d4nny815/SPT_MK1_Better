import sys

data_format = "time(msec),fuel_pressure(psi),chamber_pressure(psi),oxygen_pressure(psi),loadcell(lbs)\n"

def main():
    input_file = sys.argv[1].rstrip(".txt").split('_')
    type_of_run = input_file[0]
    run_number = input_file[1]
    date = input_file[2]
    output_file_name = "formatted_" + type_of_run + "_" + run_number + "_" + date + ".csv"
    
    out_file = open(output_file_name, "w")
    in_file = open(sys.argv[1], "r")
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