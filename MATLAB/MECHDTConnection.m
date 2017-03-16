function self=MECHDTConnection(robotName,ipAddress,tcpPort)

self.MECH_send = @MECH_send;
connect = controller(ipAddress,tcpPort,'matlab');
funct = registerfunction(connect,'MECHDataParser','*/MECHdata');
channelidx = addcommandchannel(connect,'commands');
start(connect);
scrdstid = 1;
controlschem = 2;
function funct = datamap(inp, in_min, in_max, out_min, out_max)
    funct = (inp - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
end
function MECH_send( inputVector )
    
    
    inputVector = uint16([datamap(inputVector(1), -1.0, 1.0, 0.0, 2048.0) datamap(inputVector(2), -1.0, 1.0, 0.0, 2048.0)]);


    topleft = uint8(bitshift(inputVector(1), -8));
    botleft = uint8(bitshift(bitshift(inputVector(1), 8), -8));
  
    topright = uint8(bitshift(inputVector(2), -8));
    botright = uint8(bitshift(bitshift(inputVector(2), 8), -8));

    %disp(inputVector);
    A = uint8([scrdstid controlschem topleft botleft topright botright]);
    disp(A);
    sendcommand(connect, channelidx, A);
end

end
