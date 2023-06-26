import socket
import os
from pynput import keyboard

class Server:
    """ Server that sends random data to the client when spacebar is pressed."""
    def __init__(self, host, port):
        """ Initializes the server with given host and port
        
        :param host: Host IP address
        :param port: Port number
        """
        self.host = host
        self.port = port
        self.server_socket = None
        self.client_socket = None

        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        try:
            self.server_socket.bind((self.host, self.port))
        except OSError as e:
            print(f"Bind error: {str(e)}")
            exit(1)
            
        self.server_socket.listen(1)
        print(f"Server listening on {self.host}:{self.port}...")

    def _checksum(self, data):
        """ Calculates the checksum of given data
        
        :param data: Data to calculate the checksum for
        
        :return: Checksum of the data
        """
        return bytes([sum(data) % 256])
    
    def _random_data(self, length, checksum=False):
        """ Returns random data of given length in bytes
        :param length: Length of random data in bytes
        
        :return: Random data bytes
        """
        data = os.urandom(length)
        if checksum:
            data += self._checksum(data)
        return data
    
    def start(self):
        """ Starts the server and listens for incoming connections
        """
        while True:
            self.client_socket, client_address = self.server_socket.accept()
            print(f"\nAccepted connection from {client_address[0]}:{client_address[1]}")

            with keyboard.Listener(on_release=self._on_key_release) as listener:  
                listener.join()

    def stop(self):
        """ Stops the server and closes the sockets"""
        if self.client_socket:
            self.client_socket.close()
        if self.server_socket:
            self.server_socket.close()

    def run(self):
        """ Runs the server"""
        try:
            self.start()
        except socket.error as e:
            print(f"Error: {str(e)}")
        finally:
            self.stop()

    def _on_key_release(self, key):
        """ Callback function that is called when a key is pressed
        
        :param key: Key that is pressed
        """
        print("\r", end="")
        if key == keyboard.Key.esc:
            self.client_socket.close()
            print(f"Connection closed.")
            return False
        
        elif key == keyboard.Key.space and self.client_socket:
            data = self._random_data(20, checksum=True)
            try:
                self.client_socket.sendall(data)
            except (socket.error, socket.timeout):
                print(f"Client disconnected - connection closed.")
                self.client_socket.close()
                return False


if __name__ == "__main__":
    host = '127.0.0.1'
    port = 10200

    server = Server(host, port)
    server.run()
