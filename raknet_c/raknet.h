#ifndef __RAKNET_H
#define __RAKNET_H

#ifdef __cplusplus
extern "C" {
#endif

// class RakPeerInterface

RN_RakPeer RN_RakPeerGetInstance();

void RN_RakPeerDestroyInstance(RN_RakPeer peer);

RN_StartupResult RN_RakPeerStartup(RN_RakPeer peer, unsigned int maxConnections, RN_SocketDescriptor* socketDescriptors, unsigned int socketDescriptorCount, int threadPriority);

RN_ConnectionAttemptResult RN_RakPeerConnect(RN_RakPeer peer, const char *host, unsigned short remotePort, const char *passwordData,
                                           int passwordDataLength, RN_PublicKey *publicKey,
                                           unsigned connectionSocketIndex, unsigned sendConnectionAttemptCount,
                                           unsigned timeBetweenSendConnectionAttemptsMS,
                                           RN_TimeMS timeoutTime);

void RN_RakPeerShutdown(RN_RakPeer peer, unsigned int blockDuration, unsigned char orderingChannel, RN_PacketPriority disconnectionNotificationPriority);

//void RN_RakPeerCloseConnection(RN_RakPeer peer, const RN_AddressOrGUID target, bool sendDisconnectionNotification, unsigned char orderingChannel, RN_PacketPriority disconnectionNotificationPriority);

RN_Packet* RN_RakPeerReceive(RN_RakPeer peer);

void RN_RakPeerDeallocatePacket(RN_RakPeer peer, RN_Packet *packet);

uint32_t RN_RakPeerSend(RN_RakPeer peer, const char *data, const int length, RN_PacketPriority priority, RN_PacketReliability reliability,
                         char orderingChannel, const RN_AddressOrGUID systemIdentifier, bool broadcast,
                         uint32_t forceReceiptNumber);

#ifdef __cplusplus
}
#endif

#endif
