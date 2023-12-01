#include "../fb-raknet/Source/PacketPriority.h"
#include "../fb-raknet/Source/RakPeerInterface.h"
#include "../fb-raknet/Source/RuntimeVars.h"

using namespace RakNet;

#include "raknet_types.h"
#include "raknet.h"

#ifndef _WIN32
#define visibility __attribute__((visibility("default")))
#else
#define visibility
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    visibility RN_RakPeer RN_RakPeerGetInstance()
    {
        return RakPeerInterface::GetInstance();
    }

    visibility void RN_RakPeerDestroyInstance(RN_RakPeer peer)
    {
        return RakPeerInterface::DestroyInstance((RakPeerInterface *)peer);
    }

    visibility RN_StartupResult RN_RakPeerStartup(RN_RakPeer peer, unsigned int maxConnections, RN_SocketDescriptor *socketDescriptors, unsigned int socketDescriptorCount, int threadPriority)
    {
        RakPeerInterface *p = (RakPeerInterface *)peer;
        return (RN_StartupResult)p->Startup(maxConnections, (SocketDescriptor *)socketDescriptors, socketDescriptorCount, threadPriority);
    }

    visibility RN_ConnectionAttemptResult RN_RakPeerConnect(RN_RakPeer peer, const char *host, unsigned short remotePort, const char *passwordData,
                                                 int passwordDataLength, RN_PublicKey *publicKey,
                                                 unsigned connectionSocketIndex, unsigned sendConnectionAttemptCount,
                                                 unsigned timeBetweenSendConnectionAttemptsMS,
                                                 RN_TimeMS timeoutTime)
    {
        RakPeerInterface *p = (RakPeerInterface *)peer;
        return (RN_ConnectionAttemptResult)p->Connect(host, remotePort, passwordData, passwordDataLength, (PublicKey *)publicKey, connectionSocketIndex, sendConnectionAttemptCount, timeBetweenSendConnectionAttemptsMS, (TimeMS)timeoutTime);
    }

    visibility void RN_RakPeerShutdown(RN_RakPeer peer, unsigned int blockDuration, unsigned char orderingChannel, RN_PacketPriority disconnectionNotificationPriority)
    {
        RakPeerInterface *p = (RakPeerInterface *)peer;
        p->Shutdown(blockDuration, orderingChannel, disconnectionNotificationPriority);
    }

    // void RN_RakPeerCloseConnection(RN_RakPeer peer, const RN_AddressOrGUID target, bool sendDisconnectionNotification, unsigned char orderingChannel, RN_PacketPriority disconnectionNotificationPriority) {
    //     RakPeerInterface* p = (RakPeerInterface*)peer;
    //     AddressOrGUID aog;
    //     *&aog = *(AddressOrGUID*)&target;
    //     p->CloseConnection(aog, sendDisconnectionNotification, orderingChannel, *(PacketPriority*)&disconnectionNotificationPriority);
    // }

    visibility RN_Packet *RN_RakPeerReceive(RN_RakPeer peer)
    {
        RakPeerInterface *p = (RakPeerInterface *)peer;
        return (RN_Packet *)p->Receive();
    }

    visibility void RN_RakPeerDeallocatePacket(RN_RakPeer peer, RN_Packet *packet)
    {
        RakPeerInterface *p = (RakPeerInterface *)peer;
        p->DeallocatePacket((Packet *)packet);
    }

    visibility uint32_t RN_RakPeerSend(RN_RakPeer peer, const char *data, const int length, RN_PacketPriority priority, RN_PacketReliability reliability,
                            char orderingChannel, const RN_AddressOrGUID systemIdentifier, bool broadcast,
                            uint32_t forceReceiptNumber)
    {
        RakPeerInterface *p = (RakPeerInterface *)peer;
        AddressOrGUID aog;
        *&aog = *(AddressOrGUID *)&systemIdentifier;
        return p->Send(data, length, *(PacketPriority *)&priority, *(PacketReliability *)&reliability, orderingChannel, aog, broadcast, forceReceiptNumber);
    }

    visibility void RN_SetRakNetProtocolVersion(int version)
    {
        SetRakNetProtocolVersion(version);
    }

#ifdef __cplusplus
}
#endif
