This repository contains three applications.

1. A simple Server Client Application using TCP sockets.

Description:
  The server starts waiting for connections. Once a client connects to a server,the server sends a welcome message.
  The client displays the message received from the server on the terminal.
 
Limitations:
  No error handling is implemented yet.
  The server can handle only one client connection.
  
2. A simple Echo Client Server Application using TCP Sockets.

Description:
  The server starts waiting for connections. Once a client connects to a server, the server is waiting for a message 
  to be received from the client. The server displays message received from the client and sends/echoes it back to 
  the client. The client starts by connection to the server. The client asks the user to enter a message to send to 
  the server and displays the message received from the server.
  
Parameters:
  Server: Port number
  Client: IP address of the server and the port number

Limitations:
  The server can handle only one client connection currently.
  Some values are hardcoded
