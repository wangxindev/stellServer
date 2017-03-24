#include <stdio.h>
#include <string.h>
#include <event.h>


int init_win_socket()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}
	return 0;
}

struct event_base *base;

// 读事件
void onRead(int clifd, short ievent, void *arg)
{
	int ilen;
	char buf[1500];

	ilen = recv(clifd, buf, 1500, 0);
	printf("ilen = %d\n", ilen);

	if (ilen <= 0)
	{
		printf("Client close\n");

		struct event *pread = (struct event*)arg;
		event_del(pread);
		delete pread;

		//close(clifd);
		return;
	}

	buf[ilen] = '\0';
	printf("Accpet: %s\n", buf);
}

// 连接事件
void onAccept(int svrfd, short ievent, void *arg)
{
	int clifd;
	struct sockaddr_in cliaddr;

	socklen_t sinsize = sizeof(cliaddr);
	clifd = accept(svrfd, (struct sockaddr*)&cliaddr, &sinsize);

	struct event *pread = new event;
	event_set(pread, clifd, EV_READ | EV_PERSIST, onRead, pread);  // 注册读(写)事件
	event_base_set(base, pread);
	event_add(pread, NULL);
}


int main()
{
#ifdef WIN32
	init_win_socket();
#endif

	int svrfd;
	struct sockaddr_in svraddr;

	memset(&svrfd, 0, sizeof(svraddr));
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(3354);
	svraddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	svrfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(svrfd, (struct sockaddr*)&svraddr, sizeof(svraddr));

	listen(svrfd, 10);

	// 初始化事件库
	base = event_base_new();

	// 初始化一个连接事件,EV_PRESIST指定重复执行该事件
	struct event evlisten;
	event_set(&evlisten, svrfd, EV_READ | EV_PERSIST, onAccept, NULL);

	// 设置为base事件
	event_base_set(base, &evlisten);
	// 添加事件
	event_add(&evlisten, NULL);
	// 事件循环
	event_base_dispatch(base);

	WSACleanup();

	return 0;

}
