function source = addsourcefunction(controller, channelname, data)
% ADDSOURCEFUNCTION Associates a source with a specific channel name. 
%   source = ADDSOURCEFUNCTION(CHANNELNAME, DATA) returns a 1 if the source
%   is successfully created and a 0 otherwise.
%   CONTROLLER is the Java MatlabController object that is created and
%   returned by the 'controller' function. CHANNELNAME is the same
%   CHANNELNAME that the subscriber function will subscribe to.
%   This function must be called after to the call to 'start' or the
%   communication and dispatching of the data receive event will not occur.
%   This function should only be called once, when initially setting up the
%   source.
%   
%   The specification of the CHANNELNAME must follow the naming convention
%   prescribed by the DataTurbine where the channel name is specified
%   (channel_name). If the data subscriber reads on a connection to the
%   DataTurbine with the name 'RemoteNode' and a 'data' channel, the
%   CHANNELNAME should be specified as 'data.'
% 
%   Author: Jasmine Cashbaugh
%   Version 0.1.0
%   Matlab Version: 2009a
%   Created: 2011.10.27

% add the source to the controller
source = controller.addSourceFunction(channelname,data);