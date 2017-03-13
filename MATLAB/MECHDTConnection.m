function self=RobotDTConnection(robotName,ipAddress,tcpPort)

self.MECH_send = @MECH_send;
connect = controller(ipAddress,tcpPort,'matlab');
funct = registerfunction(connect,'MECHDataParser','*/MECHdata');
channelidx = addcommandchannel(connect,'commands');
start(connect);

source=Source(1,'none',0);
source.OpenRBNBConnection(strcat(ipAddress,':',tcpPort),robotName);
cmap=ChannelMap;
cmap.Add(robotName);

source.Register(cmap);

function MECH_send( inputVector )
    inputVector = int8(inputVector);
    inputVector = arrayfun(datamap, inputVector, -256, 256, 0, 2048);
    disp(inputVector);

    import com.rbnb.sapi.*;
    cmap=ChannelMap;
    cmap.Add(robotName);
    cmap.PutTimeAuto('timeofday');
    %cmap.PutDataAsFloat32(0,uint8(inputVector));
    cmap.PutDataAsByteArray(0, inputVector);
    source.Flush(cmap,true);

end