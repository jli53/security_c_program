#include<winsock2.h>
#include<stdio.h>
#pragma comment(lib,"ws2_32")
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)

//tcp header structure
typedef struct _TCPHeader
{
	USHORT sourcePort;
	USHORT destinationPort;
	ULONG sequenceNumber;
	ULONG acknowledgeNumber;
	UCHAR dataoffset;
	UCHAR flags;
	USHORT windows;
	USHORT checksum;
	USHORT urgentPointer;
}TCPHeader, *PTCPHeader;

//udp header structure
typedef struct _UDPHeader
{
	USHORT sourcePort;
	USHORT destinationPort;
	USHORT len;
	USHORT checksum;
}UDPHeader, *PUDPHeader;

typedef struct _IPHeader
{
	UCHAR iphVerLen;
	UCHAR ipTOS;
	USHORT ipLength;
	USHORT ipID;
	USHORT ipFlags;
	UCHAR ipTTL;
	UCHAR ipProtocol;
	USHORT ipChecksum;
	ULONG ipSource;
	ULONG ipDestination;
}IPHeader,*PIPHeader;
void DecodeTCPPacket(char *pData);
void DecodeUDPPacket(char *pData);
void DecodeIPPacket(char *pData);

void main(){
	WSADATA ws;
	WSAStartup(MAKEWORD(2,2),&ws);
	//create raw socket
	SOCKET sRaw = socket(AF_INET,SOCK_RAW,IPPROTO_IP);

	char szHostName[56];
	SOCKADDR_IN addr_in;
	struct hostent *pHost;
	gethostname(szHostName,56);
	if((pHost=gethostbyname((char*)szHostName))==NULL)
		return;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(0);
	memcpy(&addr_in.sin_addr.S_un.S_addr, pHost->h_addr_list[0],pHost->h_length);
	printf("Binding To Interface: %s\n",::inet_ntoa(addr_in.sin_addr));
	if(bind(sRaw,(SOCKADDR*)&addr_in,sizeof(addr_in))==SOCKET_ERROR)
		return;
	//set net card to promiscuous mode
	DWORD dwValue = 1;
	if(ioctlsocket(sRaw,SIO_RCVALL,&dwValue)!=0)
		return;
	char buffer[1024];
	int nRet;
	while(TRUE){
		nRet = recv(sRaw,buffer,1024,0);
		if(nRet > 0)
			DecodeIPPacket(buffer);
	}
	closesocket(sRaw);
}

void DecodeTCPPacket(char *pData){
	TCPHeader *pTcpHdr = (TCPHeader *)pData;
	printf("\tTCP Port:%d -> %d\n",ntohs(pTcpHdr->sourcePort),ntohs(pTcpHdr->destinationPort));
}

void DecodeUDPPacket(char *pData){
	UDPHeader *pUdpHdr = (UDPHeader *)pData;
	printf("\tUDP Port:%d -> %d\n",ntohs(pUdpHdr->sourcePort),ntohs(pUdpHdr->destinationPort));
}

void DecodeIPPacket(char *pData){
	IPHeader *pIpHdr = (IPHeader*)pData;
	in_addr source,dest;
	char szSourceIp[32],szDestIp[32];
	source.S_un.S_addr=pIpHdr->ipSource;
	dest.S_un.S_addr=pIpHdr->ipDestination;
	strcpy(szSourceIp,::inet_ntoa(source));
	strcpy(szDestIp,::inet_ntoa(dest));
	printf("\t%s->%s\n",szSourceIp,szDestIp);

	int nHeaderLen=(pIpHdr->iphVerLen & 0xF)*sizeof(ULONG);
	switch(pIpHdr->ipProtocol){
	case IPPROTO_TCP:
		DecodeTCPPacket(pData+nHeaderLen);
		break;
	case IPPROTO_UDP:
		DecodeUDPPacket(pData+nHeaderLen);
	case IPPROTO_ICMP:
		break;
	default:
		break;
	}
}