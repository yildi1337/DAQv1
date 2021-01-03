function voltage = ADCSignedIntegerToVoltage(signedInteger)
% ADCSignedIntegerToVoltage 
%   Function for converting signed integer (int32) values into voltage
%   values (AD7768).
%   
%   Date: 2021-01-02
%   Author: Phillip Durdaut

% constants
V_REF = 4.096;
GAIN = hex2dec('555555');
OFFSET = 0;
USER_CALIBRATION_FACTOR = 1.034;

% see datasheet of AD7768 on page 64
voltage = V_REF./(3.*2^(21)) .* (signedInteger .* 4./GAIN .* 2^(42)./4194300 + OFFSET) .* USER_CALIBRATION_FACTOR;

end
