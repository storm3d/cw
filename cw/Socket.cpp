// Implementation of the Socket class.


#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

#ifdef WIN32
Socket::Socket() : m_sock ( -1 )
{
	memset (&m_addr, 0, sizeof ( m_addr ));
}

Socket::~Socket()
{
  if ( is_valid() )
    ::closesocket  ( m_sock );
}

bool Socket::create()
{
	// Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
	{
		printf("Error at WSAStartup()\n");
        return false;
	}

	m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    IPPROTO_TCP );

	if ( m_sock == INVALID_SOCKET ) {
        printf( "Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
        return false;
    }

	if ( ! is_valid() )
	    return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
  {
	  printf("Error at setsockopt SO_REUSEADDR\n");
	  return false;
  }

  int flag = 1;
  if ( setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)))
  {
	  printf("Error at setsockopt TCP_NODELAY\n");
	  return false;
  }

  return true;

}

bool Socket::bind ( const int port )
{
	if ( !is_valid() )
      return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons ( port );

	if ( ::bind ( m_sock, (SOCKADDR* ) &m_addr, sizeof ( m_addr ) ) == -1 )
      return false;

	return true;
}


bool Socket::listen() const
{
	if ( ! is_valid() )
		return false;

	if ( ::listen ( m_sock, MAXCONNECTIONS ) == -1 )
		return false;

	return true;
}

bool Socket::accept ( Socket& new_socket ) const
{
	int addr_length = sizeof ( m_addr );
	new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( int * ) &addr_length );

	if ( new_socket.m_sock <= 0 )
	    return false;
	else
	    return true;
}

bool Socket::send ( const std::string s ) const
{
	if ( ::send ( m_sock, s.c_str(), (int)s.size(), MSG_NOSIGNAL ) == -1 )
		return false;
	else
		return true;
}

int Socket::recv ( std::string& s ) const
{
	char buf [ MAXRECV + 1 ];
	s = "";
	memset ( buf, 0, MAXRECV + 1 );
	
	int status = ::recv ( m_sock, buf, MAXRECV, 0 );
	if ( status == -1 )
    {
		std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
		return 0;
    }
	else if ( status == 0 )
    {
		return 0;
    }
	else
    {
		s = buf;
		return status;
    }
}

bool Socket::connect ( const std::string host, const int port )
{
	if ( ! is_valid() ) return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons ( port );

//	inet_addr
//	int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );
//	struct sockaddr_in sain;

	struct hostent *hp;
	if ((m_addr.sin_addr.s_addr = inet_addr (host.c_str())) == -1) {
		if ((hp = gethostbyname (host.c_str())) == NULL) {
			std::cout << "gethostbyname\n" ;
			return false;
		}
		m_addr.sin_addr = *(struct in_addr *)*hp->h_addr_list;
	} 

	if ( ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) ) == 0 )
		return true;
	else
	{
		int _errno = WSAGetLastError();
		std::cout << "status == -1   errno == " << _errno << "  in Socket::connect\n";
		return false;
	}
}

#else
Socket::Socket() :
  m_sock ( -1 )
{
  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );
}

Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

bool Socket::create()
{
  m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    0 );

  if ( ! is_valid() )
    return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;

  int flag = 1;
  if ( setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)))
	  return false;


  return true;

}



bool Socket::bind ( const int port )
{

  if ( ! is_valid() )
    {
      return false;
    }



  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );


  if ( bind_return == -1 )
    {
      return false;
    }

  return true;
}


bool Socket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}


bool Socket::accept ( Socket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}


bool Socket::send ( const std::string s ) const
{
  int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  if ( status == -1 )
    {
      return false;
    }
  else
    {
      return true;
    }
}


int Socket::recv ( std::string& s ) const
{
  char buf [ MAXRECV + 1 ];

  s = "";

  memset ( buf, 0, MAXRECV + 1 );

  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 )
    {
      std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      s = buf;
      return status;
    }
}



bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

#endif