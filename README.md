

# C-Trip
C implementation of The River Protocol (TRP/TRiP).
TRiP is a flexible communications protocol.


## WARNINGS
* STILL BEING DESIGNED!
* The code is experimental and probably has flaws
* Currently in pre-release and will not follow semantic versioning
  until release v1.0.0
* The original author is not a security expert
* Examples will be based on UDP and has associated limitations
* If you're going to complain then detail the problem thoroughly
  and have a solution ready (if possible)


## Current Goals
* Test-able interface to ensure code quality and quick turn-around.
* Easily build custom protocols for niche applications.
* Security by default (user must disable security).
* Persistent, robust connections.
* Data transmission flexibility so behavior, and network behavior, fit the problem space.
* Real-time capabilities (disabled by default for better network performance).
* Framework verbosity to provide information to applications for better
  performance (e.g. User MTU for guaranteed single packet delivery).
* Future-proof, a programming interface and protocol that can be used over
  other mediums and a protocol that can scale up or down with the network.


## Future Goals
* Serializable connections for long disconnects.
* IoT capable through minimal client implementations to minimize build size.


## Why use TRiP?
Let's look at some existing communications protocols...

Note that every protocol suffers from packet attacks due to the insecure
nature of the internet and the Internet Protocol (IP).

TCP suffers from:
* No security by default
* Difficult to implement security
  (causes incorrect implementations or lack of security entirely, mitigated with libraries and frameworks)
* Slow handshake when doing security
  (one for the connection, one for encryption)
* Head-of-line blocking
  (degrades user and gaming experience in the browser)
* No unreliable send
* No unordered, reliable messaging
* Connection breaks if IPs change or NAT changes
  (this is unfortunate and can disrupt services)
* Dead connections kept alive by some load-balancer configurations
  (creates additional timeouts and error checking by TCP users)
* Heavy-weight solution when managing many connections to the same destination
  (usually to increase throughput, each connection suffering from the same issues)
* Requires another protocol to send messages or multiplex streams
  on the same connection
  (e.g. quirks and limitations of HTTP 1.1/2.0, WebSockets)
* Protocol specific vulnerabilities (e.g. SYN floods)

UDP suffers from:
* No security by default
* Unreliable send only
* Spoofing
* Difficulty in sending large messages/packets
* Connectionless

TRiP suffers from:
* Protocol details handled in user-space
  (some extra context switching or system calls)
* Needs another protocol on top
  (that is also part of the design)
* Potentially memory hungry? Will discover and record as needed.
  (struct sizes, tracking, message space pre-allocation)


## Implementation Notes
Outline of basic design and expectations.


### Testable Design
While C is definitely imperative, and calling router_perform may result in
mutations, the design is to make testing extremely controllable.
This is done by removing calls for the current time, system calls for a
specific packet interface (i.e. UDP), and other mutational functions.
While random functions are used internally, this shouldn't affect testing.
The pattern is to specify an action for `router_perform` and the action
is overwritten for the return value.


### Imperative Shells
The design is intended to be functional-esq, even if the router mutates
you can pretend that every time `router_perform` is called a new router
and action are returned.
Thus, to integrate this design to other frameworks you can introduce
wrapping functions and objects to create an imperative shell.


### Memory Allocation
Memory interface can be adjusted by overwriting the `libtrp_memory.h` header.


### Interface
The interface must be implemented in a generic way.
To/from destinations can be specified using UTF8 or binary strings.
This aids testing and allows the protocol to be used over any packet oriented
communication method.


### ABI
The compiled shared object should be backward compatible
taking version and implementation changes into account.
Struct sizes and details should not be made available to the users;
use handles (pointers) to reference needed resources.
Library methods will allocate the needed resources.
For embedded libraries, perhaps an additional header can be made available that
makes common structs globally available for handling one connection at a time.


### Dependencies: Library
* libhitime: Timeout management.
* libc: Standard/common functions.
* libsodium: For security. Efficient and easy-to-use encryption.


### Dependencies: Examples
* OS provided UDP socket interface.
* OS provided event watching (epoll for Linux).
* libcares (in the future): For DNS resolution.


## Build
For those unfamiliar with cmake:

        mkdir build && cd build
        cmake ..
        cmake --build .

For code coverage:

        cmake -DCODE_COVERAGE=ON ..
        cmake --build .

For installation:

        cd build
        sudo cmake --install .
        sudo ldconfig

Cleanup:

        rm -rf build



