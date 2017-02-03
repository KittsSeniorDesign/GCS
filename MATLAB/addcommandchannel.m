function channelidx = addcommandchannel(controller,channelname)
% ADDCOMMANDCHANNEL adds a command channel to the controller. 
%
%  CHANNELIDX = ADDCOMMANDCHANNEL(CONTROLLER,CHANNELNAME) adds a command
%  channel to the specified CONTROLLER with the specified CHANNELNAME. The
%  CHANNELIDX is the index of the added channel and should be used as a
%  reference when sending data (commands) on the channel.
% 
%  This function is provided as a convenience function and the
%  functionality can be acheived by explicitly calling the
%  'addCommandChannel' function of the controller Java object. The code
%  contained in this function can be duplicated in a script, function, or
%  the command prompt with the same results as this function. 
% 
%   Author: Mike Rasay
%   Version 0.1.0
%   Matlab Version: 2009b
%   Created: 2012.05.23

channelidx = controller.addCommandChannel(channelname);