function start(controller)
% START Starts the controller.
%   START(CONTROLLER) starts the controller, which will forward data to any
%   functions registered to the controller. CONTROLLER is the
%   MatlabController object that should be started. This object must first
%   be created with the 'controller' function. 
% 
%   This function is provided as a utility function for maintaining
%   consistency in the references made to Java objects and Matlab specific
%   data types (e.g. structures). 
% 
%   Example: The following shows how to create the MatlabController object
%   that is to be started. In this example the registration of the function
%   to be called with the receipt of data. 
%   
%   conn = controller('localhost','3333','SCU')
%   ...
%   start(conn)
% 
%   See also CONTROLLER
% 
%   Author: Mike Rasay, Robotic Systems Laboratory (Santa Clara University)
%   Version: 20100108
% 
try 
    controller.startController;
catch exception
    if strcmp(exception.identifier,'MATLAB:Java:GenericException')
        error('Connection Error: Unable to locate DataTurbine');
    end
end