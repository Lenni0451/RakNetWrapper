#ifndef __RAKNET_ENUMS_H
#define __RAKNET_ENUMS_H

// enums

typedef enum StartupResult {
   RAKNET_STARTED,
   RAKNET_ALREADY_STARTED,
   INVALID_SOCKET_DESCRIPTORS,
   INVALID_MAX_CONNECTIONS,
   SOCKET_FAMILY_NOT_SUPPORTED,
   SOCKET_PORT_ALREADY_IN_USE,
   SOCKET_FAILED_TO_BIND,
   SOCKET_FAILED_TEST_SEND,
   PORT_CANNOT_BE_ZERO,
   FAILED_TO_CREATE_NETWORK_THREAD,
   COULD_NOT_GENERATE_GUID,
   STARTUP_OTHER_FAILURE
} StartupResult;

typedef enum ConnectionAttemptResult {
   CONNECTION_ATTEMPT_STARTED,
   INVALID_PARAMETER,
   CANNOT_RESOLVE_DOMAIN_NAME,
   ALREADY_CONNECTED_TO_ENDPOINT,
   CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS,
   SECURITY_INITIALIZATION_FAILED
} ConnectionAttemptResult;

typedef enum PublicKeyMode {
   /// The connection is insecure. You can also just pass 0 for the pointer to PublicKey in
   /// RakPeerInterface::Connect()
   PKM_INSECURE_CONNECTION,

   /// Accept whatever public key the server gives us. This is vulnerable to man in the middle, but
   /// does not require distribution of the public key in advance of connecting.
   PKM_ACCEPT_ANY_PUBLIC_KEY,

   /// Use a known remote server public key. PublicKey::remoteServerPublicKey must be non-zero.
   /// This is the recommended mode for secure connections.
   PKM_USE_KNOWN_PUBLIC_KEY,

   /// Use a known remote server public key AND provide a public key for the connecting client.
   /// PublicKey::remoteServerPublicKey, myPublicKey and myPrivateKey must be all be non-zero.
   /// The server must cooperate for this mode to work.
   /// I recommend not using this mode except for server-to-server communication as it significantly
   /// increases the CPU requirements during connections for both sides. Furthermore, when it is
   /// used, a connection password should be used as well to avoid DoS attacks.
   PKM_USE_TWO_WAY_AUTHENTICATION
} PublicKeyMode;

typedef enum PacketPriority {
   /// The highest possible priority. These message trigger sends immediately, and are generally not
   /// buffered or aggregated into a single datagram.
   IMMEDIATE_PRIORITY,

   /// For every 2 IMMEDIATE_PRIORITY messages, 1 HIGH_PRIORITY will be sent.
   /// Messages at this priority and lower are buffered to be sent in groups at 10 millisecond
   /// intervals to reduce UDP overhead and better measure congestion control.
   HIGH_PRIORITY,

   /// For every 2 HIGH_PRIORITY messages, 1 MEDIUM_PRIORITY will be sent.
   /// Messages at this priority and lower are buffered to be sent in groups at 10 millisecond
   /// intervals to reduce UDP overhead and better measure congestion control.
   MEDIUM_PRIORITY,

   /// For every 2 MEDIUM_PRIORITY messages, 1 LOW_PRIORITY will be sent.
   /// Messages at this priority and lower are buffered to be sent in groups at 10 millisecond
   /// intervals to reduce UDP overhead and better measure congestion control.
   LOW_PRIORITY,

   /// \internal
   NUMBER_OF_PRIORITIES
} PacketPriority;

typedef enum PacketReliability {
   /// Same as regular UDP, except that it will also discard duplicate datagrams.  RakNet adds (6 to
   /// 17) + 21 bits of overhead, 16 of which is used to detect duplicate packets and 6 to 17 of
   /// which is used for message length.
   UNRELIABLE,

   /// Regular UDP with a sequence counter.  Out of order messages will be discarded.
   /// Sequenced and ordered messages sent on the same channel will arrive in the order sent.
   UNRELIABLE_SEQUENCED,

   /// The message is sent reliably, but not necessarily in any order.  Same overhead as UNRELIABLE.
   RELIABLE,

   /// This message is reliable and will arrive in the order you sent it.  Messages will be delayed
   /// while waiting for out of order messages.  Same overhead as UNRELIABLE_SEQUENCED. Sequenced
   /// and
   /// ordered messages sent on the same channel will arrive in the order sent.
   RELIABLE_ORDERED,

   /// This message is reliable and will arrive in the sequence you sent it.  Out or order messages
   /// will be dropped.  Same overhead as UNRELIABLE_SEQUENCED. Sequenced and ordered messages sent
   /// on the same channel will arrive in the order sent.
   RELIABLE_SEQUENCED,

   /// Same as UNRELIABLE, however the user will get either ID_SND_RECEIPT_ACKED or
   /// ID_SND_RECEIPT_LOSS based on the result of sending this message when calling
   /// RakPeerInterface::Receive(). Bytes 1-4 will contain the number returned from the Send()
   /// function. On disconnect or shutdown, all messages not previously acked should be considered
   /// lost.
   UNRELIABLE_WITH_ACK_RECEIPT,

   /// Same as UNRELIABLE_SEQUENCED, however the user will get either ID_SND_RECEIPT_ACKED or
   /// ID_SND_RECEIPT_LOSS based on the result of sending this message when calling
   /// RakPeerInterface::Receive(). Bytes 1-4 will contain the number returned from the Send()
   /// function. On disconnect or shutdown, all messages not previously acked should be considered
   /// lost. 05/04/10 You can't have sequenced and ack receipts, because you don't know if the other
   /// system discarded the message, meaning you don't know if the message was processed
   // UNRELIABLE_SEQUENCED_WITH_ACK_RECEIPT,

   /// Same as RELIABLE. The user will also get ID_SND_RECEIPT_ACKED after the message is delivered
   /// when calling RakPeerInterface::Receive(). ID_SND_RECEIPT_ACKED is returned when the message
   /// arrives, not necessarily the order when it was sent. Bytes 1-4 will contain the number
   /// returned from the Send() function. On disconnect or shutdown, all messages not previously
   /// acked should be considered lost. This does not return ID_SND_RECEIPT_LOSS.
   RELIABLE_WITH_ACK_RECEIPT,

   /// Same as RELIABLE_ORDERED_ACK_RECEIPT. The user will also get ID_SND_RECEIPT_ACKED after the
   /// message is delivered when calling RakPeerInterface::Receive(). ID_SND_RECEIPT_ACKED is
   /// returned when the message arrives, not necessarily the order when it was sent. Bytes 1-4 will
   /// contain the number returned from the Send() function. On disconnect or shutdown, all messages
   /// not previously acked should be considered lost. This does not return ID_SND_RECEIPT_LOSS.
   RELIABLE_ORDERED_WITH_ACK_RECEIPT,

   /// Same as RELIABLE_SEQUENCED. The user will also get ID_SND_RECEIPT_ACKED after the message is
   /// delivered when calling RakPeerInterface::Receive(). Bytes 1-4 will contain the number
   /// returned
   /// from the Send() function. On disconnect or shutdown, all messages not previously acked should
   /// be considered lost. 05/04/10 You can't have sequenced and ack receipts, because you don't
   /// know
   /// if the other system discarded the message, meaning you don't know if the message was
   /// processed
   // RELIABLE_SEQUENCED_WITH_ACK_RECEIPT,

   /// \internal
   NUMBER_OF_RELIABILITIES
} PacketReliability;

#endif
