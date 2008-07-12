// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <string>

#ifdef WIN32
	#include "winsock2.h"
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <arpa/inet.h>
#endif

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;
const int MSG_NOSIGNAL = 0; // defined by dgame

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;
  int recv ( std::string& ) const;


  bool is_valid() const { return m_sock != -1; }

 private:

#ifdef WIN32
	SOCKET m_sock;
#else
	int m_sock;
#endif
  sockaddr_in m_addr;
};


#endif
