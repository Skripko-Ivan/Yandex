#pragma comment(lib, "rpcrt4.lib")

#include "Menu.h"
#include "InterfaceRPC_h.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>

#define BUF_LEN 2000000
#define PACKET_SIZE 1024*1024

int Menu::start() {
	std::string ip, port;
	std::cout << "[*] Set IP-address of server" << std::endl <<
		"\tIP-address: ";
	std::cin >> ip;
	_ip = ip;
	//std::cout << "\tPort: ";
	//std::cin >> port;
	//_ip = ip = "192.168.159.131";
	port = "4444";
	RPC_STATUS status;
	RPC_CSTR binding_string;
	status = RpcStringBindingComposeA(
		NULL,
		(RPC_CSTR)("ncacn_ip_tcp"),
		(RPC_CSTR)(ip.c_str()),
		(RPC_CSTR)(port.c_str()),
		NULL,
		&binding_string);
	if (status) {
		std::cout << "[!] RpcStringBindingComposeA error, code: " << status << std::endl;
		return 0;
	}
	std::cout << "[*] RpcStringBindingComposeA success" << std::endl;
	RPC_BINDING_HANDLE handler;
	status = RpcBindingFromStringBindingA(binding_string, &handler);
	if (status) {
		std::cout << "[!] RpcBindingFromStringBindingA error, code: " << status << std::endl;
		return 0;
	}
	std::cout << "[*] RpcBindingFromStringBindingA success" << std::endl;
	_handler = handler;
	std::string username, password;
	while (1) {
		std::cout << "[*] Type username and password" << std::endl <<
			"\tUsername: ";
		getchar();
		std::getline(std::cin, username);
		std::cout << "\tPassword: ";
		std::getline(std::cin, password);
		//username = "User2";
		//password = "User2Super";
		if (!login(handler, (unsigned char*)username.c_str(), (unsigned char*)password.c_str())) {
			std::cout << "[!] Unsuccess login, try again" << std::endl;
		}
		else {
			std::cout << "[*] Success login" << std::endl;
			break;
		}
	}
	help();
	return 1;
}

void Menu::help() {
	std::cout << "[*] Usage:\n\t-d <FILE_NAME>\tdelete file <FILE_NAME>\n\t" <<
		"-dl <FILE_NAME> <NEW_FILE>\tdownload file <FILE_NAME> and save as <NEW_FILE>\n\t" <<
		"-u <FILE_NAME>\tupload file <FILE_NAME>\n\t" <<
		"-exit" << std::endl;
}

void Menu::typeCommand() {
	std::string command;
	std::cout << "[*] Type a command: ";
	std::getline(std::cin, command);
	doCommand(command);
}

void Menu::doCommand(std::string cmd) {
	std::istringstream buf(cmd);
	std::string temp, temp1;
	buf >> temp;
	if (temp == "-d") {
		buf >> temp;
		doDelete(temp);
	}
	else if (temp == "-dl") {
		buf >> temp;
		buf >> temp1;
		doDownload(temp1, temp);
	}
	else if (temp == "-u") {
		buf >> temp;
		doUpload(temp);
	}
	else if (temp == "-exit") {
		exit(0);
	}
	else {
		std::cout << "[!] Unknown command" << std::endl;
		help();
	}
}

void Menu::doDelete(std::string path) {
	//std::string path = getFilePath();
	path = "\\\\" + _ip + "\\" + path;
	if (!deleteFile(_handler, (unsigned char*)path.c_str())) {
		std::cout << "[!] File did not delete" << std::endl;
		return;
	}
	std::cout << "[*] File is deleted" << std::endl;
}

void Menu::doDownload(std::string newFileName, std::string path) {
	path = "\\\\" + _ip + "\\" + path;
	unsigned int res = fileSize(path);
	if (res < 0) {
		return;
	}
	long long fileSize = res;
	unsigned char* data = new unsigned char[PACKET_SIZE];
	int packetNumber = 1;
	std::ofstream out;
	out.open(newFileName, std::ios::app | std::ios::binary);
	if (!out.is_open()) {
		std::cout << "[!] Cannot open file for writing" << std::endl;
		delete[] data;
	}
	else
		while (fileSize > 0) {
			Sleep(100);
			if (fileSize - PACKET_SIZE > 0) {
				if (!downloadFile(_handler, (unsigned char*)path.c_str(), PACKET_SIZE, data)) {
					std::cout << "[!] Cannot dowload file" << std::endl;
					delete[] data;
					out.close();
					break;
				}
				else {
					out.write((char*)data, PACKET_SIZE);
					std::cout << "[*] Successful dowload packet " << packetNumber++ << " of file" << std::endl;
					fileSize -= PACKET_SIZE;
				}
			}
			else {
				if (!downloadFile(_handler, (unsigned char*)path.c_str(), fileSize, data)) {
					std::cout << "[!] Cannot dowload file" << std::endl;
					delete[] data;
					out.close();
					break;
				}
				else {
					out.write((char*)data, fileSize);
					std::cout << "[*] Successful dowload last packet " << packetNumber++ << " of file" << std::endl;
					fileSize -= fileSize;
				}
			}
		}
	out.close();
	if (fileSize == 0) {
		std::cout << "[*] Successful download file" << std::endl;
	}
}

void Menu::doUpload(std::string path) {
	FILE* file;
	fopen_s(&file, path.c_str(), "rb");
	if (!file) {
		std::cout << "[!] Cannot open file" << std::endl;
		return;
	}
	std::cout << "[*] File is open" << std::endl;
	struct stat info;
	stat(path.c_str(), &info);
	int res = ferror(file);
	if (res != 0) {
		return;
	}
	path = "\\\\" + _ip + "\\" + path;
	std::cout << "[*] Start upload file..." << std::endl;
	long long size = (unsigned int)info.st_size;
	int packetNumber = 1;
	unsigned char* content;
	content = new unsigned char[PACKET_SIZE];
	while (size > 0) {
		if (size - PACKET_SIZE > 0) {
			fread(content, sizeof(char), PACKET_SIZE, file);
			if (!uploadFile(_handler, (unsigned char*)path.c_str(), PACKET_SIZE, (unsigned char*)content)) {
				std::cout << "[!] Cannot upload file" << std::endl;
				delete[] content;
				break;
			}
			else {
				std::cout << "[*] Successful upload packet " << packetNumber++ << " of file" << std::endl;
				size -= PACKET_SIZE;
			}
		}
		else {
			//content = new unsigned char[size];
			fread(content, sizeof(char), size, file);
			if (!uploadFile(_handler, (unsigned char*)path.c_str(), (unsigned int)size, (unsigned char*)content)) {
				std::cout << "[!] Cannot upload file" << std::endl;
				delete[] content;
				break;
			}
			else {
				std::cout << "[*] Successful upload packet " << packetNumber++ << " of file" << std::endl;
				size -= size;
			}
		}
	}
	if (size == 0)
		std::cout << "[*] Successful upload file" << std::endl;
}

int Menu::fileSize(std::string path) {
	unsigned int fileSize;
	if (!getFileSize(_handler, (unsigned char*)path.c_str(), &fileSize)) {
		std::cout << "[!] Cannot get size of this file" << std::endl;
		return -1;
	}
	std::cout << "[*] Success got size of this file: " << fileSize << std::endl;
	return fileSize;
}

void* __RPC_USER midl_user_allocate(size_t size)
{
	return malloc(size);
}

// Memory deallocation function for RPC.
void __RPC_USER midl_user_free(void* p)
{
	free(p);
}