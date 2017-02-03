function sendcommand(controller,channelidx,data)
% SENDCOMMAND sends command data on the DataTurbine.
% 
%   SENDCOMMAND(CONTROLLER,CHANNELIDX,DATA) send the DATA on the CHANNELIDX
%   on the specified CONTROLLER. 
%
%   This function is provided as a convenience function and the
%   functionality can be acheived by explicitly calling the
%   'sendData' function of the controller Java object. The code
%   contained in this function can be duplicated in a script, function, or
%   the command prompt with the same results as this function. 
%
%   Author: Mike Rasay
%   Version 0.1.0
%   Matlab Version: 2009b
%   Created: 2012.05.23

controller.sendData(channelidx,data);