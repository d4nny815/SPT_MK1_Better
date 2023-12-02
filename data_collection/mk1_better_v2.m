clc; clear; close all


%% Diagram
input_file = '12.2.23_dataCollection_run1_formatted.csv'
[pathstr, name, ext] = fileparts(input_file);

data = readmatrix(input_file);
time = data(:, 1);
fuel_pressure = data(:, 2);
chamber_pressure = data(:, 3);
oxygen_pressure = data(:, 4);
ducer4 = data(:, 5);
ducer5 = data(:, 6);
ducer6 = data(:, 7);
ducer7 = data(:, 8);
ducer8 = data(:, 9);
thermo1 = data(:, 10);
thermo2 = data(:, 11);
thermo3 = data(:, 12);
thermo4 = data(:, 13);
thermo5 = data(:, 14);
thermo6 = data(:, 15);
thrust = data(:, 16);


allMetricsPlot = figure('name', "HOTFIRE", 'numbertitle', 'off', 'Units','normalized');
allMetricsPlot.WindowState = "maximized";

subplot(3,1,1);
plot(time, fuel_pressure, 'g', time, oxygen_pressure, 'b', time, chamber_pressure, 'r', ...
    time, ducer4, 'y', time, ducer5, '-', time, ducer6, '--', time, ducer7, '--', time, ducer8, '--');
title('Pressure Transducers');
ylabel('Pressure (psi)');
xlabel('Time (ms)');
legend({'Fuel Pressure', 'Oxygen Pressure', 'Chamber Pressure', 'Ducer 4', 'Ducer 5', 'Ducer 6', 'Ducer 7', 'Ducer 8'}, ...
    "Location","southeast", "Orientation","horizontal")
box on;
grid;

subplot(3,1,2)
plot(time, thermo1,'r', time, thermo2, 'b', time, thermo3, 'g', ... 
     time, thermo4,'r', time, thermo5, 'b', time, thermo6, 'g');
title('Thermoresistors');
ylabel('Heat (F)');
xlabel('Time (ms)');
legend({'Thermoresistor 1', 'Thermoresistor 2', 'Thermoresistor 3', 'Thermoresistor 4', 'Thermoresistor 5', 'Thermoresistor 6'}, ...
    "Location","southeast", "Orientation","horizontal")
grid;

subplot(3,1,3)
plot(time, thrust, 'r');
title('Thrust');
ylabel('Thrust (lb)');
xlabel('Time (ms)');
grid;

print('-dpng',strcat(name, '.png'));

ducerPlot = figure('name', "Transducers", 'numbertitle', 'off', 'Units','normalized');
ducerPlot.WindowState = "maximized";
plot(time, fuel_pressure, 'g', time, oxygen_pressure, 'b', time, chamber_pressure, 'r', ...
    time, ducer4, 'y', time, ducer5, '-', time, ducer6, '--', time, ducer7, '--', time, ducer8, '--');
title('Pressure Transducers');
ylabel('Pressure (psi)');
xlabel('Time (ms)');
legend({'Fuel Pressure', 'Oxygen Pressure', 'Chamber Pressure', 'Ducer 4', 'Ducer 5', 'Ducer 6', 'Ducer 7', 'Ducer 8'}, ...
    "Location","southeast", "Orientation","horizontal")
box on;
grid;

thermoresistorPlot = figure('name', "Thermistors", 'numbertitle', 'off', 'Units','normalized');
thermoresistorPlot.WindowState = "maximized";
plot(time, thermo1,'r', time, thermo2, 'b', time, thermo3, 'g', ... 
     time, thermo4,'r', time, thermo5, 'b', time, thermo6, 'g');
title('Thermoresistors');
ylabel('Heat (F)');
xlabel('Time (ms)');
legend({'Thermoresistor 1', 'Thermoresistor 2', 'Thermoresistor 3', 'Thermoresistor 4', 'Thermoresistor 5', 'Thermoresistor 6'}, ...
    "Location","southeast", "Orientation","horizontal")
grid;

thrustPlot = figure('name', "Thrust", 'numbertitle', 'off', 'Units','normalized');
thrustPlot.WindowState = "maximized";
plot(time, thrust, 'r');
title('Thrust');
ylabel('Thrust (lb)');
xlabel('Time (ms)');
grid;

clear ext input_file name pathstr
