% This script is written to test the functionality of Matlab functions to
% call java functions.
%
% Created by Alan Nguyen
% Feburary 20, 2017

% Creating a global variable called connect
global connect4;

% Create a Matlab controller.
connect4 = controller('192.168.1.107','3333','matlab');

% Add a subscriber function.
funct4 = registerfunction(connect4,'MECHDataParser','*/MECHCommand');

% Add a command channel
channelidx4 = addcommandchannel(connect4,'MECHCommand');


% Connect to the DataTurbine.
start(connect4);

% Flushes data to the DataTurbine.
sendcommand(connect,channelidx,string("Hello");



% sendcommand(connect4,channelidx1,uint8('connect4.droneInitialize()'));
