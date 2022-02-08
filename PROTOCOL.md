

# TRiP PROTOCOL SPECIFICATION
The details.

        "Onwaaaard!"
            - Glorion, The Knight from Journey Quest


## Overview
The River Protocol (TRiP) is intended to be a flexible protocol that allows
for the easy building of custom protocols.
TRiP is designed to be abstracted with security by default.
TRiP is designed to allow for different types of communication.
Hopefully you find TRiP useful for your application.
Enjoy!


## Definitions and Terms
**packet:** Raw data transmitted, including IP, UDP, and other headers.  
**segment**: Raw data once non-TRiP headers are stripped.  
**rinfo:** Return information.  
**socket:** Interface to sending/receiving packet information.  
**sender:** Context for sending to specific destination using a socket.  
**router:** Basic packet validation and routing to connections.  
**connection:** Individual connection between routers.  
**stream:** Unidirectional messaging context.  
**message:** Atomic unit of data in a stream.
         Don't confuse with 'message' from dgram event.  
**fragment:** Part of a message.  
**client:** The peer seeking to OPEN a connection.  
**server:** The peer receiving the OPEN request.  
**peer:** The other end of the connection.  
**notify:** Notification to disconnect.  
**disconnect:** Disconnect without any further waiting.  
**kill:** Hard disconnect without sending anything further.  


## Dependencies
Code for version 0 is dependent on libhitime for timeout management.
Code for version 0 is dependent on libsodium for encryption.

Code implementing TRiP over UDP based communication is dependent on appropriate OS interfaces.


## VarInts and Serialization
Variable length integers are denoted as "V" in the octets column.

I don't know if I like this encoding scheme anymore.
Use variable integral type encoding which are ~~little~~ big endian.
See https://developers.google.com/protocol-buffers/docs/encoding


## Transmission Profiles
Transmission profiles are sets of default settings according to use-case.
By specifying good defaults programmers are less prone to errors or
choosing ineffective, sub-optimal settings.

Profiles:
1. Web Server
1. Web Client
1. Gaming Client (Server is likely custom tuned)
1. IOT Server
1. IOT Client
1. Real Time News/Finance/Day Trading Server
1. Real Time News/Finance/Day Trading Client
1. Limp Mode (High packet drop rate)


## Congestion Control
Even with low levels of data transmission there should be congestion control.
Different quantities of transmission may take different methods of control.
TODO reference correct parts of rfc8085

**Retransmission:**
Packet loss retransmission interval should be 1 second for regular loads and 500ms for smaller loads.
Retransmission may be customized for real-time applications.
RTT + Estimated Response Time + Timing Variance could be a good alternative.

**Transmission rates:**
Controlled by credit.
If out-of-credit, for unreliable, earn based on the confirmation of the latest
packet delivered.
Example, if you send ten packets and none of them reach,
you still confirm that the receiver knows they were missed.
This is also used to earn credits.

If out-of-credit, for reliable, earn when data is confirmed for delivery.

**Rount-trip time (RTT) and transmission rates:**
RTT is round trip time and only one packet should be sent for each RTT on light data loads.
RTT should be determined by ping/keep-alives.
RTT probably should be used to determine a timeout for send data.

**Unreliable-ordered data:**
The sender should only send the last received per tick.
The server, if receiving multiple messages, should deliver the latest complete messages and discard any earlier messages.
Confirming the latest can free all used credits prior.

**Inc/dec transmission rates:**
The receiver may increase or decrease credit rates at any time.
The receiver may increase or decrease streams allowed at any time.
The server may require minimum credit or stream allowance.
Zero indicates that the streams are blocked; there is a use-case of one-way data transmission for this (as-well-as keep alive).
The client may increase or decrease, however, this could be considered a breach of custom protocol.

**Ping:**
The ping will determine the connection liveness.
No matter the congestion control, pings should be immediate,
but have a large gap between (30 seconds minimum).
Exponential backoff should be used when resending.
The peer should still report statistics (sent/received) every ping so packet drop rate can be determined.

**Burst:**
When sending data to the same end-point, send data in bursts?
This is for large messages and can take advantage of locality in routing software.


## Credit
Currently credit is the number of bytes allowed to be sent.
Once a receiver flags they have received the data, those credit bytes are replenished.

Note that for unreliable sending the receiver flags the latest message offset
received.
If sender's messages are getting lost then replenishment also suffers and
the sending suffers.
This can be great for congestion control.
This can be terrible for performance of the application.
To help recover credit, the implementation should resend a notification
that a message was sent every RTT.
The receiver, once getting the notification that a message was sent should
send that the message was received, even if it was not.
This works because we are doing unreliable send anyways.

Receivers may choose to delay notifying some messages as received.
However, this delay should be short, less than half of the senders RTT.
If receivers cannot handle the data the credit should be renegotiated.
When renegotiating, all streams are frozen.
If the backpressure is temporary or local to a stream,
then flagging backpressure on the individual stream is appropriate.


## Fields
Notes on common fields.

### ID
The ID field in an OPEN request is unused by the framework;
it is passed along to the user.
The user could use it to map to a specific login key.


### Sequence Numbers
The sequence numbers may start at any value before 2^30.
However, whatever is chosen by the connecting party must be used for a time
before resetting.
Resetting the sequence numbers also resets nonce and keys to prevent
obscure attacks with packet replay.
Note that sequence numbers can just circle back using integer addition overflow.
We still need to reset nonce and keys to prevent attacks.

Items to consider:
- TODO TCP recommends random initial sequence number to avoid stail packets...
- TODO does this apply?
- TODO can an issue arise if a connection on the same ports is opened/closed in quick succession??
- TODO what is the MSL (Max Segment Lifetime)?
- TODO are segments unique to TCP?
- TODO I'm thinking that use of encryption may eliminate need for random sequence numbers...


### STREAM IDs
While any ID could be chosen for a stream, indexes are used instead.
Any number from [0, MAX) where MAX is the maximum numbers of streams.
By doing this we eliminate more complex hashing methods and increase performance.
Implementations may require a power of 2 value for fast masking.


STREAM Closing:  
Ultimately the stream must be closed from the receiving end to prevent
allowing reopening of a stream ID before the receiver is ready.
This acts as a flow control mechanism.
Closing is negotiated because stream creation is optimistic.


## Unencrypted connections
If a connection is unencrypted...
God help you.
But if this is on a VPN or something and the traffic doesn't require security...
But seriously.

There is still a required nonce.
A sequence of random bytes is used to help validate the connection and help
prevent accidental connections to other machines on the same network.
A combination of random nonce, connection ID, sequence numbers,
and IP/Port should make connection mixups improbable.
Perhaps using a UUID in the nonce would be preferable for clusters.
A UUID using a combination of timestamp, node ID, and random value.

If this section about connections mixing up makes you nervous
then go read the TCP spec.

Technically, connection mixups are theoretically possible for any kind of
connection.
They're just VERY statistically improbable; and we take steps to keep it that way.


## Octet Specifications
| Octets | Field |
|:------ |:----- |
| 1 | One octet for this field
| V | Variable length integral value.
| D | Data, variable length. See table below.
| - | Unspecified as the fields are merely for example
| + | Variable items (at least one)
| P | Protocol field(s) specified elsewhere

Data:
| Octets | Field |
|:------ |:----- |
| 2 | Length of data field. Let this be n.
| n | Binary data.


## STREAM Messaging Discussion
Originally the schema for messaging was to break the message into fragments.
The question of how long each fragment was to be and how to efficiently
pack the messages into UDP packets arose.
Wasting packets would slow down overall communications.
So instead of the schema for data looking like the following:
| Octets | Field |
|:------ |:----- |
| 1 | Stream Control and Type
| - | Stream Id
| - | Sequence
| - | Fragment
| - | Fragment Total
| - | Payload

There are two parts to sending messages.
First, the defining of the message by specifying the start point and length:
| Octets | Field |
|:------ |:----- |
| 1 | Stream Control and Type
| - | Stream Id
| - | Offset
| - | Length

Second, the data portion:
| Octets | Field |
|:------ |:----- |
| 1 | Stream Control and Type
| - | Stream Id
| - | Offset
| - | Payload

The receiver can align the data and deliver the message intact.
Using the UMTU can help ensure that messages are sent in one peice,
reducing stress on the receiver.
Options can be set to force the sender to align based on packet size.

TODO is this paragraph necessary?
Resetting the stream's offset will cause the stream to go down for a time.
Forcing all outstanding data to be sent and processed before resetting to zero.
This type of flushing is identical to closing and reopening the stream and isn't even needed.


## General Notes
* "Encrypt" indicates that the following section is encrypted.
* Use exponential backoff up to 5 minutes (or similar) when resending (after 3 tries).
* Always assume breaking encrypted protocol is intended as malicious.
  This is because data in encrypted sections is very unlikely to have been
  damaged, as the decryption process should catch that.
  Failed decryption doesn't always indicate malicious intent.
* After reasonable number of attempts, assume non-response as bad behavior.
* Malicious connections are terminated with reject.
* Malicious messages are dropped.
* Credit is used to track the max number of outstanding packets for which need reliable sending.
* Streams are created on the fly. The cost of streams is incurred on tear-down.
* Re-typing a stream is an error and is considered malicious.
* Streams are unidirectional; client and server maintain separately indexed streams.
* Steps should be taken to prevent interference created by attaching encrypted data to a different packet type.
* Network endianness is big endian, which is what is used where applicable.
* Compliant implementation should perform timeouts for cleanup to advance sequence counter thresholds.
* Implementations should allow for asynchronous behavior with proper interfaces.


## Timestamp
Timestamps are 8 octets and are milliseconds since the Unix epoch.


## Packets
Leading bit of Control value is set if message is encrypted.
Bits that are not specified must be zero and are reserved for future use.
The control value is added to the 5th octet of the nonce as part-of packet replay protection.

Control numbering starts at zero:
- Stream
- Open
- Challenge
- Response
- Ping
- Renew
- Renew Confirm
- Disconnect Notice
- Disconnect Notice Confirm
- Disconnect
- Disconnect Confirm
- Reject

Future Packet Types:
- DDNS
- Forward/Proxy


### PREFIX
Prefixed to every transmission.
| Octets | Field |
|:------ |:----- |
| 1 | Control
| 8 | ID
| 4 | Sequence


### STREAM
These are the most common packet types, so zero is used.

| Octets | Field |
|:------ |:----- |
| P | PREFIX
| 16 | Encrypt
| + | MESSAGE+


### REJECT
Reject incoming connection.
Options for ignoring connections that may be malicious should be provided.

TODO:
Consideration for DDoS Amplification should be taken into account.

| Octets | Field |
|:------ |:----- |
| P | PREFIX
| 48 | Encrypt
| 8 | Timestamp
| 2 | Rejection type
| D | Message (UTF-8 string)

Rejection types are:
0 - Unknown/Other
1 - Busy
2 - Incompatible version
3 - Unsafe connections not allowed
4 - Invalid request
5 - Violation of protocol
6 - User reject
7 - Server error


### OPEN
Open a connection.

Notes:
- PREFIX.ID can be used to request a specific ID.
  The default assumption is that the OPEN.PREFIX.ID field is ignored.
- The PREFIX.Sequence field is the starting value. It can be any integral number
  that fits the number of bytes of the field.
  Recommended that the value is randomly generated by the connecting PEER.

Security:
- Floods of open connection requests should be met with increasing rejections
  and fewer accepted packets.

TODO organize and review:
If extreme cullings are taking place then errors will be emitted.
The packet is sealed with the servers public key, this helps ensure that the
correct server is being reached.
The ID for responses is done because the client may be juggling multiple
connections on the same port.
The timestamp is to help prevent packet replay, if the timestamp of the original
connection is significantly less than the current time, then we're experiencing
packet replay and they should be dropped without a second thought.
The Version ID is listed outside the encrypted data so the correct encryption
scheme can be used.
TODO add SIGNATURE
TODO remove encrypted OPEN feature??? Signatures should be valid enough, right?

| Octets | Field |
|:------ |:----- |
| PRE | PREFIX
| 2 | Major Version ID
| D | Routing Information (Binary)
| OPENING INFO |


### Challenge
Challenge the OPEN request.
Return the server's nonce and public key for the connection.
Same format as OPEN.
If the OPENING INFO has different keys on resubmissions then it is considered malicious and the connection is terminated.
TODO add SIGNATURE

| Octets | Field |
|:------ |:----- |
| PRE | PREFIX

| OPENING INFO |


### OPENING INFO
SECURITY: The signature of the OPEN segment is zero for public servers.
SECURITY: The signature of the OPEN segment MUST sign entire packet for private servers.
SECURITY: The signature of the CHALLENGE segment SHOULD be checked.
SECURITY: The signature of the CHALLENGE segment MUST also sign the OPEN segment.
SECURITY: The signature of the CHALLENGE segment MUST be done for public servers.
| Octets | Field |
|:------ |:----- |
| 48 | Encrypt Padding
| 8 | Receiver ID for future responses/requests
| 24 | Nonce client (Zeroes if unencrypted)
| 32 | Public key client (Zeroes if unencrypted)
| 4 | Sender Max Credit
| 4 | Sender Max Streams
| 4 | Sender Max Message Size
| 4 | Sender Max Messages
| S | SIGNATURE (OUTSIDE ENCRYPTION) |

TODO should this be a part of it???
| 1 | Allowed stream types, cannot be zero.
| 8 | Timestamp


### Forward
UNUSED

TODO should this be re-labeled "redirect"??
Forwarding instructions.
Should only be used with encryption.
| Octets | Field |
|:------ |:----- |
| PRE | PREFIX
| 4 | ID (Zeros or ANY)
| 4 | Sequence
| 48 | Encryption.
| 2 | Major Version ID
| VD | LOCATIONS+
TODO.


### Response
TODO should a ping just be returned and make pinging the connector's responsibility???
TODO currently proceeding to use PING as the response and see if any issues arrive.
Accept the server's CHALLENGE request.
If the client does not respond with this then the connection is closed.
The client cannot have lower limitations than the server.

| Octets | Field |
|:------ |:----- |
| PRE | PREFIX
| 16 | Encrypt
| 8 | Timestamp
| 24 | Nonce of server again for confirmation


### PING
The PING or keepalive packet.
Sending the PING is the responsibility of the connecting entity.

Ping peer connection.
Ping applies at the connection level, hence a ping control flag.
Both should be sending pings around the same time.
Pings are sent in response to pings until the ping requisite is satisfied.
Exponential backoff up to ping interval.
For network robustness a client/server can pause a connection, however, the
default is for the client/server to terminate.
Try to re-resolve original connection information (maybe IP address changed).
Notify user if server is just un-reachable.
If valid reject found, then terminate connection.

Pings are when the send address is checked.
This allows connections to change networks or have disruptions in NAT traversal.

So... If the connection receiver times out, then there should be an option
to send a PING to the sender.
Just in case the receiver's IP address changed.
By default the connection should be either frozen or dropped.

PING.Random + PING.Timestamp help identify the PING.
If incorrect values are returned by the PING'd entity then something is wrong.
Only one PING is outstanding at any given time.

| Octets | Field |
|:------ |:----- |
| PRE | PREFIX
| 16 | Encrypt
| 24 | Random
| 8 | Timestamp
| 4 | RTT
| 8 | Sent Count
| 8 | Received Count


### Renew
TODO review

Reset the sequence and get new keys.

| Octets | Field |
|:------ |:----- |
| PRE | PREFIX
| 16 | Encrypt
| 4 | Non-zero New Sequence
| 24 | New Nonce
| 32 | New Key


### Renew Confirm
TODO review

Confirm that the reset has taken place.

| Octets | Field |
|:------ |:----- |
| PRE | PREFIX
| 16 | Encrypt
| 4 | Non-zero Old Sequence
| 24 | Old Nonce
| 32 | Old Key

### Disconnect Notify
TODO soft notice
TODO leave soft notice to user space?

### Disconnect Notify Confirm
TODO soft notice
TODO leave soft notice to user space?

### Disconnect
TODO review

Disconnect and nicely terminate connection.
Every stream by compliant peer should be closed prior.
After reasonable threshold, resend if not confirmed until confirmed, rejected, or timeout.

| Octets | Field |
|:------ |:----- |
| PRE | PREFIX
| 16 | Encrypt
| 8 | Timestamp
| 24 | Nonce


### Disconnect Confirm
TODO review

Confirm that connection is disconnected.
Every stream by compliant peer should be closed prior to acknowledgement.
The ID should not be immediately reused.
If no ID is found the peer should respond with reject if DDoS amplification is not detected.

| Octets | Field |
|:------ |:----- |
| PRE | PREFIX
| 16 | Encrypt
| 8 | Timestamp
| 24 | Nonce


### MESSAGE
The MESSAGE Protocol is internal and is parsed after decryption.
It contains the data and instructions on how to reconstruct it.

As a checksum and to save an extra octet, the upper two bits of the Stream Control value are used to store the stream type, when applicable.
Streams are optimistic, so they are opened with data being sent.
Streams must be controled and closed using Stream Control.
Any violation of protocol will be considered malicious and the connection will be terminated.

Control numbering starts at zero:
Data
Data Validate Received
Data Received
Backpressure
Backpressure Confirm
Close
Close Confirm


#### Data
Note that all numeric fields (stream id, sequence, fragment, total) are listed as being one octet long; really the uppermost bit indicates continuation.
Thus, numbers can be unlimited, in theory. In practice, 4 octets should not be exceeded for performance reasons and a conforming server should discard and disconnect if this limit is exceeded for security reasons, however, allowing infinite sizes is within the standard (for unique use-cases and future compatibility).
Limits are specified by the users of the protocol and going outside them breaks the standard, the connections are terminated by conforming implementations.
The maximum message size until unlimited amounts are implemented is UMTU * 127.
Note that default limits are set to be reasonable values for modern clients/servers.

| Octets | Field |
|:------ |:----- |
| 1 | Stream Control and Type
| V | Stream Id
| V | Sequence
| 2 | Fragment
| 4 | Fragment Total
| VD | Payload


#### Data Validate Received

| Octets | Field |
|:------ |:----- |
| 1 | Stream Control
| V | Stream Id
| V | Sequence
| V | Fragment


### Data Received

| Octets | Field |
|:------ |:----- |
| 1 | Stream Control
| V | Stream Id
| V | Sequence
| V | Fragment


### Backpressure
TODO

| Octets | Field |
|:------ |:----- |
| 1 | Stream Control
| V | Stream Id
| 1 | Start/stop
| 8 | Timestamp


### Backpressure Confirm
TODO

| Octets | Field |
|:------ |:----- |
| 1 | Message Control
| V | Stream Id
| 1 | Start/stop
| 8 | Timestamp


### Close

| Octets | Field |
|:------ |:----- |
| 1 | Stream Control
| V | Stream Id
| V | Final sequence value


### Close Confirm

| Octets | Field |
|:------ |:----- |
| 1 | Stream Control
| V | Stream Id
| V | Final sequence value


## Attack Mitigation
Address how TRiP handles known attacks.


### Encryption Attack
**Problem:**
Current encryption methods are found to be vulnerable or broken.

**Solution:**
Use another method of encryption.
Update software.

**TRiP:**
Apply new methods of encryption and incrementing the version counter.
Software will need to be updated.
Attacks can proceed against outdated software.
Reject incorrect version numbers to help prevent exposing the vulnerability.
Reject incorrect version numbers to signal that a party must update.


### DDoS Attack
**Problem:**
Sending too many packets

**Solution:**
No known server-side solution.
Mitigated through ISP, Firewall, or intermediate network devices.

**TRiP:**
Rate limit OPEN connections.
Encrypted communications helps drop erroneous segments.


### DDoS Reflection/Amplification Attack
**Problem:**
Create IP packets with the return IP address and port set to the network to DDoS.
Amplified if the response is larger than the request.

**Solution:**
No known perfect solution.
Mitigated by having smaller responses than the request.

**TRiP:**
Mitigated by having known public key for OPEN connection.
Nature of encryption-by-default should help mitigate.
Size of OPEN packet is larger than CHALLENGE, so it won't amplify.
Rate limiting REJECT segments.
Temporarily tracking "sender" can reduce reject messages or OPEN accepts.
No perfect fix for public servers with unencrypted OPEN.
Mitigated by encrypting communication; if decryption fails the segment is dropped.


### Packet Replay
**Problem:**
Packet is captured and resent potentially multiple times.

**Solution:**
Mitigated with sequence numbers and timestamps.
Encryption can ensure that tampering cannot occur, but does not prevent replay.

**TRiP:**
Use a combination of control, sequences, timestamps, encryption, nonces.
OPEN messages can be replayed with no damage or success.
If a connection is already OPENed then the packet is dropped.

If an encrypted STREAM packet is replayed after a stream is closed, data
could be injected if that packet didn't originally arrive at the destination.
Old information would get injected to a newly created stream.
Shrinking the sequence window upon a stream will mitigate this scenario.
The sequence window must be shrunken to the last sequence number given to the stream.

Replay a previously unreceived PING packet in a low-flow application could be an issue.
Timestamps are sent with each PING.
The response PING replies with the same timestamp.
PINGs with mismatched timestamps should be discarded as an attempt at packet replay.


### Man-in-the-Middle
**Problem:**
Intercept communications, effectively controlling what is sent.

**Solution:**
No known perfect solution.
No solution to flow control, packet corruption, or packet injection.
Mitigate packet source validation with encryption and signatures.
Mitigate spoofing of packets with encryption.

**TRiP:**
Signatures used in OPEN handshake.
Encrypted OPEN to prevent spoofing.
Validity of connection maintained with continued encryption.
Mitigate with signatures.


### Packet Injection
**Problem:**
IP/UDP allows any source address and port to be set.
Servers reading the packets will believe they were sent from the specified source address.
Clients may connect to incorrect servers.

**Solution:**
No known solution.
Mitigated by difficulty of timing.
Mitigated with encryption and signatures.

**TRiP:**
No mitigation when signatures and encryption are not used.
Drop packets that don't make sense.
Rate limit reject messages.
Ignore unsigned REJECT messages.
Ignore unsigned CHALLENGE messages.
Ignore unsigned FORWARD messages if using unencrypted OPEN.
Disallow changing destination IP:PORT during OPEN handshake.
Established connections use encryption.
OPEN message can be encrypted.

Even when using encrypted OPEN segments there is a possibility of connecting to the wrong server
if the attacker has the OPEN key.
Let's assume the server can redirect or connect you to a different server using
FORWARD or acting as a proxy.
If the attacker knows the OPEN key,
then they can forge a packet with different routing information.
The server will be able to decrypt, and sign a different FORWARD or CHALLENGE packet.
If the CHALLENGE packet is accepted by the client,
then they are connected to the incorrect server.
This does rely on chance or the ability to block packets.
Will prevent by signing at least the route in the OPEN request.

