% This code was written in order to test DataTurbine's upload and download
% capability on XBees, Arduinos, etc. It is called by testScript.m.
%
% Written by: Jasmine Cashbaugh
% Written on: August 19, 2011
%
% *************************************************************************

function dataOut = droneDataParserLite(dataIn)
    byte_array = dataIn.getArray;
    dataOut = char(java.lang.String(byte_array)); % Converts the byte
        % array to a string.
end