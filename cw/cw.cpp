// cw.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"

int main(int argc, char* argv[])
{
	/*
	std::cout << "running....\n";
  try
    {
          // Create the listening socket
      ServerSocket server ( 30000 );

      while ( true )
        {    //create the conversational socket
          ServerSocket new_sock;
             // wait for a client connection
          server.accept ( new_sock );

          try
            {
              while ( true )
                {  // read the string and write it back
                  std::string data;
                  new_sock >> data;
                  new_sock << data;
                }
            }
          catch ( SocketException& ) {}

        }
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
*/

	
	try
    {

      ClientSocket client_socket ( "192.168.10.1", 17676 );

      std::string reply;

      try
	{
	  client_socket << "Hi there";
	  client_socket >> reply;
	}
      catch ( SocketException& ) {}

      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
  
}

