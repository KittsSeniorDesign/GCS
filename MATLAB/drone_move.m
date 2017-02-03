function drone_move(u)

global connect2;

% DRONE_MOVE moves the drone using the DataTurbine.
% 
%   DRONE_MOVE(u) sends the move command on the specified
%   CONTROLLER. 
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

connect2.droneMove(u(1), u(2), u(3), u(4));
