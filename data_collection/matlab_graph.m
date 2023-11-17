clc; clear; close all


%% Ckt 1

data = readmatrix('formatted_hotfire_run2_11.16.txt');
time = data(:, 1);
fuel_pressure = data(:, 2);
chamber_pressure = data(:, 3);
oxygen_pressure = data(:, 4);
loadcell = data(:, 5)

plot2 = figure('name', "random", 'numbertitle', 'off');
plot(time, fuel_pressure, 'g', time, chamber_pressure,'r');
title('HOT fire');
ylabel('y axis');
xlabel('x axis');
%legend({'Vin', 'Vout'})
grid


%savefig(plot, 'CE_Amp_ckt.fig');
print('-dpng','hotfire_run2.png');
clear data fuel_pressure oxygen_pressure chamber_pressure loadcell


