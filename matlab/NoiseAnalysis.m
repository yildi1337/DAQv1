% NoiseAnalysis
%   Script for analyzing the measured noise signals in the frequency 
%   domain.
%   
%   Date: 2021-01-03
%   Author: Phillip Durdaut

%% clear everything
clc;
clear all;
close all;

%% analysis parameters
MEASUREMENT_NAMES = { ...
    'noise_inputs_open_supply_usb', ...
    'noise_inputs_open_supply_mains', ...
    'noise_inputs_50ohm_supply_usb', ...
    'noise_inputs_50ohm_supply_mains', ...
    'noise_inputs_short_supply_usb', ...
    'noise_inputs_short_supply_mains', ...
};

LEGEND_NAMES = { ...
    'Open / USB', ...
    'Open / Mains', ...
    '50 Ohm / USB', ...
    '50 Ohm / Mains', ...
    'Short / USB', ...
    'Short / Mains', ...
};

N = length(MEASUREMENT_NAMES);
cmap = parula(N+1);

fs = 256e3;
NFFT = fs;
mywindow = hann(NFFT);

%% load noise signals
for n = 1 : N   

    data{n} = load([ 'noise_measurements/' MEASUREMENT_NAMES{n} '.mat' ], 'V1', 'V2', 'fs'); 

end

%% calculate amplitude spectral densities
for n = 1 : N  

    [PSD1{n},f1{n}] = pwelch(data{n}.V1, mywindow, [], NFFT, fs, 'psd');
    [PSD2{n},f2{n}] = pwelch(data{n}.V2, mywindow, [], NFFT, fs, 'psd');
    
    LSD1{n} = sqrt(PSD1{n});
    LSD2{n} = sqrt(PSD2{n});

end

%% plot
figure;

for n = 1 : N 
    
    subplot(1,2,1);
    loglog(f1{n}, LSD1{n}, 'Color', cmap(n,:), 'DisplayName', LEGEND_NAMES{n}, 'LineWidth', 1.5);
    hold on;
    
    subplot(1,2,2);
    loglog(f2{n}, LSD2{n}, 'Color', cmap(n,:), 'DisplayName', LEGEND_NAMES{n}, 'LineWidth', 1.5);
    hold on;
    
end

subplot(1,2,1);
grid on;
hold off;
xlim([1 100e3]);
ylim([1e-8 1e-5]);
xlabel('Frequency [Hz]');
ylabel('Amplitude Density [V/sqrt(Hz)]');
title('Channel 0');
legend('show', 'Location', 'SouthWest');

subplot(1,2,2);
grid on;
hold off;
xlim([1 100e3]);
ylim([1e-8 1e-5]);
xlabel('Frequency [Hz]');
ylabel('Amplitude Density [V/sqrt(Hz)]');
title('Channel 1');
legend('show', 'Location', 'SouthWest');
