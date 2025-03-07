#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

class Customer {
public:
    std::string name;
    float savings_balance;
    float checking_balance;

    Customer(std::string customer_name, float savings, float checking)
        : name(customer_name), savings_balance(savings), checking_balance(checking) {}

    void display() {
        std::cout << "Customer: " << name << "\n"
                  << "Savings Account: $" << savings_balance << "\n"
                  << "Checking Account: $" << checking_balance << "\n";
    }
};

class Server {
public:
    int server_fd, client_fd;
    sockaddr_in address;
    std::vector<Customer> customers;
    int addr_len = sizeof(address);

    Server(int port) {
        // Create server socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            std::cerr << "Failed to create socket.\n";
            exit(EXIT_FAILURE);
        }

        // Setup the server address
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind the socket
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "Bind failed.\n";
            exit(EXIT_FAILURE);
        }

        // Listen for incoming connections
        if (listen(server_fd, 3) < 0) {
            std::cerr << "Listen failed.\n";
            exit(EXIT_FAILURE);
        }
        std::cout << "Server listening on port " << port << "\n";
    }

    void acceptConnection() {
        // Accept incoming connection
        client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addr_len);
        if (client_fd < 0) {
            std::cerr << "Client connection failed.\n";
            exit(EXIT_FAILURE);
        }

        std::cout << "Client connected\n";
    }

    void receiveData() {
        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));

            int valread = read(client_fd, buffer, sizeof(buffer));
            if (valread == 0) {
                break;  // Client disconnected
            }

            // Process received data
            std::string data(buffer);
            std::string name = data.substr(0, data.find(":"));
            float savings = std::stof(data.substr(data.find(":") + 1, data.find(",") - data.find(":") - 1));
            float checking = std::stof(data.substr(data.find(",") + 1));

            Customer new_customer(name, savings, checking);
            customers.push_back(new_customer);

            std::cout << "Received customer data: " << name << ", Savings: " << savings << ", Checking: " << checking << "\n";

            // Send acknowledgment to client
            std::string ack = "Data received and processed.";
            send(client_fd, ack.c_str(), ack.length(), 0);
        }
    }

    void closeServer() {
        close(client_fd);
        close(server_fd);
    }
};

int main() {
    Server server(8080);
    server.acceptConnection();
    server.receiveData();
    server.closeServer();

    return 0;
}
