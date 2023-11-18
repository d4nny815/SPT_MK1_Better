clc; clear; close all


%% Diagram
input_file = '11.17_hotfire_run1_formatted.csv'
[pathstr, name, ext] = fileparts(input_file);

data = readmatrix(input_file);
time = data(:, 1);
fuel_pressure = data(:, 2);
chamber_pressure = data(:, 3);
oxygen_pressure = data(:, 4);
loadcell = data(:, 5);


plot2 = figure('name', "HOTFIRE", 'numbertitle', 'off');

subplot(3,1,1);
plot(time, fuel_pressure, 'g', time, oxygen_pressure, 'b');
title('Pressure Transducers');
ylabel('Pressure (psi)');
xlabel('Time (ms)');
legend({'Fuel Pressure', 'Oxygen Pressure'})
grid;

subplot(3,1,2)
plot(time, chamber_pressure,'r');
title('Thermoresistors');
ylabel('Heat (F)');
xlabel('Time (ms)');
grid;

subplot(3,1,3)
plot(time, loadcell, 'y');
title('Thrust');
ylabel('Thrust (lb)');
xlabel('Time (ms)');
grid;

input_file = extractBefore(input_file, '.');
photo_name = strcat(name, '.png');
print('-dpng',photo_name);
clear data fuel_pressure oxygen_pressure chamber_pressure loadcell

