// example_client.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <iostream>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

int main()
{
	SOCKET s;
	WSADATA wsaData;
	SOCKADDR_IN sin;

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		return 0;
	}

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET) {
		WSACleanup();
		return 0;
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(1234);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(s, (SOCKADDR*)&sin, sizeof(sin)) != 0) {
		closesocket(s);
		WSACleanup();
		return 0;
	}

	printf("서버 연결 성공 \n 전송할 데이터 입력 : ");
	
	//String str;
	//char buffer[2048];
	char *buffer;
	buffer = new char[1024];
	scanf_s("%c", &buffer);
	int send_len;

	while ((send_len = send(s, buffer, 257, 0)) == -1)
	{
		if (errno == EINTR) continue;
		else
		{
			fprintf(stderr, "발신 실패 \n");
			return -1;
		}
	}
	//strlen(buffer) + 1, 0);

	printf("전송 성공");

	recv(s, buffer, 1024, 0);

	printf("%s", buffer);

	if (closesocket(s) != 0)
	{
		WSACleanup();
		return 0;
	}

	if (WSACleanup() != 0)
	{
		return 0;
	}
	delete[] buffer;
    return 0;
}

