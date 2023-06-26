#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::string;

const int MESSAGE_SIZE = 20;
const int CHECKSUM_SIZE = 1;
const int BUFFER_SIZE = MESSAGE_SIZE + CHECKSUM_SIZE;

const char *IP_ADDRESS = "127.0.0.1";
const int PORT = 10200;

/**  
 * Prints the hexadecimal representation of the given bytes.
 * 
 * @param bytes The bytes to print.
 * @param len The number of bytes to print.
 * 
 * @return void
*/
void print_hex(char* bytes, int len){
    for (int i = 0; i < len; i++) {
        printf("%02x", (unsigned char)bytes[i]);
    }
}

/**  
 * Validates the checksum of the given bytes.
 * 
 * @param bytes The bytes to validate.
 * @param len The number of bytes to validate.
 * @param checksum The checksum to validate against.
 * 
 * @return bool True if the checksum is valid, false otherwise.
*/
bool is_checksum_valid(char* bytes, int len, unsigned char checksum){
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += (unsigned char)bytes[i];
    }

    unsigned char calc_checksum = sum % 256;
    return calc_checksum == checksum;
}   

/**  
 * Prints the given message.
 * 
 * @param buffer The message to print.
 * 
 * @return void
*/
void print_message(char* buffer){
    std::cout << "Received data: ";
    print_hex(buffer, MESSAGE_SIZE);

    if (is_checksum_valid(buffer, MESSAGE_SIZE, buffer[MESSAGE_SIZE])) {
        std::cout << " Checksum OK" << std::endl;;
    } else {
        std::cout << " Checksum NOT OK" << std::endl;;
    }
}

int main() {

    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Set server address details
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    struct sockaddr *sockaddr_ptr = (struct sockaddr*)&server_address;

    // Connect to server    
    std::cout << "Connecting to the server at " << IP_ADDRESS << ":" << PORT << "..." << std::endl;
    if (connect(client_socket, sockaddr_ptr, sizeof(server_address)) == -1) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return 1;
    }
    std::cout << "Successfully connected to the server." << std::endl;

    // Receive and print server response until connection is closed
    char buffer[BUFFER_SIZE];
    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        int num_bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (num_bytes_read == -1) {
            std::cerr << "Failed to receive data from the server." << std::endl;
            close(client_socket);
            return 1;
        } else if (num_bytes_read == 0) {
            std::cout << "Connection closed by the server." << std::endl;
            break;
        }

        print_message(buffer);
    }

    // Close the socket
    close(client_socket);

    return 0;
}
