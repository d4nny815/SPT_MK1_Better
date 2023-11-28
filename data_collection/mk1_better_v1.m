clc; clear; close all


%% Diagram
input_file = '11.16_hotfire_run5_formatted.csv'
[pathstr, name, ext] = fileparts(input_file);

data = readmatrix(input_file);
time = data(:, 1);
fuel_pressure = data(:, 2);
chamber_pressure = data(:, 3);
oxygen_pressure = data(:, 4);
loadcell = data(:, 5);


plot2 = figure('name', "HOTFIRE", 'numbertitle', 'off');

plot(time, fuel_pressure, 'g', time, oxygen_pressure, 'b', time, chamber_pressure, 'r', time, loadcell, 'y');
legend({'Fuel Pressure', 'Oxygen Pressure', 'Chamber Pressure', 'Thrust'})


photo_name = strcat(name, '.png');
print('-dpng',photo_name);

clear ext input_file pathstr name photo_name

