#ifndef MENU_H
#define MENU_H
#include <string>
#include "InterfaceRPC_h.h"
class Menu
{
public:
	int start();
	void typeCommand();
private:
	int fileSize(std::string path);
	void help();
	void doCommand(std::string);
	void doDelete(std::string);
	void doDownload(std::string, std::string);
	void doUpload(std::string);
	std::string _ip;
	handle_t _handler;
};
#endif
