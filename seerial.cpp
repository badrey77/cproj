#include <iostream>
#include <windows.h>

int main(int argc, char** argv) {
    HANDLE hSerial;
    const char* portName = "COM3"; // Change to the appropriate COM port
    
    // Open the serial port
    hSerial = CreateFileA(portName,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open the serial port." << std::endl;
        return 1;
    }

    // Configure the serial port settings
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_115200; // Set baud rate to 115200
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Sending a command
    char* cmd = argv[1];
    printf("%s\r\n",cmd);
    const char* command = "AT+COPS?\r\n"; // Replace with your command
    DWORD bytesWritten;
    if (!WriteFile(hSerial, command, strlen(command), &bytesWritten, NULL)) {
        std::cerr << "Error sending data." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Reading data
    char buffer[128];
    DWORD bytesRead;
    if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
        std::cerr << "Error reading data." << std::endl;
    } else {
        buffer[bytesRead] = '\0';
        std::cout << "Received data: " << buffer << std::endl;
    }

    // Close the serial port
    CloseHandle(hSerial);

    return 0;
}
