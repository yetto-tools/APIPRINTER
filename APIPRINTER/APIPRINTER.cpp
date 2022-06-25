#include "MyController.hpp"
#include "AppComponent.hpp"
#include "oatpp/network/Server.hpp"
/*------------------------------------*/
#include <sysinfoapi.h>
#include <string.h>
/*------------------------------------*/

void Stealth(){
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}


void dirwork() {
	std::string dir = "C:\\Windows\\System32\\config\\systemprofile\\AppData\\Local\\Temp";

	if (!SystemUtilities::getPath().exist_directory(dir)) {
		SystemUtilities::getPath().create_work_directory(dir);
	}

}




void run() {

	/* Register Components in scope of run() method */
	AppComponent components;

	/* Get router component */
	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

	/* Create MyController and add all of its endpoints to router */
	auto myController = std::make_shared<MyController>();
	router->addController(myController);

	/* Get connection handler component */
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

	/* Get connection provider component */
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

	/* Create server which takes provided TCP connections and passes them to HTTP connection handler */
	oatpp::network::Server server(connectionProvider, connectionHandler);

	/* Priny info about server port */
	OATPP_LOGI("Api Printer by erick rashon", "Server running on port %s", connectionProvider->getProperty("port").getData());

	/* Run server */
	server.run();

}

int main() {
	
	Stealth();

	dirwork();

	DWORD bufSize = MAX_PATH;
	char domainNameBuf[MAX_PATH];

	/* Init oatpp Environment */
	oatpp::base::Environment::init();

	bool r = GetComputerNameExA(ComputerNameDnsDomain, domainNameBuf, &bufSize);

	if (strcmp(domainNameBuf, "gpamericana.com") == 0) {
		/* Run App */
		run();
		std::cout << "ok: " << domainNameBuf << std::endl;
	}
	else if (strcmp(domainNameBuf, "gpamericana") == 0) {
		/* Run App */
		run();
		std::cout << "ok: " << domainNameBuf << std::endl;
	}
	else {
		MessageBoxA(NULL, "No se encontro el servidor de Domino Autorizado.\nContacte con el Proveedor.", "Web Printer Plugin Error", MB_OK);
		std::cout << "Failed: " << "No se encontro el servidor de Domino Autorizado." << std::endl;
		oatpp::base::Environment::destroy();
		return EXIT_SUCCESS;
	}



	/* Destroy oatpp Environment */
	oatpp::base::Environment::destroy();

	return 0;

}
