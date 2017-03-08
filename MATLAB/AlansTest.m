function self=AlansTest()
import com.rbnb.sapi.*;

robotName = 'Derp';
ipAddress = 'localhost';
tcpPort = '3333';
self.alanssend = @alanssend;

source=Source(1,'none',0);
source.OpenRBNBConnection(strcat(ipAddress,':',tcpPort),robotName);
cmap=ChannelMap;
cmap.Add(robotName);
source.Register(cmap);
%while true
%    cmap=ChannelMap;
%    cmap.Add(robotName);
%    cmap.PutTimeAuto('timeofday');
%    cmap.PutDataAsByteArray(0,uint8(inputVector));
    
%    source.Flush(cmap,true);
%    pause(1);
%end

    function alanssend( inputVector )
        inputVector = int8(inputVector);
        inputVector = arrayfun(datamap, inputVector, -256, 256, 0, 2048);
        disp(inputVector);
    %UNTITLED2 Summary of this function goes here
    %   Detailed explanation goes here
        import com.rbnb.sapi.*;
        cmap=ChannelMap;
        cmap.Add(robotName);
        cmap.PutTimeAuto('timeofday');
        %cmap.PutDataAsFloat32(0,uint8(inputVector));
        cmap.PutDataAsByteArray(0, inputVector);
        source.Flush(cmap,true);

    end
end