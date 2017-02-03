function stop(controller)
% STOP Disconnects the controller from the DataTurbine.
%   STOP(CONTROLLER) stops the CONTROLLER from receiving data from the
%   DataTurbine and forwarding information to the registered functions.
%   CONTROLLER is the MatlabController object that is to be stopped.
% 
%   This function is provided as a utility function for maintaining
%   consistency in the references made to Java objects and Matlab specific
%   data types (e.g. structures). Rather than requiring a using function or
%   script to use the dot-operator in the Java methodology, which is
%   somewhat confusing since Matlab uses the dot-operator for structures,
%   the function call explicitly handles the java method call. 
% 
%   The MatlabController object uses the Matlab JRE to establish a thread
%   that does not require the explicit use of the Matlab resoruces to
%   receive data from the remote ground station. As such, this thread
%   requires proper disposal, which is provided by this function. 
% 
%   Example: 
%   conn = %   conn = controller('localhost','3333','SCU')
%   ...
%   start(conn)
%   ...
%   stop(conn)
% 
%   See also CONTROLLER
% 
%   Author: Mike Rasay, Robotic Systems Laboratory (Santa Clara University)
%   Version: 20100108
% 
controller.stopController;