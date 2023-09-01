/***********************************************************************************************************************
 * @author Hackman Lo
 * @file platform_selector.h
 * @description
 *      !!MUST DEFINE OBELISK_MAX_LISTEN_CONNECTION
 *      !!MUST DEFINE SOCKET_TYPE
 * @created_at 2023-08-30
***********************************************************************************************************************/


#if defined(__linux__)
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#elif defined(_WIN32)
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET_TYPE int
#define INVALID_SOCKET -1
#endif
#define OBELISK_MAX_LISTEN_CONNECTION SOMAXCONN

