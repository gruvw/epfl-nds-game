#include <nds.h>
#include <stdio.h>
#include "WiFi_minilib.h"

typedef enum Messages_e {
	A,	//0x00
	B,	//0x01
	X,	//0x02
	Y	//0x03
}Message;

void sendMessage()
{
	char msg[1];

	//Poll the keypad
	scanKeys();
	unsigned short keys = keysDown();

	//Print and send a message if key pressed
	switch(keys)
	{
	case KEY_A:
		printf("You pressed A\n");
		msg[0] = (char)A;
		sendData(msg, 1);
		break;
	case KEY_B:
		printf("You pressed B\n");
		msg[0] = (char)B;
		sendData(msg, 1);
		break;
	case KEY_X:
		printf("You pressed X\n");
		msg[0] = (char)X;
		sendData(msg, 1);
		break;
	case KEY_Y:
		printf("You pressed Y\n");
		msg[0] = (char)Y;
		sendData(msg, 1);
		break;
	}
}

void receiveMessage()
{
	char msg[1];

	//Listen for messages from others
	if(receiveData(msg,1)>0	)
	{
		//If received, decode the key and print
		switch(msg[0])
		{
		case A:
			printf("Other pressed A\n");
			break;
		case B:
			printf("Other pressed B\n");
			break;
		case X:
			printf("Other pressed X\n");
			break;
		case Y:
			printf("Other pressed Y\n");
			break;
		}
	}
}

int main(void) {

    consoleDemoInit();

    //Initialize WiFi
    if(initWiFi())
    	printf("WiFi OK!\n");
    else
    	printf("Error WiFi\n");

    //Open Socket
    if(openSocket())
    	printf("Socket OK!\n");
    else
    	printf("Error Socket\n");


    while(1)
    {
    	//Send a message if key is pressed
    	sendMessage();
    	//Receive message of someone else that pressed a key
    	receiveMessage();

        swiWaitForVBlank();
    }
}
