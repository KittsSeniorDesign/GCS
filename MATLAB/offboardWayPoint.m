function self=offboardWayPoint(robotName,ipAddress,tcpPort)

connect = controller(ipAddress,tcpPort,'matlab');
funct = registerfunction(connect,'MECHDataParser','*/MECHdata');
channelidx = addcommandchannel(connect,'commands');
start(connect);


scrdstid = 1;
controlschem = 5;

waypointThresh = 5;
speed = 75;
position = [0, 0, 0, 0];
mPower = [0 ,0];

function funct = datamap(inp, in_min, in_max, out_min, out_max)
    funct = (inp - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
end

	while(1)
		disp(position);
		%Seperate into individual tag strings
		tag1string = position(1:(paren(1)));


		%Parse Tag data into x,y,z,t

		% Finding the correct delimiters for tag1
		comma = strfind(tag1string, ',');

		mx = str2num(tag1string(2:(comma(1)-1)));
		my = str2num(tag1string((comma(1)+1):(comma(2)-1)));
		z = str2num(tag1string((comma(2)+1):(comma(3)-1)));
		h = str2num(tag1string((comma(3)+1):(end-1)));


		if (x > waypointThresh | y > waypointThresh)
			thetam = degtorad(thetam);
			x = wx - mx;
			y = wy - my;


			thetaw = atan2(y, x);
			phi = thetaw - thetam;
			if phi > 0
				rm = speed * cos(phi);
				lm = speed;
			elseif phi < 0
				rm = speed;
				lm = speed * cos(phi);
			else
				rm = speed;
				lm = speed;
			end

			%mpowers = uint16([abs(lm), abs(rm)]);

			ldir = 0;
			if lm < 0
				ldir = 1;
			end

			rdir = 0;
			if rm < 0
				rdir = 1;
			end

			%direction = uint16([ldir, rdir]);

			lm = abs(lm) * ldir;
			rm = abs(rm) * rdir;
			inputVector(1) = lm;
			inputVector(2) = rm;

			inputVector = uint16([datamap(inputVector(1), -1.0, 1.0, 0.0, 2048.0) datamap(inputVector(2), -1.0, 1.0, 0.0, 2048.0)]);
		    topleft = uint8(bitshift(inputVector(1), -8));
		    botleft = uint8(bitshift(bitshift(inputVector(1), 8), -8));
		  
		    topright = uint8(bitshift(inputVector(2), -8));
		    botright = uint8(bitshift(bitshift(inputVector(2), 8), -8));
			A = uint8([scrdstid controlschem topleft botleft topright botright]);
		   

		else
			lm = 0;
			rm = 0;
			inputVector = uint16([datamap(inputVector(1), -1.0, 1.0, 0.0, 2048.0) datamap(inputVector(2), -1.0, 1.0, 0.0, 2048.0)]);
		    topleft = uint8(bitshift(inputVector(1), -8));
		    botleft = uint8(bitshift(bitshift(inputVector(1), 8), -8));
		  
		    topright = uint8(bitshift(inputVector(2), -8));
		    botright = uint8(bitshift(bitshift(inputVector(2), 8), -8));
			A = uint8([scrdstid controlschem topleft botleft topright botright]);
		   

		end


		
		disp(A);
		sendcommand(connect, channelidx, A);


	end

end