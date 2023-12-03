import sys
import os
import shutil
import pandas as pd
import matplotlib.pyplot as plt

def makeCSV(input_file_name):
    input_file = input_file_name.rstrip(".txt").split('_')
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
    data_format = "time_ms,fuel_pressure_psi,chamber_pressure_psi,oxygen_pressure_psi,ducer4_psi,ducer5_psi,ducer6_psi,ducer7_psi,ducer8_psi,thermistor1_C,thermistor2_C,thermistor3_C,thermistor4_C,thermistor5_C,thermistor6_C,loadcell_lbs\n"
    out_file.write(data_format)

    while True:
        line = in_file.readline().split(",")
        try:
            time = int(line[0])
            # gets rid of random outliers from weird transmissions
            if int(line[1]) < 0: continue
            if int(line[1]) > 1000: continue
            if int(line[2]) > 1000: continue
            if int(line[3]) > 1000: continue
            if int(line[4]) > 1000: continue
            if int(line[5]) > 1000: continue
            if int(line[6]) > 1000: continue
            if int(line[7]) > 1000: continue
            if int(line[8]) > 1000: continue
            if int(line[9]) > 1000: continue
            if int(line[10]) > 1000: continue
            if int(line[11]) > 1000: continue
            if int(line[12]) > 1000: continue
            if int(line[13]) > 1000: continue
            if int(line[14]) > 1000: continue
            if int(line[15]) > 1000: continue
        except:
            break
        line[0] = time - start_time
        out_file.write(','.join([str(i) for i in line]))
    in_file.close()
    out_file.close()
    return output_file_name


def makeGraph(input_file):
    plt.rcParams["figure.figsize"] = [20.50, 10.50]
    plt.rcParams["figure.autolayout"] = True
    plt.rcParams["legend.loc"] = 'lower right'

    headers = ['time_ms','fuel_pressure_psi','chamber_pressure_psi','oxygen_pressure_psi','ducer4_psi','ducer5_psi','ducer6_psi','ducer7_psi','ducer8_psi','thermistor1_C','thermistor2_C','thermistor3_C','thermistor4_C','thermistor5_C','thermistor6_C','loadcell_lbs']

    pressure_names = ["Fuel Pressure", "Chamber Pressure", "Oxygen Pressure", "Ducer 4", "Ducer 5", "Ducer 6", "Ducer7", "Ducer 8"]
    thermistor_names = ['thermistor1', 'thermistor2', 'thermistor3', 'thermistor4', 'thermistor5', 'thermistor6']
    thrust_name = ['thrust']

    # main graph
    df = pd.read_csv(input_file, usecols=headers)
    plt.figure("All Graphs")
    plt.subplot(311)
    plt.ylabel("Pressure (psi)")
    plt.xlabel("Time(ms)")
    
    plt.plot(df.time_ms, df.fuel_pressure_psi, df.time_ms, df.chamber_pressure_psi,  df.time_ms, df.oxygen_pressure_psi, \
        df.time_ms, df.ducer4_psi, df.time_ms, df.ducer5_psi, df.time_ms, df.ducer6_psi, df.time_ms, df.ducer7_psi, df.time_ms, df.ducer8_psi)
    plt.legend(pressure_names)
    plt.grid()
    
    plt.subplot(312)
    plt.ylabel("Temperature (C)")
    plt.xlabel("Time(ms)")
    plt.plot(df.time_ms, df.thermistor1_C, df.time_ms, df.thermistor2_C, df.time_ms, df.thermistor3_C, df.time_ms, df.thermistor4_C, df.time_ms,\
        df.thermistor5_C, df.time_ms, df.thermistor6_C)
    plt.legend(thermistor_names)
    plt.grid()
    
    
    plt.subplot(313)
    plt.ylabel("Thrust (lbs)")
    plt.xlabel("Time(ms)")
    plt.plot(df.time_ms, df.loadcell_lbs)
    plt.legend(thrust_name)
    plt.grid()
   
    plt.savefig(f"{input_file.rstrip(".csv")}.png")
    
    plt.figure("Pressure Graph")
    plt.ylabel("Pressure (psi)")
    plt.xlabel("Time(ms)")
    plt.plot(df.time_ms, df.fuel_pressure_psi, df.time_ms, df.chamber_pressure_psi,  df.time_ms, df.oxygen_pressure_psi, \
        df.time_ms, df.ducer4_psi, df.time_ms, df.ducer5_psi, df.time_ms, df.ducer6_psi, df.time_ms, df.ducer7_psi, df.time_ms, df.ducer8_psi)
    plt.legend(pressure_names)
    plt.grid()
    plt.savefig(f"{input_file.rstrip(".csv")}_pressure.png")
    
    plt.figure("Temps Graph")
    plt.ylabel("Temperature (C)")
    plt.xlabel("Time(ms)")
    plt.plot(df.time_ms, df.thermistor1_C, df.time_ms, df.thermistor2_C, df.time_ms, df.thermistor3_C, df.time_ms, df.thermistor4_C, df.time_ms,\
        df.thermistor5_C, df.time_ms, df.thermistor6_C)
    plt.legend(thermistor_names)
    plt.savefig(f"{input_file.rstrip(".csv")}_temps.png")
    plt.grid()
    
    plt.figure("Thrust Graph")
    plt.ylabel("Thrust (lbs)")
    plt.xlabel("Time(ms)")
    plt.plot(df.time_ms, df.loadcell_lbs, 'b')
    plt.legend(thrust_name)
    plt.savefig(f"{input_file.rstrip(".csv")}_thrust.png")
    plt.grid()
    
    plt.show()

def main():
    if (len(sys.argv) != 2):
        print("Usage: python format.py <input_file>")
        return
    
    input_file_name = sys.argv[1]
    dir_name = input_file_name.rstrip(".txt")
    try:
        os.mkdir(dir_name)
    except:
        print("dir exist")
    shutil.copy2(input_file_name, dir_name)
    # os.remove(input_file_name)
    os.chdir(dir_name)
    output_file_name = makeCSV(input_file_name)
    makeGraph(output_file_name)


if __name__ == "__main__":
    main()