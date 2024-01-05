#include <nds.h>
#include <stdio.h>
#include "WiFi_minilib.h"
#include "dswifi9.h"
#include "nds/interrupts.h"

#define MSG 4
#define GAME_ID ((char) 0b110)

typedef enum Messages_e {
	A,	//0x00
	B,	//0x01
	X,	//0x02
	Y	//0x03
}Message;

void sendMessage()
{
	char msg[MSG] = { GAME_ID };

	//Poll the keypad
	scanKeys();
	unsigned short keys = keysDown();

	//Print and send a message if key pressed
	switch(keys)
	{
	case KEY_A:
		printf("You pressed A\n");
		msg[MSG - 1] = (char)A;
		sendData(msg, MSG);
		break;
	case KEY_B:
		printf("You pressed B\n");
		msg[MSG - 1] = (char)B;
		sendData(msg, MSG);
		break;
	case KEY_X:
		printf("You pressed X\n");
		msg[MSG - 1] = (char)X;
		sendData(msg, MSG);
		break;
	case KEY_Y:
		printf("You pressed Y\n");
		msg[MSG - 1] = (char)Y;
		sendData(msg, MSG);
		break;
	}
}

void receiveMessage()
{
	char msg[MSG];

	//Listen for messages from others
	if(receiveData(msg, MSG) == MSG)
	{
        if (msg[0] != GAME_ID) {
            printf("WRONG GAME_ID\n");
        }

		//If received, decode the key and print
		switch(msg[MSG - 1])
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

    // disconnectFromWiFi();
    // swiWaitForVBlank();
    // closeSocket();
    // swiWaitForVBlank();

    // if (Wifi_CheckInit()) {
    //     printf("Still init!\n");
    // } else {
    //     printf("not init");
    // }

    // swiWaitForVBlank();

    //Initialize WiFi
    // if(initWiFi())
    // 	printf("WiFi OK!\n");
    // else
    // 	printf("Error WiFi\n");

    // //Open Socket
    // if(openSocket())
    // 	printf("Socket OK!\n");
    // else
    // 	printf("Error Socket\n");

    while(1)
    {
    	//Send a message if key is pressed
    	sendMessage();
    	//Receive message of someone else that pressed a key
    	receiveMessage();

        swiWaitForVBlank();
    }
}
