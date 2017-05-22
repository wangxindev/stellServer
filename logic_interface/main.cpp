#include "global.h"
#include "logicHttpGetJson.h"
#include "logicHttpSendJson.h"

int main(void)
{
	logInit();

	int port = 3355;
	int nthread = 10;
	int backlog = 10240;
	char addr[] = "0.0.0.0";

	LOGIS("main", "port:%d\n", port);
	LOGIS("main", "nthread:%d\n", nthread);
	LOGIS("main", "backlog:%d\n", backlog);

	logicHttpGetJson pushJsonToServer("interfaceSendOriginalData");
	logicHttpSendJson sendJsonToUser("interfaceGetObjectData");

	httpServer server(addr, port, nthread, backlog, NULL);
	server.addInterfaceLogic(&pushJsonToServer);
	server.addInterfaceLogic(&sendJsonToUser);
	server.startServer();

	return 0;
}
