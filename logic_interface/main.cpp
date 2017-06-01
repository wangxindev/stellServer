#include "global.h"
#include "logicHttpGetJson.h"
#include "logicHttpSendJson.h"

int main(void)
{
	logInit();
	thread_pool_init(16);

	int port = 3355;
	int nthread = 10;
	int backlog = 10240;
	char addr[] = "0.0.0.0";

	LOG_I_S("main", "port:%d\n", port);
	LOG_I_S("main", "nthread:%d\n", nthread);
	LOG_I_S("main", "backlog:%d\n", backlog);

	logicHttpGetJson pushJsonToServer("interfaceSendOriginalData");
	logicHttpSendJson sendJsonToUser("interfaceGetObjectData");

	httpServer server(addr, port, nthread, backlog, NULL);
	server.addInterfaceLogic(&pushJsonToServer);
	server.addInterfaceLogic(&sendJsonToUser);
	server.startServer();

	return 0;
}
