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

// ���¼�
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

// �����¼�
void onAccept(int svrfd, short ievent, void *arg)
{
	int clifd;
	struct sockaddr_in cliaddr;

	socklen_t sinsize = sizeof(cliaddr);
	clifd = accept(svrfd, (struct sockaddr*)&cliaddr, &sinsize);

	struct event *pread = new event;
	event_set(pread, clifd, EV_READ | EV_PERSIST, onRead, pread);  // ע���(д)�¼�
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

	// ��ʼ���¼���
	base = event_base_new();

	// ��ʼ��һ�������¼�,EV_PRESISTָ���ظ�ִ�и��¼�
	struct event evlisten;
	event_set(&evlisten, svrfd, EV_READ | EV_PERSIST, onAccept, NULL);

	// ����Ϊbase�¼�
	event_base_set(base, &evlisten);
	// ����¼�
	event_add(&evlisten, NULL);
	// �¼�ѭ��
	event_base_dispatch(base);

	WSACleanup();

	return 0;

}
