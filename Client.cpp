#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

class Client {
public:
    int sock;
    sockaddr_in server_addr;

    Client(std::string ip, int port) {
        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cerr << "Socket creation failed.\n";
            exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address.\n";
            exit(EXIT_FAILURE);
        }

        // Connect to server
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Connection failed.\n";
            exit(EXIT_FAILURE);
        }
    }

    void sendCustomerData(std::string name, float savings, float checking) {
        std::string data = name + ":" + std::to_string(savings) + "," + std::to_string(checking);

        // Send data to the server
        send(sock, data.c_str(), data.length(), 0);

        // Receive acknowledgment
        char buffer[1024];
        int valread = read(sock, buffer, sizeof(buffer));
        buffer[valread] = '\0';
        std::cout << "Server response: " << buffer << "\n";
    }

    void closeClient() {
        close(sock);
    }
};

int main() {
    Client client("127.0.0.1", 8080);

    std::string name;
    float savings, checking;

    // Input customer details
    std::cout << "Enter customer name: ";
    std::getline(std::cin, name);
    std::cout << "Enter savings amount: ";
    std::cin >> savings;
    std::cout << "Enter checking amount: ";
    std::cin >> checking;

    client.sendCustomerData(name, savings, checking);
    client.closeClient();

    return 0;
}
