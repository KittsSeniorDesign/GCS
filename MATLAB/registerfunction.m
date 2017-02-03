function funct = registerfunction(controller,functionname,channelname)
% REGISTERFUNCTION Associates a callback function with a specific channel
% name. 
%   FUNCT = REGISTERFUNCTION(CONTROLLER,FUNCTIONNAME,CHANNELNAME) returns
%   the Java Function object that is created within this function.
%   CONTROLLER is the Java MatlabController object that is created and
%   returned by the 'controller' function. FUNCTIONNAME is the name of the
%   callback funtion that is to be executed when data is received on the
%   specified CHANNELNAME. This function must be called prior to the call
%   to 'start' or the communication and dispatching of the data receive
%   event will not occur. 
%   
%   The specification of the CHANNELNAME must follow the naming convention
%   prescribed by the DataTurbine where the source and channel name be
%   specified (/source_name/channel_name). If the data source creates a
%   connection to the DataTurbine with the name 'RemoteNode' and a 'data'
%   channel, the CHANNELNAME should be specified as '/RemoteNode/data.'
% 
%   In the case that the callback function that is to be excuted requires
%   more than the data from the DataTurbine, this function will not provide
%   the appropriate means to registering the callback function. 
% 
%   Author: Mike Rasay
%   Version 0.1.0
%   Matlab Version: 2009a
%   Created: 2010.01.19


 % the addSubscriberFunction method requires that the Function parameter be
 % an array
 funct(1) = edu.scu.engr.rsl.util.Function(functionname);
%  the data that is forwarded by the controller
 funct(1).addArg(edu.scu.engr.rsl.util.ByteArray);
% add the function to the controller
controller.addSubscriberFunction(channelname,funct);
