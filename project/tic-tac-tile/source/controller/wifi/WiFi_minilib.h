#define SSID "SEM-NDS"

// WiFi initialization
int initWiFi();

// Socket initialization
int openSocket();

// Receive data trough the socket
int receiveData(char * data_buff, int bytes);

// Send data through the socket
int sendData(char * data_buff, int bytes);

// Close the Socket
void closeSocket();

// Disconnect the WiFi
void disconnectFromWiFi();
