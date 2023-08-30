/***********************************************************************************************************************
 * @author Hackman Lo
 * @file platform_selector.h
 * @description
 *      !!MUST DEFINE OBELISK_MAX_LISTEN_CONNECTION
 * @created_at 2023-08-30
***********************************************************************************************************************/


#if defined(__linux__)
#elif defined(_WIN32)
#include <winsock2.h>
#include <Ws2tcpip.h>
#elif defined(__APPLE__)
#include <netdb.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#define OBELISK_MAX_LISTEN_CONNECTION SOMAXCONN
