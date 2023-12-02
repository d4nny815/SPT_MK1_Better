clc; clear; close all


%% Diagram
input_file = '12.1.23_dataCollection_run1_formatted.csv'
[pathstr, name, ext] = fileparts(input_file);

data = readmatrix(input_file);
time = data(:, 1);
fuel_pressure = data(:, 2);
chamber_pressure = data(:, 3);
oxygen_pressure = data(:, 4);
ducer4 = data(:, 5);
ducer5 = data(:, 6);
ducer6 = data(:, 7);
thermo1 = data(:, 8);
thermo2 = data(:, 9);
thermo3 = data(:, 10);
thrust = data(:, 11);


plot2 = figure('name', "HOTFIRE", 'numbertitle', 'off', 'Units','normalized');
plot2.WindowState = "maximized";

subplot(3,1,1);
plot(time, fuel_pressure, 'g', time, oxygen_pressure, 'b', time, chamber_pressure, 'r', ...
    time, ducer4, 'y', time, ducer5, '-', time, ducer6, '--');
title('Pressure Transducers');
ylabel('Pressure (psi)');
xlabel('Time (ms)');
legend({'Fuel Pressure', 'Oxygen Pressure', 'Chamber Pressure', 'Ducer 4', 'Ducer 5', 'Ducer 6'}, ...
    "Location","southeast", "Orientation","horizontal")
box on;
grid;

subplot(3,1,2)
plot(time, thermo1,'r', time, thermo2, 'b', time, thermo3, 'g');
title('Thermoresistors');
ylabel('Heat (F)');
xlabel('Time (ms)');
legend({'Thermoresistor 1', 'Thermoresistor 2', 'Thermoresistor 3'}, ...
    "Location","southeast", "Orientation","horizontal")
grid;

subplot(3,1,3)
plot(time, thrust, 'y');
title('Thrust');
ylabel('Thrust (lb)');
xlabel('Time (ms)');
grid;

print('-dpng',strcat(name, '.png'));

clear ext input_file name pathstr
