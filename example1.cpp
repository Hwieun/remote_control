#include "stdafx.h"
#include <stdio.h>
#include <WinSock2.h>


#pragma comment (lib, "ws2_32.lib")


#define NM_CHAT_DATA			1	// 채팅 메시지
#define NM_SEND_SCREEN_IMAGE	2	// 화면 전송 메시지


int main() {

	SOCKET		s, cs;
	WSADATA		wsaData;
	SOCKADDR_IN sin, cli_addr;

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		printf("WSAStartup 실패, 에러코드 : %d \n", WSAGetLastError());
		return 0;

	}

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (s == INVALID_SOCKET) {
		printf("소켓 생성 실패, 에러코드 : %d \n", WSAGetLastError());
		WSACleanup();
		return 0;
	}

	sin.sin_family = AF_INET;

	sin.sin_port = htons(1234);
	sin.sin_addr.S_un.S_addr = htonl(ADDR_ANY);

	if (bind(s, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind실패 , 에러콛 : %d \n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return 0;
	}

	if (listen(s, SOMAXCONN) != 0) {
		printf("모드 설정 실패");
		closesocket(s);
		WSACleanup();
		return 0;
	}

	printf(" 서버를 시작합니다. ");
	printf(" 클라이언트 접속 대기 ");

	int cli_size = sizeof(cli_addr);

	cs = accept(s, (SOCKADDR*)&cli_addr, &cli_size);
	if (cs == INVALID_SOCKET) {
		printf("접속 승인 실패");
		closesocket(s);
		WSACleanup();
		return 0;
	}

	printf(" 클라이언트와 연결되었습니다.");

	char Buffer[1024];
	int recv_len;

	while ((recv_len = recv(cs, Buffer, 257, 0)) == -1)
	{
		if (errno == EINTR) continue;
		else
		{
			fprintf(stderr, "수신 실패 \n");
			return -1;
		}
	}
		
	printf("클라이언트가 보낸 데이터 \n %c \n", Buffer);

	printf("서버에 보낼 데이터 입력 : ");
	scanf_s("%c", &Buffer);
	send(cs, Buffer, strlen(Buffer), 0);

	if (closesocket(cs) != 0 || closesocket(s) != 0)
	{
		WSACleanup();
		return 0;
	}

	if (WSACleanup() != 0)
	{
		return 0;
	}

	printf("서버를 종료합니다.");
	return 0;
}
