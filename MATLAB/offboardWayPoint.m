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

			mPower = uint16([datamap(inputVector(1), -1.0, 1.0, 0.0, 2048.0) datamap(inputVector(2), -1.0, 1.0, 0.0, 2048.0)]);
		    topleft = uint8(bitshift(inputVector(1), -8));
		    botleft = uint8(bitshift(bitshift(inputVector(1), 8), -8));
		  
		    topright = uint8(bitshift(inputVector(2), -8));
		    botright = uint8(bitshift(bitshift(inputVector(2), 8), -8));
			A = uint8([scrdstid controlschem topleft botleft topright botright]);
		   

		else
			mpowers = uint16([0, 0]);
			direction = uint16([0, 0]);

		end


		
		disp(A);
		sendcommand(connect, channelidx, A);


	end

end