#include <windows.h>
#include <stdio.h>	//For file/serial I/O

int main(int argc, char* argv[]) {
	if(argc < 3){
		printf("VFD Name Changer\nUsage: %s COM NEWNAME\nNAME MUST BE NO MORE THAN 35 CHARACTERS!!!\n", argv[0]);
		exit(1);
	}
	
	DCB port_attributes;		//Sturcture for serial port attributes
	DWORD bytes_written = 0;	//Counter for number of bytes written to the serial port/file
	
	unsigned char sendbuffer[35], begin[2] = {0xFE, 0x22}, end[2] = {0x22, 0xFF};
	memset(&sendbuffer, 0x20, sizeof(sendbuffer));

	//unsigned char aidadump[] = { 0xFE, 0x99, 0xFF, 0xFE, 0x9C, 0xFF, 0xFE, 0x48, 0x43, 0x50, 0x55, 0x20, 0x33, 0x35, 0x35, 0x39, 0x20, 0x4D, 0x48, 0x7A, 0x20, 0x38, 0x25, 0x20, 0x20, 0x20, 0x32, 0x35, 0x33, 0x33, 0x38, 0x20, 0x4D, 0x42, 0x20, 0x36, 0x32, 0x7C, 0x20, 0x20, 0x20, 0x32, 0x32, 0x3A, 0x32, 0x38, 0x20, 0x20, 0xFE, 0x47, 0x01, 0x02, 0x47, 0x50, 0x55, 0x20, 0x31, 0x35, 0x33, 0x31, 0x20, 0x4D, 0x48, 0x7A, 0x20, 0x30, 0x25, 0x20, 0x20, 0x20, 0x20, 0x32, 0x33, 0x36, 0x36, 0x20, 0x4D, 0x42, 0x20, 0x35, 0x36, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 };

	HANDLE my_port = CreateFileA(argv[1], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (my_port == INVALID_HANDLE_VALUE)
		exit(1);

	GetCommState(my_port, &port_attributes);	//Get and alter serial attributes for the display
	port_attributes.BaudRate = CBR_9600;
	port_attributes.ByteSize = 8;
	port_attributes.Parity = NOPARITY;
	port_attributes.StopBits = ONESTOPBIT;
	port_attributes.fRtsControl = RTS_CONTROL_DISABLE;
	SetCommState(my_port, &port_attributes);	//Set specific display attributes
	
	printf("VFD Name Changer\n");
	printf("Changing Name on %s to %s\n", argv[1], argv[2]);

	if(sizeof(argv[2]) < 35){
		memcpy(&sendbuffer, argv[2], strlen(argv[2]));
	} else {
		memcpy(&sendbuffer, argv[2], 35);
	}

	WriteFile(my_port, &begin, sizeof(begin), &bytes_written, NULL);
	WriteFile(my_port, &sendbuffer, sizeof(sendbuffer), &bytes_written, NULL);
	WriteFile(my_port, &end, sizeof(end), &bytes_written, NULL);

	CloseHandle(my_port);	//Close serial port/file

	return 0;
}