function drone_land()

global connect2;

% DRONE_LAND causes the drone to land using the DataTurbine.
% 
%
%   This function is provided as a convenience function and the
%   functionality can be acheived by explicitly calling the
%   'droneMove' function of the controller Java object. The code
%   contained in this function can be duplicated in a script, function, or
%   the command prompt with the same results as this function. 
%
%   Author: Jasmine Cashbaugh
%   Version 0.1.0
%   Matlab Version: 2008b
%   Created: 2012.09.05

connect2.droneLand();