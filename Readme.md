# TCP Server and Client

## Server 
Server implemented using *Python3* located in `server.py` that sends 20 bytes random data to the client when the spacebar is pressed and closes the connection when Esc is pressed.

### Requirements
* [pynput library](https://pypi.org/project/pynput/) - for monitoring the keyboard input

### Usage 
The se
```
$ python3 server.py 
Server listening on 127.0.0.1:10200...
```

## Client 
TCP client implemented using *C++* located in `client.cpp` that connects to the server and prints the received data in HEX alongside the checksum status.

### Usage
Compile and run using the `Makefile`
```
$ make run 
Connecting to the server at 127.0.0.1:10200...
Successfully connected to the server.
 Received data: f6173f12a1d0ddd99e97462cb8039a49fe35de13 Checksum OK
 Received data: 156de886e1f38efd65754e317c71b02fb2f2cd83 Checksum OK
```