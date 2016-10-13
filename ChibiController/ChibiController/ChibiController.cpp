// ChibiController.cpp : Defines the entry point for the console application.
//

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <sstream>
#include <iostream>
#include <cmath>
#include "CXBOXController.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "12345"

#define LEFT 0
#define RIGHT 1

// from 0-65534
#define LEFT_STICK_DEADZONE  7500
#define RIGHT_STICK_DEADZONE 7500
#define TRIGGER_THRESHOLD    30

// maxForward - 127 = 127 - maxBackward
#define maxMove 220
#define minMove 0
// maxSm = 255 - maxForward
#define maxSm 35
// maxSp = maxForward
#define maxSp 220

#define STEERING_THROTTLE_OFFBOARD 1
#define STEERING_THROTTLE_ONBOARD 2
#define TANK 3
#define VELOCITY_HEADING 4
// this number should be equal to whatever the number above it is
#define MAX_CONFIGS 4 

using namespace std;

CXBOXController* Player1;
SOCKET ConnectSocket = INVALID_SOCKET;
int mode = 1;

SOCKET connectToBot(PCSTR address) {
	SOCKET ConnectSocket = INVALID_SOCKET;
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(address, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return INVALID_SOCKET;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return INVALID_SOCKET;
		}

		//cout << ptr->ai_addr << endl;
		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		cout << "connected apprently\n";
		if (iResult == SOCKET_ERROR) {
			cout << "closing socket\n";
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return INVALID_SOCKET;
	}

	// Send an initial buffer
	/*	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
	printf("send failed with error: %d\n", WSAGetLastError());
	closesocket(ConnectSocket);
	WSACleanup();
	return 1;
	}
	printf("Bytes Sent: %ld\n", iResult);
	*/
	return ConnectSocket;
}

// returns true if the stick is not in the deadzone
// It will set normX and normY to a number between -1 and 1
bool getStickPos(int stick, float *normX, float *normY) {
	if (Player1->IsConnected()) {
		float magnitude;
		XINPUT_STATE state;
		float X, Y;
		float normMag;
		int deadzone;
		state = Player1->GetState();
		if (stick == LEFT) {
			X = state.Gamepad.sThumbLX;
			Y = state.Gamepad.sThumbLY;
			deadzone = LEFT_STICK_DEADZONE;
		}
		else {
			X = state.Gamepad.sThumbRX;
			Y = state.Gamepad.sThumbRY;
			deadzone = RIGHT_STICK_DEADZONE;
		}
		//determine how far the controller is pushed
		magnitude = sqrt(X*X + Y*Y);
		//determine the direction the controller is pushed
		*normX = X / magnitude;
		*normY = Y / magnitude;
		//*normX = X / abs(X);
		//*normY = Y / abs(Y);
		normMag = 0;

		//check if the controller is outside a circular dead zone
		if (magnitude > deadzone) {
			//clip the magnitude at its expected maximum value
			if (magnitude > 32767) magnitude = 32767;

			//adjust magnitude relative to the end of the dead zone
			magnitude -= deadzone;
			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			normMag = magnitude / (32767 - deadzone);
		}
		else {
			//if the controller is in the deadzone zero out the magnitude
			magnitude = 0.0;
			normMag = 0.0;
		}
		if (normMag > 0) {
			*normX *= normMag;
			*normY *= normMag;
			return true;
		}
		else {
			*normX = 0;
			*normY = 0;
			return false;
		}
	}
	else {
		cout << "Cannot connect to controller. Waiting for it to be fixed.\n";
		*normX = 0;
		*normY = 0;
		return true;
	}
}

float transform(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void incrementMode() {
	mode++;
	if (mode > MAX_CONFIGS) {
		mode = 1;
	}
	cout << mode << endl;
}

char *steeringThrottleOffBoard() {
	//struct sendByte buff;
	char *buffTemp = (char *)malloc(12);
	float LX = 0, LY = 0, RX = 0, RY = 0;
	if (Player1->IsConnected()) {
		XINPUT_STATE state = Player1->GetState();
		// True if not in deadzone
		bool lAlive = getStickPos(LEFT, &LX, &LY);
		bool rAlive = getStickPos(RIGHT, &RX, &RY);
		WORD buttons = state.Gamepad.wButtons;
		//		bool lights = !(buttons | 0x1000);
		//		bool speakers = !(buttons | 0x2000);
		if (buttons & 0x8000) {
			incrementMode();
		}
		float steering = 0;
		if (rAlive) {
			steering = RX;
		}
		float throttle = 0;
		if (lAlive) {
			throttle = LY;
		}
		float sm = transform(abs(steering), 0, 1, 0, maxSm);
		float sp = transform(abs(steering), 0, 1, 0, maxSp);
		float t = transform(abs(throttle), 0, 1, minMove, maxMove);
		float L = t;
		float R = t;
		if (throttle < 0) {
			if (steering < 0) {
				L += sm;
				R -= sp;
			}
			else {
				L -= sp;
				R += sm;
			}
		}
		else {
			if (steering < 0) {
				L -= sp;
				R += sm;
			}
			else {
				L += sm;
				R -= sp;
			}
		}
		L = L < 0 ? 0 : L;
		R = R < 0 ? 0 : R;
		L = L > 255 ? 255 : L;
		R = R > 255 ? 255 : R;
		int mL = 1500;
		int mR = 1500;
		if (throttle < 0) {
			mL = round(transform(L, 0, 255, 1500, 2000));
			mR = round(transform(R, 0, 255, 1500, 2000));
		}
		else {
			mL = round(transform(L, 0, 255, 1500, 1000));
			mR = round(transform(R, 0, 255, 1500, 1000));
		}
		//cout << mL << " " << mR << endl;
		int controlScheme = STEERING_THROTTLE_OFFBOARD; // motor level control
		memcpy(&buffTemp[0], &controlScheme, sizeof(int));
		memcpy(&buffTemp[4], &mL, sizeof(int));
		memcpy(&buffTemp[8], &mR, sizeof(int));
	}
	else {
		cout << "Cannot connect to controller.\n";
	}
	return buffTemp;
}

char *tank() {
	//struct sendByte buff;
	char *buffTemp = (char *)malloc(12);
	float LX = 0, LY = 0, RX = 0, RY = 0;
	if (Player1->IsConnected()) {
		XINPUT_STATE state = Player1->GetState();
		// True if not in deadzone
		bool lAlive = getStickPos(LEFT, &LX, &LY);
		bool rAlive = getStickPos(RIGHT, &RX, &RY);
		WORD buttons = state.Gamepad.wButtons;
		//		bool lights = !(buttons | 0x1000);
		//		bool speakers = !(buttons | 0x2000);
		if ((buttons & 0x8000)) { // ybutton pressed
			incrementMode();
		}
		float rstick = 0;
		if (rAlive) {
			rstick = RY;
		}
		float lstick = 0;
		if (lAlive) {
			lstick = LY;
		}
		int L = round(transform(lstick, -1, 1, 2000, 1000));
		int R = round(transform(rstick, -1, 1, 2000, 1000));
		L = L < 1000 ? 1000 : L;
		R = R < 1000 ? 1000 : R;
		L = L > 2000 ? 2000 : L;
		R = R > 2000 ? 2000 : R;
		cout << L << " " << R << endl;
		int controlScheme = TANK; // motor level control
		memcpy(&buffTemp[0], &controlScheme, sizeof(int));
		memcpy(&buffTemp[4], &L, sizeof(int));
		memcpy(&buffTemp[8], &R, sizeof(int));
	}
	else {
		cout << "Cannot connect to controller.\n";
	}
	return buffTemp;
}

char *steeringThrottleOnBoard() {
	//struct sendByte buff;
	char *buffTemp = (char *)malloc(12);
	float LX = 0, LY = 0, RX = 0, RY = 0;
	if (Player1->IsConnected()) {
		XINPUT_STATE state = Player1->GetState();
		// True if not in deadzone
		bool lAlive = getStickPos(LEFT, &LX, &LY);
		bool rAlive = getStickPos(RIGHT, &RX, &RY);
		WORD buttons = state.Gamepad.wButtons;
		//		bool lights = !(buttons | 0x1000);
		//		bool speakers = !(buttons | 0x2000);
		if (buttons & 0x8000) { // ybutton pressed
			incrementMode();
		}
		float steering = 0;
		if (rAlive) {
			steering = RX;
		}
		float throttle = 0;
		if (lAlive) {
			throttle = LY;
		}
		int s = transform(steering, -1, 1, 1000, 2000);
		int t = transform(throttle, -1, 1, 1000, 2000);
		cout << s << " " << t << endl;
		int controlScheme = STEERING_THROTTLE_ONBOARD; // motor level control
		memcpy(&buffTemp[0], &controlScheme, sizeof(int));
		memcpy(&buffTemp[4], &s, sizeof(float));
		memcpy(&buffTemp[8], &t, sizeof(float));
	}
	else {
		cout << "Cannot connect to controller.\n";
	}
	return buffTemp;
}

char *velocityHeading() {
	char *buffTemp = (char *)malloc(12);
	float LX = 0, LY = 0, RX = 0, RY = 0;
	if (Player1->IsConnected()) {
		XINPUT_STATE state = Player1->GetState();
		// True if not in deadzone
		bool lAlive = getStickPos(LEFT, &LX, &LY);
		bool rAlive = getStickPos(RIGHT, &RX, &RY);
		WORD buttons = state.Gamepad.wButtons;
		//		bool lights = !(buttons | 0x1000);
		//		bool speakers = !(buttons | 0x2000);
		if (buttons & 0x8000) { // ybutton pressed
			incrementMode();
		}
		float heading = 0;
		if (rAlive) {
			// atan2(+,+) = 45, atan2(-,+) = 45, atan2(+,-) = 135, atan2(-,-) = -135
			cout << RY << " " << RX << " ";
			heading = atan2(RY, RX) - (3.14159265 / 2.0);
			if (heading < -3.14159265) {
				heading += 2 * 3.14159265;
			}
		}
		cout << heading*180/3.14159265 << endl;
		float velocity = 0;
		if (lAlive) {
			velocity = transform(LY, -1, 1, 1000, 2000);
		}
		int controlScheme = VELOCITY_HEADING; // motor level control
		memcpy(&buffTemp[0], &controlScheme, sizeof(int));
		memcpy(&buffTemp[4], &velocity, sizeof(float));
		memcpy(&buffTemp[8], &heading, sizeof(float));
	} else {
		cout << "Cannot connect to controller.\n";
	}
	return buffTemp;
}

bool sendByte(char *sendBuf, SOCKET s) {
	int iResult = send(s, sendBuf, 20, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

void cleanUp() {
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
	}
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}

int main() {
	Player1 = new CXBOXController(1);
	/*	while (true) {
	cout << getSendBuf() << endl;
	this_thread::sleep_for(chrono::milliseconds(10));
	}
	*/
	mode = STEERING_THROTTLE_OFFBOARD;
	//SOCKET s = connectToBot("192.168.1.14");
	//connectToBot("192.168.43.20");
	SOCKET s = connectToBot("192.168.1.10");
	//SOCKET s[2] = {connectToBot("192.168.1.14"), connectToBot("192.168.1.10")};
	for (;;) {
		char *sendBuf = NULL;
		if (mode == STEERING_THROTTLE_OFFBOARD) {
			sendBuf = steeringThrottleOffBoard();
		} else if(mode == STEERING_THROTTLE_ONBOARD) {
			sendBuf = steeringThrottleOnBoard();
		} else if(mode == TANK) {
			sendBuf = tank();
		} else if(mode == VELOCITY_HEADING) {
			sendBuf = velocityHeading();
		}
		sendByte(sendBuf, s);
//		for (int i = 0; i < 2; i++) {
//			sendByte(sendBuf, s[i]);
//		}
		this_thread::sleep_for(chrono::milliseconds(100));
	}

	cleanUp();
	return 0;
}