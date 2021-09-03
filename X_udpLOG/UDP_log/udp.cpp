#include "../UDP_log/udp.h"

#include <iostream>
#include <unistd.h>
#include <stdio.h>
//#include "../CORE/c_SYST.h"
#include  <sys/socket.h>
#include <stropts.h>// I_NREAD
//#include "../EXCEPT/except.h"
#include <fstream>      // std::ifstream
using namespace std;

bool socketUDP::isClient(){
	return _isClient;
}

void socketUDP::setCaller(const string CALLER){
	_caller = CALLER;
	while (_caller.length() < 24) {
		_caller += "~";
	}
}
int socketUDP::readInt(){
	char buf[DEF_UDP_DATA_BUFLEN];
	recvBuf((char*)&buf, DEF_UDP_DATA_BUFLEN);
	//U_PRINTF(" '%s'.\n", buf);
	return atoi(buf);
}
long int socketUDP::readLongInt(){
	char buf[DEF_UDP_DATA_BUFLEN];
	recvBuf((char*)&buf, DEF_UDP_DATA_BUFLEN);
	//U_PRINTF(" '%s'.\n", buf);
	return atol(buf);
}
/*
 * TIMEOUT milliseconds
 */
socketUDP::socketUDP(const std::string NAME, int domain, int type, int protocol,
	uint16_t TIMEOUT){
	_name = NAME + ":";
	_sockfd = socket(domain, type, protocol);
	//_sockfd = -1;
	if (_sockfd < 0)
		perror("socketUDP creation failed");
	//string chn=string(__FUNCTION__) + S_int(__LINE__)+ string(__FILE__)+"\n";
	//checkError("socketUDP");
	//exit(EXIT_FAILURE);
//	DBGALL("xxx", 5, 4, "~", " :%s \n",  _name.c_str());
	/*
	 if (true)EAI_SOCKTYPE
	 _THROW(1, "socketUDP creation failed", 2);
	 */
	setRecvTimeout(TIMEOUT);
	memset(&_REMOTEADDR, 0, sizeof(_REMOTEADDR));
}
socketUDP::~socketUDP(){
	if (-1 < _sockfd)
		close(_sockfd);
}
/*
 * Used by client to set server
 */
void socketUDP::connect2server(const string IP, const int PORT){
// Filling server information
	_REMOTEADDR.sin_family = AF_INET;
	_REMOTEADDR.sin_port = htons(PORT);
	_REMOTEADDR.sin_addr.s_addr = inet_addr(IP.c_str()); //INADDR_ANY;
	if (_REMOTEADDR.sin_addr.s_addr == 0xFFFFFFFF)
		cout << "ERROR connect2server";
	else
		cout << "OK connect2server";
	// Try to connect.
	int cnx=connect(this->_sockfd, (const sockaddr*)&_REMOTEADDR, sizeof(sockaddr_in));
	if (cnx < 0) {
		perror("Connection failed to the host.");
		return;
	}
	cout << "(" << IP << ":" << PORT << ")" << flush;
	_isClient = true;
}
bool socketUDP::sendBuf(char *BUF, const uint16_t LEN, const bool checkError){
	_cntWrite++;
	int n = sendto(_sockfd, BUF, LEN, MSG_CONFIRM, (const struct sockaddr*)&_REMOTEADDR,
		sizeof(_REMOTEADDR));
	//cout << n << flush;
	if (n == LEN)
		return true;
	return false;
}
int socketUDP::recvBuf(char *BUF, const uint16_t BUFLEN, const bool checkError){
	_cntRead++;
	int n;
	_len = sizeof(_REMOTEADDR);
	n = recvfrom(_sockfd, BUF, BUFLEN, MSG_WAITALL, (struct sockaddr*)&_REMOTEADDR, &_len);
	//cout << n << flush;
	if (n == -1) {
		BUF[0] = '\0';
	}
	return n;
}
//---------------------------------------------------------------------
/**
 *
 * Reads string with the '\0' at end of the last packet to reassemble
 * The intermediate packets '\x1e' '\0' are add at the end of string
 * @param checkError
 * @return
 */
string socketUDP::readStr(const bool checkError){
	char buf[DEF_UDP_DATA_BUFLEN];
	int n = recvBuf((char*)&buf, DEF_UDP_DATA_BUFLEN, checkError);
	if (n > 0) {
		if (buf[n - 1] != '\0') {
			buf[n] = '\x1e';
			buf[n + 1] = '\0';
		}
	}
//	buf[n] = '\0';
//	U_PRINTF(" '%s'.\n", buf);
	return string(buf);
}
/**
 * @fn bool writeStr(const string TXT, const bool checkError = true)
 * @param TXT
 * @param checkError
 * @return
 */
bool socketUDP::writeStr(const string TXT, const bool checkError){
	/*
	 Because we don't want a const char * in senBuf(...) for PackID tracing !!!
	 */
	char cstr[DEF_UDP_DATA_BUFLEN];
	strcpy(cstr, TXT.c_str());

	return sendBuf(cstr, 1 + TXT.length(), checkError); // +1 for final '\0' !!!
//	U_PRINTF(" '%s'.\n", TXT.c_str());
}
void socketUDP::setRecvBufSiz(const uint32_t Size){
	size_t t1, t2;
	t1 = Size;
	t2 = sizeof(int);
	setsockopt(_sockfd, SOL_SOCKET, SO_RCVBUF, &t1, t2);
}
void socketUDP::setSendBufSiz(const uint32_t Size){
	size_t t1, t2;
	t1 = Size;
	t2 = sizeof(int);
	setsockopt(_sockfd, SOL_SOCKET, SO_SNDBUF, &t1, t2);
}
void socketUDP::end(){
	close(_sockfd);
}
void socketUDP::setRecvTimeout(uint32_t TIMEOUT){
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000 * TIMEOUT;
	setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}
void socketUDP::clear(const int TIMEOUT_MILLIS){
	setRecvTimeout(0);
	_len = sizeof(_REMOTEADDR);
	bool done = false;
	while (!done) {
		char BUF[DEF_UDP_DATA_BUFLEN];
		int n = recvfrom(_sockfd, BUF, DEF_UDP_DATA_BUFLEN, MSG_WAITALL,
			(struct sockaddr*)&_REMOTEADDR, &_len);
		done = n <= 0;
	}
	setRecvTimeout(TIMEOUT_MILLIS);
}
/*
 int socketUDP::checkError(const string CHN, const int ERR, const int LVL) {
 socklen_t errcod;
 //socklen_t * err=&errcod;
 socklen_t errsize = sizeof(errcod);
 if (getsockopt(_sockfd, SOL_SOCKET, SO_ERROR, (&errcod), &errsize) != 0) {
 // erreur lors de la recuperation d'erreur…
 std::cout << "Erreur lors de la determination de l'erreur : "
 << std::endl;
 //<< Sockets::GetError()
 }
 int err;
 if (ERR == 0)
 err = errno;
 else
 err = ERR;
 //	return err;
 if ((err != 0) && (err != 22)) {
 switch (err) {
 case BUF_WRITE_NOCHAR:
 _THROW(BUF_WRITE_NOCHAR, _name + CHN);
 break;
 case EAI_SOCKTYPE:
 _THROW(EAI_SOCKTYPE, _name + CHN);
 break;
 case -1:
 _THROW(CHAR_READ_TIMEOUT, _name + CHN);
 break;
 case 22: //EINVAL:
 U_PRINTF(
 "%s%s FATAL ERROR:%i (An incorrect argument was specified. Check the validity of function parameters.\n",
 _name.c_str(), CHN.c_str(), err);
 exit(255);
 break;
 default:
 U_PRINTF("%s%s FATAL ERROR %i (see upd unit for explanation code.\n",
 _name.c_str(), CHN.c_str(), err);
 exit(255);
 }
 }
 return err;
 }
 */
/*

 SO_DEBUG
 Reports whether debugging information is being recorded. This option shall store an int value. This is a Boolean option.
 SO_ACCEPTCONN
 Reports whether socket listening is enabled. This option shall store an int value. This is a Boolean option.
 SO_BROADCAST
 Reports whether transmission of broadcast messages is supported, if this is supported by the protocol. This option shall store an int value. This is a Boolean option.
 SO_REUSEADDR
 Reports whether the rules used in validating addresses supplied to bind() should allow reuse of local addresses, if this is supported by the protocol. This option shall store an int value. This is a Boolean option.
 SO_KEEPALIVE
 Reports whether connections are kept active with periodic transmission of messages, if this is supported by the protocol.

 If the connected socket fails to respond to these messages, the connection shall be broken and threads writing to that socket shall be notified with a SIGPIPE signal. This option shall store an int value. This is a Boolean option.
 SO_LINGER
 Reports whether the socket lingers on close() if data is present. If SO_LINGER is set, the system shall block the calling thread during close() until it can transmit the data or until the end of the interval indicated by the l_linger member, whichever comes first. If SO_LINGER is not specified, and close() is issued, the system handles the call in a way that allows the calling thread to continue as quickly as possible. This option shall store a linger structure.
 SO_OOBINLINE
 Reports whether the socket leaves received out-of-band data (data marked urgent) inline. This option shall store an int value. This is a Boolean option.
 SO_SNDBUF
 Reports send buffer size information. This option shall store an int value.
 SO_RCVBUF
 Reports receive buffer size information. This option shall store an int value.
 SO_ERROR
 Reports information about error status and clears it. This option shall store an int value.
 SO_TYPE
 Reports the socket type. This option shall store an int value. Socket types are described in Socket Types.
 SO_DONTROUTE
 Reports whether outgoing messages bypass the standard routing facilities. The destination shall be on a directly-connected network, and messages are directed to the appropriate network interface according to the destination address. The effect, if any, of this option depends on what protocol is in use. This option shall store an int value. This is a Boolean option.
 SO_RCVLOWAT
 Reports the minimum number of bytes to process for socket input operations. The default value for SO_RCVLOWAT is 1. If SO_RCVLOWAT is set to a larger value, blocking receive calls normally wait until they have received the smaller of the low water mark value or the requested amount. (They may return less than the low water mark if an error occurs, a signal is caught, or the type of data next in the receive_queue is different from that returned; for example, out-of-band data.) This option shall store an int value. Note that not all implementations allow this option to be retrieved.
 SO_RCVTIMEO
 Reports the timeout value for input operations. This option shall store a timeval structure with the number of seconds and microseconds specifying the limit on how long to wait for an input operation to complete. If a receive operation has blocked for this much time without receiving additional data, it shall return with a partial count or errno set to [EAGAIN] or [EWOULDBLOCK] if no data was received. The default for this option is zero, which indicates that a receive operation shall not time out. Note that not all implementations allow this option to be retrieved.
 SO_SNDLOWAT
 Reports the minimum number of bytes to process for socket output operations. Non-blocking output operations shall process no data if flow control does not allow the smaller of the send low water mark value or the entire request to be processed. This option shall store an int value. Note that not all implementations allow this option to be retrieved.
 SO_SNDTIMEO
 Reports the timeout value specifying the amount of time that an output function blocks because flow control prevents data from being sent. If a send operation has blocked for this time, it shall return with a partial count or with errno set to [EAGAIN] or [EWOULDBLOCK] if no data was sent. The default for this option is zero, which indicates that a send operation shall not time out. The option shall store a timeval structure. Note that not all implementations allow this option to be retrieved.

 For Boolean options, a zero value indicates that the option is disabled and a non-zero value indicates that the option is enabled.
 */
/*
 https://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.halc001/syserret.htm
 Sockets return codes (ERRNOs)

 z/OS Communications Server: IP CICS Sockets Guide
 SC27-3649-00

 This section provides the system-wide message numbers and codes set by the system calls. These message numbers and codes are in the TCPERRNO.H include file supplied with TCP/IP Services.
 Table 1. Sockets ERRNOsError number 	Message name

 Socket
 API type
 Error 								description 				Programmer's response
 1 	EAI_NONAME 	GETADDRINFO GETNAMEINFO NODE or HOST cannot be found. 	Ensure the NODE or HOST name can be resolved.
 1 	EDOM 		All 					Argument too large. 	Check parameter values of the function call.
 1 	EPERM 		All 					Permission is denied. No owner exists. 	Check that TPC/IP is still active; check protocol value of socket () call.
 1 	EPERM 	IOCTL (SIOCGPARTNERINFO) 	Both endpoints do not reside in the same security domain. 	Check and modify the security domain name for the endpoints. After you correct the security domain name, the application might need to close the connection if the IOCTL is needed.
 1 	EPERM 	IOCTL (SIOCGPARTNERINFO, SIOCSPARTNERINFO) 	The security domain name is not defined. 	Define the security domain name on both endpoints. After you define the security domain name, the application might need to close the connection if the IOCTL is needed.
 1 	EPERM 	IOCTL (SIOCTTLSCTL requesting both TTLS_INIT_ CONNECTION and TTLS_RESET_ SESSION or both TTLS_INIT_ CONNECTION and TTLS_RESET_ CIPHER) 	The combination of requests specified is not permitted. 	Request TTLS_RESET_SESSION and TTLS_RESET_CIPHER only when TTLS_INIT_ CONNECTION has been previously requested for the connection.
 1 	EPERM 	IOCTL (SIOCTTLSCTL) 	Denotes one of the following error conditions:

 The TTLS_INIT_ CONNECTION option was requested with either TTLS_RESET_ SESSION, TTLS_RESET_ CIPHER or TTLS_STOP_ CONNECTION
 The TTLS_STOP_ CONNECTION option was requested along with TTLS_RESET_ SESSION or TTLS_RESET_ CIPHER
 The TTLS_ALLOW_ HSTIMEOUT option was requested without TTLS_INIT_ CONNECTION

 Request TTLS_RESET_SESSION and TTLS_RESET_CIPHER only when TTLS_INIT_ CONNECTION and TTLS_STOP_ CONNECTION are not requested. Always request TTLS_INIT_ CONNECTION when TTLS_ALLOW_ HSTIMEOUT is requested. Use seperate SIOCTTLSCTL ioctls to request TTLS_INIT_ CONNECTION and TTLS_STOP_ CONNECTION.
 2 	EAI_AGAIN 	FREEADDRINFO GETADDRINFO GETNAMEINFO 	For GETADDRINFO, NODE could not be resolved within the configured time interval. For GETNAMEINFO, HOST could not be resolved within the configured time interval. The Resolver address space has not been started. The request can be retried later. 	Ensure the Resolver is active, then retry the request.
 2 	ENOENT 	All 	The data set or directory was not found. 	Check files used by the function call.
 2 	ERANGE 	All 	The result is too large. 	Check parameter values of the function call.
 3 	EAI_FAIL 	FREEADDRINFO GETADDRINFO GETNAMEINFO 	This is an unrecoverable error. NODELEN, HOSTLEN, or SERVLEN is incorrect. For FREEADDRINFO, the resolver storage does not exist. 	Correct the NODELEN, HOSTLEN, or SERVLEN. Otherwise, call your system administrator.
 3 	ESRCH 	All 	The process was not found. A table entry was not located. 	Check parameter values and structures pointed to by the function parameters.
 4 	EAI_OVERFLOW 	GETNAMEINFO 	The output buffer for the host name or service name was too small. 	Increase the size of the buffer to 255 characters, which is the maximum size permitted.
 4 	EINTR 	All 	A system call was interrupted. 	Check that the socket connection and TCP/IP are still active.
 5 	EAI_FAMILY 	GETADDRINFO GETNAMEINFO 	The AF or the FAMILY is incorrect. 	Correct the AF or the FAMILY.
 5 	EIO 	All 	An I/O error occurred. 	Check status and contents of source database if this occurred during a file access.
 6 	EAI_MEMORY 	GETADDRINFO GETNAMEINFO 	The resolver cannot obtain storage to process the host name. 	Contact your system administrator.
 6 	ENXIO 	All 	The device or driver was not found. 	Check status of the device attempting to access.
 7 	E2BIG 	All 	The argument list is too long. 	Check the number of function parameters.
 7 	EAI_BADFLAGS 	GETADDRINFO GETNAMEINFO 	FLAGS has an incorrect value. 	Correct the FLAGS.
 8 	EAI_SERVICE 	GETADDRINFO 	The SERVICE was not recognized for the specified socket type. 	Correct the SERVICE.
 8 	ENOEXEC 	All 	An EXEC format error occurred. 	Check that the target module on an exec call is a valid executable module.
 9 	EAI_SOCKTYPE 	GETADDRINFO 	The SOCTYPE was not recognized. 	Correct the SOCTYPE.
 9 	EBADF 	All 	An incorrect socket descriptor was specified. 	Check socket descriptor value. It might be currently not in use or incorrect.
 9 	EBADF 	Givesocket 	The socket has already been given. The socket domain is not AF_INET or AF_INET6. 	Check the validity of function parameters.
 9 	EBADF 	Select 	One of the specified descriptor sets is an incorrect socket descriptor. 	Check the validity of function parameters.
 9 	EBADF 	Takesocket 	The socket has already been taken. 	Check the validity of function parameters.
 9 	EAI_SOCKTYPE 	GETADDRINFO 	The SOCTYPE was not recognized. 	Correct the SOCTYPE.
 10 	ECHILD 	All 	There are no children. 	Check if created subtasks still exist.
 11 	EAGAIN 	All 	There are no more processes. 	Retry the operation. Data or condition might not be available at this time.
 11 	EAGAIN 	All 	TCP/IP is not active at the time of the request. 	Start TCP/IP, and retry the request.
 11 	EAGAIN 	IOCTL (SIOCGPARTNERINFO) 	The IOCTL was issued in no-suspend mode and the SIOCSPARTNERINFO IOCTL has not been issued. 	Reissue the IOCTL with a timeout value to set the amount of time to wait while the partner security credentials are being retrieved.

 Restriction: You cannot use a select mask to determine when an IOCTL is complete, because an IOCTL is not affected by whether the socket is running in blocking or nonblocking mode. If the IOCTL times out, reissue the IOCTL to retrieve the partner security credentials.
 12 	ENOMEM 	All 	There is not enough storage. 	Check the validity of function parameters.
 13 	EACCES 	All 	Permission denied, caller not authorized. 	Check access authority of file.
 13 	EACCES 	IOCTL (SIOCGPARTNERINFO) 	The application is not running in supervisor state, is not APF authorized, or is not permitted to the appropriate SERVAUTH profile. 	Allow the application to issue this IOCTL, or provide the user ID with the proper SERVAUTH permission.
 13 	EACCES 	IOCTL (SIOCTTLSCTL) 	The IOCTL is requesting a function that requires that the socket be mapped to policy that specifies ApplicationControlled On. 	Check policy and add ApplicationControlled On if the application should be permitted to issue the controlled SIOCTTLSCTL functions.
 13 	EACCES 	Takesocket 	The other application (listener) did not give the socket to your application. Permission denied, caller not authorized. 	Check access authority of file.
 14 	EFAULT 	All 	An incorrect storage address or length was specified. 	Check the validity of function parameters.
 14 	EFAULT 	All EZASMI macros when using an asynchronous exit routine. 	The exit routine has abnormally ended (ABEND condition). 	Correct the error in the routine's code. Add an ESTAE routine to the exit.
 14 	EFAULT 	IOCTL (SIOCSAPPLDATA) 	An abend occurred while attempting to copy the SetADcontainer structure from the address provided in the SetAD_ptr field. 	Check the validity of function parameters.
 15 	ENOTBLK 	All 	A block device is required. 	Check device status and characteristics.
 16 	EBUSY 	All 	Listen has already been called for this socket. Device or file to be accessed is busy. 	Check if the device or file is in use.
 17 	EEXIST 	All 	The data set exists. 	Remove or rename existing file.
 18 	EXDEV 	All 	This is a cross-device link. A link to a file on another file system was attempted. 	Check file permissions.
 19 	ENODEV 	All 	The specified device does not exist. 	Check file name and if it exists.
 20 	ENOTDIR 	All 	The specified directory is not a directory. 	Use a valid file that is a directory.
 21 	EISDIR 	All 	The specified directory is a directory. 	Use a valid file that is not a directory.
 22 	EINVAL 	All types 	An incorrect argument was specified. 	Check the validity of function parameters.
 22 	EINVAL 	Multicast Source filter APIs 	Mix of any-source, source-specific or full-state APIs 	Specify the correct type of APIs.
 22 	EINVAL 	MCAST_JOIN_GROUP, MCAST_JOIN_SOURCE_ GROUP, MCAST_BLOCK_SOURCE, MCAST_LEAVE_GROUP, MCAST_LEAVE_SOURCE_ GROUP, MCAST_UNBLOCK_ SOURCE, SIOCGMSFILTER, SIOCSMSFILTER 	The socket address family or the socket length of the input multicast group or the source IP address is not correct. 	Specify the correct value.
 22 	EINVAL 	SIOCSMSFILTER, SIOCSIPMSFILTER 	The specified filter mode is not correct. 	Specify the correct value.
 23 	ENFILE 	All 	Data set table overflow occurred. 	Reduce the number of open files.
 24 	EMFILE 	All 	The socket descriptor table is full. 	Check the maximum sockets specified in MAXDESC().
 25 	ENOTTY 	All 	An incorrect device call was specified. 	Check specified IOCTL() values.
 26 	ETXTBSY 	All 	A text data set is busy. 	Check the current use of the file.
 27 	EFBIG 	All 	The specified data set is too large. 	Check size of accessed dataset.
 28 	ENOSPC 	All 	There is no space left on the device. 	Increase the size of accessed file.
 29 	ESPIPE 	All 	An incorrect seek was attempted. 	Check the offset parameter for seek operation.
 30 	EROFS 	All 	The data set system is Read only. 	Access data set for read only operation.
 31 	EMLINK 	All 	There are too many links. 	Reduce the number of links to the accessed file.
 32 	EPIPE 	All 	The connection is broken. For socket write/send, peer has shut down one or both directions. 	Reconnect with the peer.
 32 	EPIPE 	IOCTL (SIOCTTLSCTL requesting TTLS_INIT_ CONNECTION, TTLS_RESET_CIPHER, or TTLS_STOP_ CONNECTION) 	The TCP connection is not in the established state. 	Issue the SIOCTTLSCTL IOCTL when the socket is connected.
 33 	EDOM 	All 	The specified argument is too large. 	Check and correct function parameters.
 34 	ERANGE 	All 	The result is too large. 	Check function parameter values.
 35 	EWOULDBLOCK 	Accept 	The socket is in nonblocking mode and connections are not queued. This is not an error condition. 	Reissue Accept().
 35 	EWOULDBLOCK 	IOCTL (SIOCTTLSCTL) 	The handshake is in progress and the socket is a nonblocking socket. 	For a nonblocking socket, you can wait for the handshake to complete by issuing Select or Poll for Socket Writable.
 35 	EWOULDBLOCK 	Read Recvfrom 	The socket is in nonblocking mode and read data is not available. This is not an error condition. 	Issue a select on the socket to determine when data is available to be read or reissue the Read()/Recvfrom().
 35 	EWOULDBLOCK 	All receive calls (RECV, RECVMSG, RECVFROM, READV, READ), when the socket is set with the SO_RCVTIMEO socket option 	The socket is in blocking mode and the receive call has blocked for the time period that was specified in the SO_RCVTIMEO option. No data was received. 	The application should reissue the receive call.
 35 	EWOULDBLOCK 	Send Sendto Write 	The socket is in nonblocking mode and buffers are not available. 	Issue a select on the socket to determine when data is available to be written or reissue the Send(), Sendto(), or Write().
 35 	EWOULDBLOCK 	All send calls (SEND, SENDMSG, SENDTO, WRITEV, WRITE), when the socket is set with the SO_SNDTIMEO socket option 	The socket is in blocking mode and the send call has blocked for the time period that was specified in the SO_SNDTIMEO option. No data was sent. 	The application should reissue the send call.
 36 	EINPROGRESS 	Connect 	The socket is marked nonblocking and the connection cannot be completed immediately. This is not an error condition. 	See the Connect() description for possible responses.
 36 	EINPROGRESS 	IOCTL (SIOCGPARTNERINFO) 	The IOCTL was issued in no-suspend mode after the SIOCSPARTNERINFO IOCTL was issued, but the partner security credentials are not currently available. 	Retry the IOCTL, or issue the IOCTL with a timeout value to set the amount of time to wait while the partner security credentials are being retrieved.

 Restriction: You cannot use a select mask to determine when an IOCTL is complete, because an IOCTL is not affected by whether the socket is running in blocking or nonblocking mode. If the IOCTL times out, reissue the IOCTL to retrieve the partner security credentials.
 36 	EINPROGRESS 	IOCTL (SIOCTTLSCTL requesting TTLS_INIT_ CONNECTION or TTLS_STOP_ CONNECTION) 	The handshake is already in progress and the socket is a nonblocking socket. 	For a nonblocking socket, you can wait for the handshake to complete by issuing Select or Poll for Socket Writable.
 37 	EALREADY 	Connect 	The socket is marked nonblocking and the previous connection has not been completed. 	Reissue Connect().
 37 	EALREADY 	IOCTL (SIOCGPARTNERINFO) 	The request is already in progress. Only one IOCTL can be outstanding. 	Check and modify the socket descriptor, if specified; otherwise, no action is needed.
 37 	EALREADY 	IOCTL (SIOCTTLSCTL requesting TTLS_INIT_ CONNECTION or TTLS_STOP_ CONNECTION) 	For TTLS_INIT_ CONNECTION, the socket is already secure. For TTLS_STOP_ CONNECTION, the socket is not secure. 	Modify the application so that it issues the SIOCTTLSCTL IOCTL that requests TTLS_INIT_ CONNECTION only when the socket is not already secure and that requests TTLS_STOP_ CONNECTION only when the socket is secure.
 37 	EALREADY 	Maxdesc 	A socket has already been created calling Maxdesc() or multiple calls to Maxdesc(). 	Issue Getablesize() to query it.
 37 	EALREADY 	Setibmopt 	A connection already exists to a TCP/IP image. A call to SETIBMOPT (IBMTCP_IMAGE), has already been made. 	Call Setibmopt() only once.
 38 	ENOTSOCK 	All 	A socket operation was requested on a nonsocket connection. The value for socket descriptor was not valid. 	Correct the socket descriptor value and reissue the function call.
 39 	EDESTADDRREQ 	All 	A destination address is required. 	Fill in the destination field in the correct parameter and reissue the function call.
 40 	EMSGSIZE 	Sendto Sendmsg Send Write for Datagram (jctUDP) or RAW sockets 	The message is too long. It exceeds the IP limit of 64K or the limit set by the setsockopt() call. 	Either correct the length parameter, or send the message in smaller pieces.
 41 	EPROTOTYPE 	All 	The specified protocol type is incorrect for this socket. 	Correct the protocol type parameter.
 41 	EPROTOTYPE 	bind2addrsel 	The referenced socket is not a stream (TCP) or datagram (jctUDP) socket. 	Issue bind2addrsel() on TCP or jctUDP sockets only.
 41 	EPROTOTYPE 	IOCTL (SIOCGPARTNERINFO, SIOCSAPPLDATA, SIOCSPARTNERINFO, SIOCTTLSCTL) 	Socket is not a TCP socket. 	Issue the IOCTL on TCP sockets only.
 42 	ENOPROTOOPT 	Getsockopt Setsockopt 	The socket option specified is incorrect or the level is not SOL_SOCKET. Either the level or the specified optname is not supported. 	Correct the level or optname.
 42 	ENOPROTOOPT 	Getibmsockopt Setibmsockopt 	Either the level or the specified optname is not supported. 	Correct the level or optname.
 43 	EPROTONOSUPPORT 	Socket 	The specified protocol is not supported. 	Correct the protocol parameter.
 44 	ESOCKTNOSUPPORT 	All 	The specified socket type is not supported. 	Correct the socket type parameter.
 45 	EOPNOTSUPP 	Accept Givesocket 	The selected socket is not a stream socket. 	Use a valid socket.
 45 	EOPNOTSUPP 	bind2addrsel 	The referenced socket is not a type that supports the requested function 	Use a socket of the correct type.
 45 	EOPNOTSUPP 	Getibmopt Setibmopt 	The socket does not support this function call. This command is not supported for this function. 	Correct the command parameter. See Getibmopt() for valid commands. Correct by ensuring a Listen() was not issued before the Connect().
 45 	EOPNOTSUPP 	GETSOCKOPT 	The specified GETSOCKOPT OPTNAME option is not supported by this socket API. 	Correct the GETSOCKOPT OPTNAME option.
 45 	EOPNOTSUPP 	IOCTL 	The specified IOCTL command is not supported by this socket API. 	Correct the IOCTL COMMAND.
 45 	EOPNOTSUPP 	IOCTL (SIOCSPARTNERINFO) 	The request must be issued before the listen call or the connect call. 	Check and modify the socket descriptor, or close the connection and reissue the call.
 45 	EOPNOTSUPP 	IOCTL (SIOCTTLSCTL requesting TTLS_INIT_ CONNECTION, TTLS_RESET_ SESSION, TTLS_RESET_ CIPHER or TTLS_STOP_ CONNECTION) 	Mapped policy indicates that AT-TLS is not enabled for the connection. 	Modify the policy to enable AT-TLS for the connection.
 45 	EOPNOTSUPP 	Listen 	The socket does not support the Listen call. 	Change the type on the Socket() call when the socket was created. Listen() supports only a socket type of SOCK_STREAM.
 45 	EOPNOTSUPP 	RECV, RECVFROM, RECVMSG, SEND, SENDTO, SENDMSG 	The specified flags are not supported on this socket type or protocol. 	Correct the FLAG.
 46 	EPFNOSUPPORT 	All 	The specified protocol family is not supported or the specified domain for the client identifier is not AF_INET=2. 	Correct the protocol family.
 47 	EAFNOSUPPORT 	bind2addrsel inet6_is_srcaddr 	You specified an IP address that is not an AF_INET6 IP address 	Correct the IP address. If the IP address is an IPv4 address, you must specify it as an IPv4-mapped IPv6 address.
 47 	EAFNOSUPPORT 	bind2addrsel inet6_is_srcaddr 	You attempted an IPv6-only API for a stack that does not support the AF_INET6 domain. 	Activate the AF_INET6 stack, and retry the request.
 47 	EAFNOSUPPORT 	Bind Connect Socket 	The specified address family is not supported by this protocol family. 	For Socket(), set the domain parameter to AF_INET. For Bind() and Connect(), set Sin_Family in the socket address structure to AF_INET.
 47 	EAFNOSUPPORT 	Getclient Givesocket 	The socket specified by the socket descriptor parameter was not created in the AF_INET domain. 	The Socket() call used to create the socket should be changed to use AF_INET for the domain parameter.
 47 	EAFNOSUPPORT 	IOCTL 	You attempted to use an IPv4-only ioctl on an AF_INET6 socket. 	Use the correct socket type for the ioctl or use an ioctl that supports AF_INET6 sockets.
 48 	EADDRINUSE 	Bind 	The address is in a timed wait because a LINGER delay from a previous close or another process is using the address. This error can also occur if the port specified in the bind call has been configured as RESERVED on a port reservation statement in the TCP/IP profile. 	If you want to reuse the same address, use Setsockopt() with SO_REUSEADDR. Refer to the section about Setsockopt() in z/OS Communications Server: IP Sockets Application Programming Interface Guide and Reference for more information. Otherwise, use a different address or port in the socket address structure.
 48 	EADDRINUSE 	IP_ADD_MEMBERSHIP, IP_ADD_SOURCE_ MEMBERSHIP, IPV6_JOIN_GROUP, MCAST_JOIN_GROUP, MCAST_JOIN_SOURCE_ GROUP 	The specified multicast address and interface address (or interface index) pair is already in use. 	Correct the specified multicast address, interface address, or interface index.
 49 	EADDRNOTAVAIL 	Bind 	The specified address is incorrect for this host. 	Correct the function address parameter.
 49 	EADDRNOTAVAIL 	Connect 	The calling host cannot reach the specified destination. 	Correct the function address parameter.
 49 	EADDRNOTAVAIL 	bind2addrsel 	For the specified destination address, there is no source address that the application can bind to. Possible reasons can be one of the following situations:

 The socket is a stream socket, but the specified destination address is a multicast address.
 No ephemeral ports are available to assign to the socket.

 Correct the function address parameter or issue the request when ephemeral ports are available.
 49 	EADDRNOTAVAIL 	inet6_is_srcaddr 	The address specified is not correct for one of these reasons:

 The address is not an address on this node.
 The address was not active at the time of the request.
 The scope ID specified for a link-local IPV6 address is incorrect.

 Correct or activate the address
 49 	EADDRNOTAVAIL 	IP_BLOCK_SOURCE, IP_ADD_SOURCE_ MEMBERSHIP, MCAST_BLOCK_SOURCE, MCAST_JOIN_SOURCE_ GROUP 	A duplicate source IP address is specified on the multicast group and interface pair. 	Correct the specified source IP address.
 49 	EADDRNOTAVAIL 	IP_UNBLOCK_SOURCE, IP_DROP_SOURCE_ MEMBERSHIP, MCAST_UNBLOCK_ SOURCE, MCAST_LEAVE_SOURCE_ GROUP 	A previously blocked source multicast group cannot be found. 	Correct the specified address.
 49 	EADDRNOTAVAIL 	Multicast APIs 	The specified multicast address, interface address, or interface index is not correct. 	Correct the specified address.
 50 	ENETDOWN 	All 	The network is down. 	Retry when the connection path is up.
 51 	ENETUNREACH 	Connect 	The network cannot be reached. 	Ensure that the target application is active.
 52 	ENETRESET 	All 	The network dropped a connection on a reset. 	Reestablish the connection between the applications.
 53 	ECONNABORTED 	All 	The software caused a connection abend. 	Reestablish the connection between the applications.
 54 	ECONNRESET 	All 	The connection to the destination host is not available. 	N/A
 54 	ECONNRESET 	Send Write 	The connection to the destination host is not available. 	The socket is closing. Issue Send() or Write() before closing the socket.
 55 	ENOBUFS 	All 	No buffer space is available. 	Check the application for massive storage allocation call.
 55 	ENOBUFS 	Accept 	Not enough buffer space is available to create the new socket. 	Call your system administrator.
 55 	ENOBUFS 	IOCTL (SIOCGPARTNERINFO) 	The buffer size provided is too small. 	Create a larger input buffer based on the value returned in the PI_Buflen field.
 55 	ENOBUFS 	IOCTL (SIOCSAPPLDATA) 	There is no storage available to store the associated data. 	Call your system administrator.
 55 	ENOBUFS 	IOCTL (SIOCTTLSCTL TTLS_Version1 requesting TTLS_RETURN_ CERTIFICATE or TTLS_Version2 query) 	The buffer size provided is too small. 	For TTLS_Version1 use the returned certificate length to allocate a larger buffer and reissue IOCTL with the larger buffer.
 55 	ENOBUFS 	IP_BLOCK_SOURCE, IP_ADD_SOURCE_ MEMBERSHIP, MCAST_BLOCK_SOURCE, MCAST_JOIN_SOURCE_ GROUP, SIOCSIPMSFILTER, SIOCSMSFILTER, setipv4sourcefilter, setsourcefilter 	A maximum of 64 source filters can be specified per multicast address, interface address pair. 	Remove unneeded source IP addresses and reissue the command.
 55 	ENOBUFS 	Send Sendto Write 	Not enough buffer space is available to send the new message. 	Call your system administrator.
 55 	ENOBUFS 	Takesocket 	Not enough buffer space is available to create the new socket. 	Call your system administrator.
 56 	EISCONN 	Connect 	The socket is already connected. 	Correct the socket descriptor on Connect() or do not issue a Connect() twice for the socket.
 57 	ENOTCONN 	All 	The socket is not connected. 	Connect the socket before communicating.
 57 	ENOTCONN 	IOCTL (SIOCGPARTNERINFO) 	The requested socket is not connected. 	Check and modify the socket descriptor, or reissue the IOCTL after the connect call from the client side or after the accept call from the server side.
 57 	ENOTCONN 	IOCTL (SIOCTTLSCTL) 	The socket is not connected. 	Issue the SIOCTTLSCTL IOCTL only after the socket is connected.
 58 	ESHUTDOWN 	All 	A Send cannot be processed after socket shutdown. 	Issue read/receive before shutting down the read side of the socket.
 59 	ETOOMANYREFS 	All 	There are too many references. A splice cannot be completed. 	Call your system administrator.
 59 	ETOOMANYREFS 	IP_ADD_MEMBERSHIP, IP_ADD_SOURCE_ MEMBERSHIP, MCAST_JOIN_GROUP, MCAST_JOIN_SOURCE_ GROUP, IPV6_JOIN_GROUP 	A maximum of 20 multicast groups per single jctUDP socket or a maximum of 256 multicast groups per single RAW socket can be specified. 	Remove unneeded multicast groups and reissue the command.
 60 	ETIMEDOUT 	Connect 	The connection timed out before it was completed. 	Ensure the server application is available.
 61 	ECONNREFUSED 	Connect 	The requested connection was refused. 	Ensure server application is available and at specified port.
 62 	ELOOP 	All 	There are too many symbolic loop levels. 	Reduce symbolic links to specified file.
 63 	ENAMETOOLONG 	All 	The file name is too long. 	Reduce size of specified file name.
 64 	EHOSTDOWN 	All 	The host is down. 	Restart specified host.
 65 	EHOSTUNREACH 	All 	There is no route to the host. 	Set up network path to specified host and verify that host name is valid.
 66 	ENOTEMPTY 	All 	The directory is not empty. 	Clear out specified directory and reissue call.
 67 	EPROCLIM 	All 	There are too many processes in the system. 	Decrease the number of processes or increase the process limit.
 68 	EUSERS 	All 	There are too many users on the system. 	Decrease the number of users or increase the user limit.
 69 	EDQUOT 	All 	The disk quota has been exceeded. 	Call your system administrator.
 70 	ESTALE 	All 	An old NFS** data set handle was found. 	Call your system administrator.
 71 	EREMOTE 	All 	There are too many levels of remote in the path. 	Call your system administrator.
 72 	ENOSTR 	All 	The device is not a stream device. 	Call your system administrator.
 73 	ETIME 	All 	The timer has expired. 	Increase timer values or reissue function.
 73 	ETIME 	IOCTL (SIOCGPARTNERINFO) 	The wait time for the request has expired, possibly as the result of network problems. 	Retry the request.

 Restriction: You cannot use a select mask to determine when an IOCTL is complete, because an IOCTL is not affected by whether the socket is running in blocking or nonblocking mode. If the IOCTL times out, reissue the IOCTL to retrieve the partner security credentials.
 74 	ENOSR 	All 	There are no more stream resources. 	Call your system administrator.
 75 	ENOMSG 	All 	There is no message of the desired type. 	Call your system administrator.
 76 	EBADMSG 	All 	The system cannot read the message. 	Verify that z/OS® Communications Server installation was successful and that message files were properly loaded.
 77 	EIDRM 	All 	The identifier has been removed. 	Call your system administrator.
 78 	EDEADLK 	All 	A deadlock condition has occurred. 	Call your system administrator.
 78 	EDEADLK 	Select Selectex 	None of the sockets in the socket descriptor sets are either AF_INET or AF_IUCV sockets and there is no timeout value or no ECB specified. The select/selectex would never complete. 	Correct the socket descriptor sets so that an AF_INET or AF_IUCV socket is specified. A timeout or ECB value can also be added to avoid the select/selectex from waiting indefinitely.
 79 	ENOLCK 	All 	No record locks are available. 	Call your system administrator.
 80 	ENONET 	All 	The requested machine is not on the network. 	Call your system administrator.
 81 	ERREMOTE 	All 	The object is remote. 	Call your system administrator.
 82 	ENOLINK 	All 	The link has been severed. 	Release the sockets and reinitialize the client-server connection.
 83 	EADV 	All 	An ADVERTISE error has occurred. 	Call your system administrator.
 84 	ESRMNT 	All 	An SRMOUNT error has occurred. 	Call your system administrator.
 85 	ECOMM 	All 	A communication error has occurred on a Send call. 	Call your system administrator.
 86 	EPROTO 	All 	A protocol error has occurred. 	Call your system administrator.
 86 	EPROTO 	IOCTL (SIOCTTLSCTL request in TTLS_RESET_SESSION, TTLS_RESET_CIPHER, TTLS_STOP_CONNECTION, or TTLS_ALLOW_HSTIMEOUT) 	One of the following errors occurred:

 A TTLS_INIT_CONNECTION request was not received for the connection.
 TTLS_STOP_CONNECTION was requested on a connection that has outstanding application data. For unread application data, the errno junior is JrTTLSStopReadDataPending. For unwritten application data, the errno junior is JrTTLSStopWriteDataPending.
 TTLS_RESET_CIPHER or TTLS_STOP_ CIPHER was requested on a connection that is secured using SSL version 2.
 TTLS_ALLOW_HSTIMEOUT was requested but the policy has the HandshakeRole value client or the HandshakeTimeout value is 0.



 Request TTLS_INIT_CONNECTION before requesting TTLS_RESET_SESSION or TTLS_RESET_CIPHER.
 Request TTLS_STOP_CONNECTION after all application data is cleared from the connection. For JrTTLSStopReadDataPending, read all available application data. For JrTTLSStopWriteDataPending, wait for all the outstanding application data to be written.
 Request TTLS_RESET_CIPHER or TTLS_STOP_CONNECTION only on connections secured using SSL version 3 or TLS version 1.0 or higher.
 Request TTLS_ALLOW_HSTIMEOUT only when the security type is TTLS_SEC_SERVER or higher and the HandshakeTimeout value is not 0.

 87 	EMULTIHOP 	All 	A multi-hop address link was attempted. 	Call your system administrator.
 88 	EDOTDOT 	All 	A cross-mount point was detected. This is not an error. 	Call your system administrator.
 89 	EREMCHG 	All 	The remote address has changed. 	Call your system administrator.
 90 	ECONNCLOSED 	All 	The connection was closed by a peer. 	Check that the peer is running.
 113 	EBADF 	All 	Socket descriptor is not in correct range. The maximum number of socket descriptors is set by MAXDESC(). The default range is 0–49. 	Reissue function with corrected socket descriptor.
 113 	EBADF 	Bind socket 	The socket descriptor is already being used. 	Correct the socket descriptor.
 113 	EBADF 	Givesocket 	The socket has already been given. The socket domain is not AF_INET. 	Correct the socket descriptor.
 113 	EBADF 	Select 	One of the specified descriptor sets is an incorrect socket descriptor. 	Correct the socket descriptor. Set on Select() or Selectex().
 113 	EBADF 	Takesocket 	The socket has already been taken. 	Correct the socket descriptor.
 113 	EBADF 	Accept 	A Listen() has not been issued before the Accept(). 	Issue Listen() before Accept().
 121 	EINVAL 	All 	An incorrect argument was specified. 	Check and correct all function parameters.
 121 	EINVAL 	IOCTL (SIOCSAPPLDATA) 	The input parameter is not a correctly formatted SetApplData structure.

 The SetAD_eye1 value is not valid.
 The SetAD_ver value is not valid.
 The storage pointed to by SetAD_ptr does not contain a correctly formatted SetADcontainer structure.
 The SetAD_eye2 value is not valid.
 The SetAD_len value contains an incorrect length for the SetAD_ver version of the SetADcontainer structure.

 Check and correct all function parameters.
 121 	EINVAL 	inet6_is_srcaddr

 One or more invalid IPV6_ADDR_ PREFERENCES flags were specified
 A scope ID was omitted for a link local IP address
 A scope ID was specified for an IP address that is not link-local
 The socket address length was not valid

 Correct the function parameters
 122 	ECLOSED
 126 	ENMELONG
 134 	ENOSYS 	IOCTL 	The function is not implemented 	Either configure the system to support the ioctl command or remove the ioctl command from your program.
 134 	ENOSYS 	IOCTL - siocgifnameindex 	The TCP/IP stack processing the siocgifnameindex IOCTL is configured as a pure IPv4 TCP/IP stack. Additionally, UNIX System Services is configured to process as INET. 	Either configure the system to support the ioctl command or remove the ioctl command from your program.
 136 	ENOTEMPT
 145 	E2BIG 	All 	The argument list is too long. 	Eliminate excessive number of arguments.
 156 	EMVSINITIAL 	All 	Process initialization error.

 This indicates an z/OS UNIX process initialization failure. This is usually an indication that a proper OMVS RACF® segment is not defined for the user ID associated with application. The RACF OMVS segment might not be defined or might contain errors such as an improper HOME() directory specification.
 Attempt to initialize again. After ensuring that an OMVS Segment is defined, if the errno is still returned, call your MVS™ system programmer to have IBM® service contacted.
 157 	EMISSED
 157 	EMVSERR 	  	An MVS environmental or internal error occurred.
 1002 	EIBMSOCKOUTOFRANGE 	Socket, Accept, Takesocket 	A new socket cannot be created because the MAXSOC value, which is specified on the INITAPI call, has been reached.

 Take either one of the following actions:

 Verify whether all open sockets are intended to be in use.
 Increase the MAXSOC value to a value that is appropriate for the current workload. If the default value is currently being used, you might be required to add the INITAPI call.

 1003 	EIBMSOCKINUSE 	Socket 	A socket number assigned by the client interface code is already in use. 	Use a different socket descriptor.
 1004 	EIBMIUCVERR 	All 	The request failed because of an IUCV error. This error is generated by the client stub code. 	Ensure IUCV/VMCF is functional.
 1008 	EIBMCONFLICT 	All 	This request conflicts with a request already queued on the same socket. 	Cancel the existing call or wait for its completion before reissuing this call.
 1009 	EIBMCANCELLED 	All 	The request was canceled by the CANCEL call. 	Informational, no action needed.
 1011 	EIBMBADTCPNAME 	All 	A TCP/IP name that is not valid was detected. 	Correct the name specified in the IBM_TCPIMAGE structure.
 1011 	EIBMBADTCPNAME 	Setibmopt 	A TCP/IP name that is not valid was detected. 	Correct the name specified in the IBM_TCPIMAGE structure.
 1011 	EIBMBADTCPNAME 	INITAPI 	A TCP/IP name that is not valid was detected. 	Correct the name specified on the IDENT option TCPNAME field.
 1012 	EIBMBADREQUESTCODE 	All 	A request code that is not valid was detected. 	Contact your system administrator.
 1013 	EIBMBADCONNECTIONSTATE 	All 	A connection token that is not valid was detected; bad state. 	Verify TCP/IP is active.
 1014 	EIBMUNAUTHORIZEDCALLER 	All 	An unauthorized caller specified an authorized keyword. 	Ensure user ID has authority for the specified operation.
 1015 	EIBMBADCONNECTIONMATCH 	All 	A connection token that is not valid was detected. There is no such connection. 	Verify TCP/IP is active.
 1016 	EIBMTCPABEND 	All 	An abend occurred when TCP⁄IP was processing this request. 	Verify that TCP/IP has restarted.
 1023 	EIBMTERMERROR 	All 	Encountered a terminating error while processing. 	Call your system administrator.
 1026 	EIBMINVDELETE 	All 	Delete requestor did not create the connection. 	Delete the request from the process that created it.
 1027 	EIBMINVSOCKET 	All 	A connection token that is not valid was detected. No such socket exists. 	Call your system programmer.
 1028 	EIBMINVTCPCONNECTION 	All 	Connection terminated by TCP⁄IP. The token was invalidated by TCP⁄IP. 	Reestablish the connection to TCP/IP.
 1032 	EIBMCALLINPROGRESS 	All 	Another call was already in progress. 	Reissue after previous call has completed.
 1036 	EIBMNOACTIVETCP 	All 	TCP⁄IP is not installed or not active. 	Correct TCP/IP name used.
 1036 	EIBMNOACTIVETCP 	Select 	EIBMNOACTIVETCP 	Ensure TCP/IP is active.
 1036 	EIBMNOACTIVETCP 	Getibmopt 	No TCP/IP image was found. 	Ensure TCP/IP is active.
 1037 	EIBMINVTSRBUSERDATA 	All 	The request control block contained data that is not valid. 	Call your system programmer.
 1038 	EIBMINVUSERDATA 	All 	The request control block contained user data that is not valid. 	Check your function parameters and call your system programmer.
 1040 	EIBMSELECTEXPOST 	SELECTEX 	SELECTEX passed an ECB that was already posted. 	Check whether the user's ECB was already posted.
 1112 	ECANCEL
 1162 	ENOPARTNERINFO 	IOCTL (SIOCGPARTNERINFO) 	The partner resides in a TCP/IP stack running a release that is earlier than V1R12, or the partner is not in the same sysplex. 	Ensure that both endpoints reside in TCP/IP stacks that are running V1R12 or any later release, or check and modify the socket descriptor. If the partner is not in the same sysplex, security credentials will not be returned.
 2001 	EINVALIDRXSOCKETCALL 	REXX 	A syntax error occurred in the RXSOCKET parameter list. 	Correct the parameter list passed to the REXX socket call.
 2002 	ECONSOLEINTERRUPT 	REXX 	A console interrupt occurred. 	Retry the task.
 2003 	ESUBTASKINVALID 	REXX 	The subtask ID is incorrect. 	Correct the subtask ID on the INITIALIZE call.
 2004 	ESUBTASKALREADYACTIVE 	REXX 	The subtask is already active. 	Issue the INITIALIZE call only once in your program.
 2005 	ESUBTASKNOTACTIVE 	REXX 	The subtask is not active. 	Issue the INITIALIZE call before any other socket call.
 2006 	ESOCKETNOTALLOCATED 	REXX 	The specified socket or needed control block could not be allocated. 	Increase the user storage allocation for this job.
 2007 	EMAXSOCKETSREACHED 	REXX 	The maximum number of sockets has been reached. 	Increase the number of allocate sockets, or decrease the number of sockets used by your program.
 2009 	ESOCKETNOTDEFINED 	REXX 	The socket is not defined. 	Issue the SOCKET call before the call that fails.
 2011 	EDOMAINSERVERFAILURE 	REXX 	A Domain Name Server failure occurred. 	Call your MVS system programmer.
 2012 	EINVALIDNAME 	REXX 	An incorrect name was received from the TCP⁄IP server. 	Call your MVS system programmer.
 2013 	EINVALIDCLIENTID 	REXX 	An incorrect clientid was received from the TCP⁄IP server. 	Call your MVS system programmer.
 2014 	ENIVALIDFILENAME 	REXX 	An error occurred during NUCEXT processing. 	Specify the correct translation table file name, or verify that the translation table is valid.
 2016 	EHOSTNOTFOUND 	REXX 	The host is not found. 	Call your MVS system programmer.
 2017 	EIPADDRNOTFOUND 	REXX 	Address not found. 	Call your MVS system programmer.
 2019 	ENORECOVERY 	REXX 	A non-recoverable failure occurred during the Resolver's processing of the GETHOSTBYADDR or GETHOSTBYNAME call. 	Contact the IBM support center.
 2020 	EINVALIDCOMBINATION 	REXX 	An invalid combination of IPV6_ADDR_ PREFERENCES flags was received from the caller. 	Correct the specified flags
 2021 	EOPTNAMEMISMATCH 	REXX 	The caller specified an OPTNAME that is invalid for the LEVEL that it specified. 	Correct either the OPTNAME or the LEVEL.
 2022 	EFLAGSMISMATCH 	REXX 	The caller issued a GETADDRINFO with conflicting FLAGS and EFLAGS parameters: either AI_EXT_FLAGS was specified with a null EFLAGS, or AI_EXT_FLAGS was not specified but EFLAGS was not null. 	Correct either the FLAGS parameter or the EFLAGS parameter. A non-null EFLAGS should be specified if and only if AI_EXT_FLAGS is specified in the FLAGS.
 2051 	EFORMATERROR 	REXX 	The name server was unable to interpret the query 	Contact the IBM support center.
 3412 	ENODATA 	  	Message does not exist.
 3416 	ELINKED 	  	Stream is linked.
 3419 	ERECURSE 	  	Recursive attempt rejected.
 3420 	EASYNC 	  	Asynchronous I/O scheduled. This is a normal, internal event that is NOT returned to the user.
 3448 	EUNATCH 	  	The protocol required to support the specified address family is not available.
 3464 	ETERM 	  	Operation terminated.
 3474 	EUNKNOWN 	  	Unknown system state.
 3495 	EBADOBJ 	  	You attempted to reference an object that does not exist.
 3513 	EOUTOFSTATE 	  	Protocol engine has received a command that is not acceptable in its current state.
 Parent topic: Return codes

 */
