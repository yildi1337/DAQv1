function signedInteger = ADCBytesToSignedInteger(msb, csb, lsb)
% ADCBytesToSignedInteger 
%   Function for converting the three bytes from the ADC (AD7768) into a
%   signed integer (int32) value.
%   
%   Date: 2021-01-02
%   Author: Phillip Durdaut

% combine the three bytes to a 24-bit (temporary) number
combined = (bitshift(msb,16)) + (bitshift(csb,8)) + bitshift(lsb,0);

% check the most significant bit
if (bitget(msb,8))
    
    % negative number
    signedInteger = int32(typecast(uint32(bitor(bitshift(255,24),combined)), 'int32'));
    
else
    
    % positive number    
    signedInteger = int32(typecast(uint32(bitor(bitshift(0,24),combined)), 'int32'));
    
end


