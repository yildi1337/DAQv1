% NoiseMeasurement 
%   Script for measuring the input voltage noise densities of the
%   measurement platform and displaying the results in the time and the
%   frequency domain.
%   
%   Date: 2021-01-02
%   Author: Phillip Durdaut

%% clear everything
instrreset;
clc;
clear all;
close all;

%% measurement parameters
MEASUREMENT_NAME = 'noise_inputs_open_supply_usb';

Tmeas = 60;
fs = 256e3;
Ts = 1/fs;
t = 0 : Ts : (Tmeas-Ts);

%% constants
USB_NI_VISA_DEVICE_ADDRESS = 'USB0::0x1337::0x2020::0000::RAW';
USB_NI_VISA_INPUT_BUFFER_SIZE_BYTES = 10240000;

SAMPLES_PER_SECOND = fs;
CHANNELS = 2;
BYTES_PER_SAMPLE_PER_CHANNEL = 4;
BYTES_PER_SAMPLE = CHANNELS * BYTES_PER_SAMPLE_PER_CHANNEL;
DURATION_PER_READ_IN_SECONDS = 25e-3;
SAMPLES_PER_READ = SAMPLES_PER_SECOND * DURATION_PER_READ_IN_SECONDS;
BYTES_PER_READ = SAMPLES_PER_READ * BYTES_PER_SAMPLE;

Nsamples = SAMPLES_PER_SECOND * Tmeas;
Nreads = Tmeas / DURATION_PER_READ_IN_SECONDS;
Nbytes = Nreads * BYTES_PER_READ;

%% connect to hardware
hvisa = visa('NI', USB_NI_VISA_DEVICE_ADDRESS);
hvisa.InputBufferSize = USB_NI_VISA_INPUT_BUFFER_SIZE_BYTES;
fopen(hvisa);
    
%% measurement   

% allocate buffer for received raw data
DATA_RAW_bytes = zeros(Nbytes, 1);

% start the measurement
fwrite(hvisa, 'START;', 'uint8');
pause(20e-3);

% get data
for i = 1 : Nreads
    fwrite(hvisa, 'GET;', 'uint8');

    idx_start = ((i-1)*BYTES_PER_READ)+1;
    idx_end = i*BYTES_PER_READ;

    DATA_RAW_bytes(idx_start:idx_end,1) = fread(hvisa, BYTES_PER_READ, 'uint8');
end

% stop the measurement
fwrite(hvisa, 'STOP;', 'uint8');

% reshape raw data
DATA_RAW_bytes = reshape(DATA_RAW_bytes, 4, Nbytes/4);

% allocate memory for signed integer samples
DATA_SAMPLES_int32_Ch0 = int32(zeros(1,Nsamples));
DATA_SAMPLES_int32_Ch1 = int32(zeros(1,Nsamples));
DATA_STATUS_Ch0 = int32(zeros(1,Nsamples));
DATA_STATUS_Ch1 = int32(zeros(1,Nsamples));

for s = 1 : Nsamples
    DATA_SAMPLES_int32_Ch0(s) = ADCBytesToSignedInteger(DATA_RAW_bytes(3,2*s-1), DATA_RAW_bytes(2,2*s-1), DATA_RAW_bytes(1,2*s-1));
    DATA_SAMPLES_int32_Ch1(s) = ADCBytesToSignedInteger(DATA_RAW_bytes(3,2*s), DATA_RAW_bytes(2,2*s), DATA_RAW_bytes(1,2*s));
end

% convert signed integer data into real voltages
DATA_SAMPLES_voltage_Ch0 = ADCSignedIntegerToVoltage(double(DATA_SAMPLES_int32_Ch0));
DATA_SAMPLES_voltage_Ch1 = ADCSignedIntegerToVoltage(double(DATA_SAMPLES_int32_Ch1));

% measured voltages in time domain without mean
V1 = DATA_SAMPLES_voltage_Ch0 - mean(DATA_SAMPLES_voltage_Ch0);
V2 = DATA_SAMPLES_voltage_Ch1 - mean(DATA_SAMPLES_voltage_Ch1);

% calculate amplitude spectra
[PSD1,f1] = pwelch(V1, hann(Nsamples), [], Nsamples, fs, 'psd');
[PSD2,f2] = pwelch(V2, hann(Nsamples), [], Nsamples, fs, 'psd');
LSD1 = sqrt(PSD1);
LSD2 = sqrt(PSD2);

% plot data
figure(1);

subplot(2,1,1);
plot(t, V1);
hold on;
plot(t, V2);
grid on;
hold off;
xlim([min(t) max(t)]);
xlabel('Time [s]');
ylabel('Amplitude [V]');
title('Time Domain');
drawnow;

subplot(2,1,2);
loglog(f1, LSD1);
hold on;
loglog(f2, LSD2);
grid on;
hold off;
xlim([1 100e3]);
ylim([1e-9 1e-3]);
xlabel('Frequency [Hz]');
ylabel('Amplitude Density [V/sqrt(Hz)]');
title('Amplitude Density Spectrum');
drawnow;
    
%% disconnect from hardware
fclose(hvisa);

%% save the time domain signals
save([ 'noise_measurements/' MEASUREMENT_NAME '.mat' ], 'V1', 'V2', 'fs');

