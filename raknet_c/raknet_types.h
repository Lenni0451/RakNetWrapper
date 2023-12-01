#ifndef __RAKNET_TYPES_H
#define __RAKNET_TYPES_H

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#include <WinSock2.h>
#endif

#define DEFAULT_THREAD_PRIORITY (-99999)
#define DEFAULT_SEND_CONNECTION_ATTEMPT_COUNT 12
#define DEFAULT_SEND_CONNECTION_ATTEMPT_INTERVAL_MS 500

#define _PP_Instance_ int

// typedefs

typedef void* RN_RakPeer;
typedef uint32_t RN_TimeMS;
typedef unsigned short RN_SystemIndex;
typedef uint32_t RN_BitSize_t;

// enum typedefs
typedef StartupResult RN_StartupResult;
typedef ConnectionAttemptResult RN_ConnectionAttemptResult;
typedef PublicKeyMode RN_PublicKeyMode;
typedef PacketPriority RN_PacketPriority;
typedef PacketReliability RN_PacketReliability;

// structs

typedef struct RN_SocketDescriptor {

   /// The local port to bind to.  Pass 0 to have the OS autoassign a port.
   unsigned short port;

   /// The local network card address to bind to, such as "127.0.0.1".  Pass an empty string to use
   /// INADDR_ANY.
   char hostAddress[32];

   /// IP version: For IPV4, use AF_INET (default). For IPV6, use AF_INET6. To autoselect, use
   /// AF_UNSPEC. IPV6 is the newer internet protocol. Instead of addresses such as
   /// natpunch.jenkinssoftware.com, you may have an address such as fe80::7c:31f7:fec4:27de%14.
   /// Encoding takes 16 bytes instead of 4, so IPV6 is less efficient for bandwidth.
   /// On the positive side, NAT Punchthrough is not needed and should not be used with IPV6 because
   /// there are enough addresses that routers do not need to create address mappings.
   /// RakPeer::Startup() will fail if this IP version is not supported.
   /// \pre RAKNET_SUPPORT_IPV6 must be set to 1 in RakNetDefines.h for AF_INET6
   short socketFamily;

   unsigned short remotePortRakNetWasStartedOn_PS3_PSP2;

   // Required for Google chrome
   _PP_Instance_ chromeInstance;

   // Set to true to use a blocking socket (default, do not change unless you have a reason to)
   bool blockingSocket;

   /// XBOX only: set IPPROTO_VDP if you want to use VDP. If enabled, this socket does not support
   /// broadcast to 255.255.255.255
   unsigned int extraSocketOptions;
} RN_SocketDescriptor;

typedef struct RN_PublicKey {
   /// How to interpret the public key, see above
   RN_PublicKeyMode publicKeyMode;

   /// Pointer to a public key of length cat::EasyHandshake::PUBLIC_KEY_BYTES. See the Encryption
   /// sample.
   char *remoteServerPublicKey;

   /// (Optional) Pointer to a public key of length cat::EasyHandshake::PUBLIC_KEY_BYTES
   char *myPublicKey;

   /// (Optional) Pointer to a private key of length cat::EasyHandshake::PRIVATE_KEY_BYTES
   char *myPrivateKey;
} RN_PublicKey;

typedef struct RN_RakNetGUID {
    uint64_t g;
    RN_SystemIndex systemIndex;
} RN_RakNetGUID;

typedef struct RN_SystemAddress {
    union  // In6OrIn4
   {
#if RAKNET_SUPPORT_IPV6 == 1
      struct sockaddr_storage sa_stor;
      struct sockaddr_in6 addr6;
#endif

      struct sockaddr_in addr4;
   } address;
   unsigned short debugPort;
   RN_SystemIndex systemIndex;
} RN_SystemAddress;

typedef struct RN_Packet {
   /// The system that send this packet.
   RN_SystemAddress systemAddress;

   /// A unique identifier for the system that sent this packet, regardless of IP address (internal
   /// / external / remote system) Only valid once a connection has been established
   /// (ID_CONNECTION_REQUEST_ACCEPTED, or ID_NEW_INCOMING_CONNECTION) Until that time, will be
   /// UNASSIGNED_RAKNET_GUID
   RN_RakNetGUID guid;

   /// The length of the data in bytes
   unsigned int length;

   /// The length of the data in bits
   RN_BitSize_t bitSize;

   /// The data from the sender
   unsigned char *data;

   /// @internal
   /// Indicates whether to delete the data, or to simply delete the packet.
   bool deleteData;

   /// @internal
   /// If true, this message is meant for the user, not for the plugins, so do not process it
   /// through plugins
   bool wasGeneratedLocally;
} RN_Packet;

typedef struct RN_AddressOrGUID {
   RN_RakNetGUID rakNetGuid;
   RN_SystemAddress systemAddress;
} RN_AddressOrGUID;

#endif
