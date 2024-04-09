#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // Include ws2tcpip.h for InetPtonA
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Open the ping handle
    HANDLE hIcmp = IcmpCreateFile();
    if (hIcmp == INVALID_HANDLE_VALUE) {
        std::cerr << "IcmpCreateFile failed." << std::endl;
        WSACleanup();
        return 2;
    }

    // The IP address to ping (use a regular C-style string for IP addresses)
    const char* ipAddress = "192.168.50.81"; // Example IP address
    IN_ADDR ipaddr = { 0 };

    // Use InetPtonA to convert the IP address string to a network address
    if (InetPtonA(AF_INET, ipAddress, &ipaddr) != 1) {
        std::cerr << "Invalid IP address." << std::endl;
        IcmpCloseHandle(hIcmp);
        WSACleanup();
        return 3;
    }

    DWORD replySize = sizeof(ICMP_ECHO_REPLY) + 8;
    void* replyBuffer = (void*)malloc(replySize);

    // Send the ping request
    DWORD retVal = IcmpSendEcho(hIcmp, ipaddr.S_un.S_addr, NULL, 0, NULL, replyBuffer, replySize, 1000);
    if (retVal != 0) {
        // The call succeeded, process the reply to get the status
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)replyBuffer;
        if (pEchoReply->Status == IP_SUCCESS) {
            std::cout << "Ping to " << ipAddress << " successful." << std::endl;
            std::cout << "Roundtrip time: " << pEchoReply->RoundTripTime << " milliseconds" << std::endl;
        }
        else {
            std::cout << "Ping to " << ipAddress << " failed." << std::endl;
        }
    }
    else {
        std::cerr << "IcmpSendEcho failed." << std::endl;
    }

    // Cleanup
    free(replyBuffer);
    IcmpCloseHandle(hIcmp);
    WSACleanup();

    return 0;
}
