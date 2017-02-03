% This code was written in order to test DataTurbine's upload and download
% capability on XBees, Arduinos, etc. It is called by testScript.m.
%
% Written by: Jasmine Cashbaugh
% Written on: August 19, 2011
%
% *************************************************************************

function dataOut = droneDataParser(dataIn)
    byte_array = dataIn.getArray;
    dataOut = char(java.lang.String(byte_array)); % Converts the byte
        % array to a string.
        
    % The following two lines are not needed to use dataParser. I kept them
    % in for debugging purposes as they display the data to the screen.
    disp('This is called and the data is: ');
    disp(dataOut);
    
    % Run the command.
    global connect2 % In the code for debugging. Will be removed in final
                   % version.
    eval(dataOut);
end