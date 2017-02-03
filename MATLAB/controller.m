function connect = controller(dataturbineip,dataturbineport,controllername)
% CONTROLLER Connect data handling controller to a ground station DataTurbine.
%   CONNECT = CONTROLLER(CONTROLLERNAME,DATATURBINEIP,DATATURBINEPORT) returns
%   a MatlabController object, which is responsible for the forwarding of
%   data to functions defined and registered to the object. CONTROLLERNAME
%   is the name that identifies the application that is sending data. 
%   DATATURBINEIP is the specific IP address of the DataTurbine instance 
%   to be connected to, which may be 'localhost.' DATATURBINEPORT is the 
%   port used to establish communication with the DataTurbine server.
%   CONNECT is a Java MatlabController object. 
% 
%   Use of this method requires that the jmatlab.jar file be included on
%   the Matlab classpath. 
% 
%   Author: Mike Rasay
%   Version: 0.1.1
%   Matlab Version: 2009a
%

% handling input arguments - In the case that this function is called with
% two arguments, it is assumed that it will be a listen only entity and
% that a command channel should not be created. 
% If no arguments are specified the default is to establish 2-way
% communication with the Source being 'matlab'.
if nargin == 0
    controllername = 'matlab';
    dataturbineip = 'localhost';
    dataturbineport = '3333';
elseif nargin < 2
    error('Invalid parameter count');
end

try 
    connect = edu.scu.engr.rsl.matlab.MatlabController(dataturbineip,...
        dataturbineport,controllername);
catch exception
    % notify the caller that the jar cannot be located
    if strcmp(exception.identifier,'MATLAB:undefinedVarOrClass')
        error('JMatlab Jar Error: Insure that the classpath includes jmatlab.jar');
    end
end