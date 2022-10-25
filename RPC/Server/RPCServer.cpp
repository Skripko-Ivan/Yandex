#pragma comment(lib, "rpcrt4.lib")

#include "RPCServer.h"
#include "InterfaceRPC_h.h"
#include <iostream>
#include <Windows.h>
#include <unordered_map>
#include <future>
#include <fstream>
#include <filesystem>
#include <cstdio>

RPCServer::RPCServer() {
	_server_name = "192.168.159.131";
	_error_code = 0;  // no error

	RPC_STATUS status;
	status = RpcServerRegisterIfEx(InterfaceRPC_v1_0_s_ifspec, NULL, NULL, 
		RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, RPC_C_LISTEN_MAX_CALLS_DEFAULT, NULL);
	if (status != RPC_S_OK) {
		std::cout << "[!] RpcServerRegisterIf failed, error code: " << (int)status;
		_error_code = (int)status;
		return;
	}

	status = RpcServerUseProtseqEpA(
		(RPC_CSTR)("ncacn_ip_tcp"),
		RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
		(RPC_CSTR)("4444"), 
		NULL
		);
	if (status != RPC_S_OK) { // RPC_S_OK return value
		std::cout << "[!] RpcServerUseProtseqEpA failed, error code: " << (int)status;
		_error_code = (int)status;
		return;
	}

	RPC_BINDING_VECTOR* rpcBindingVector;
	status = RpcServerInqBindings(&rpcBindingVector);
	if (status != RPC_S_OK) { // RPC_S_OK return value
		std::cout << "[!] RpcServerInqBindings failed, error code: " << (int)status;
		_error_code = (int)status;
		return;
	}

	status = RpcEpRegister(InterfaceRPC_v1_0_s_ifspec, rpcBindingVector, NULL, NULL);
	if (status != RPC_S_OK) { // RPC_S_OK return value
		std::cout << "[!] RpcEpRegister failed, error code: " << (int)status;
		_error_code = (int)status;
		return;
	}
}

bool RPCServer::listen() {
	RPC_STATUS status;
	std::cout << "[*] Listening" << std::endl;
	status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, false);
	if (status != RPC_S_OK) {
		std::cout << "[!] RpcServerListen failed, error code: " << (int)status;
		_error_code = (int)status;
		return false;
	}
	std::cout << "[*] Listen success" << std::endl;
	return true;
}

bool RPCServer::login(handle_t binding_handle, const unsigned char* username,
	const unsigned char* password) {
	std::cout << "[*] Client tries to login, username: " << username << " password: " << password << std::endl;
	HANDLE user_token;
	// attempt to log on on local computer as user and get token handle for impersonalisation

	int success = LogonUserA((LPCSTR)username, NULL,
		(LPCSTR)password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT,
		&user_token);
	if (!success)  // no zero if success
	{
		int error = GetLastError();
		if (error == ERROR_LOGON_FAILURE) {
			std::cout << "[!] Invalid username or password" << std::endl;
			return false;
		}
		else  // runtime error
			std::cout << "[!] LogonUser failed, error code: " + error;
	}

	_clients.insert(std::make_pair(binding_handle, user_token));  // for many users in one time
	std::cout << "[*] Client logged in" << std::endl;
	return true;
}

void RPCServer::logout(handle_t binding_handle) {
	std::cout << "[*] Client logged out" << std::endl;
	_clients.erase(binding_handle);
}

bool RPCServer::downloadFile(handle_t binding_handle, const unsigned char* file_name,
	const unsigned int file_size, unsigned char* data) {
	unsigned int copy_file_size = file_size;
	return workWithFile(binding_handle, file_name, &copy_file_size, data, 1);
}

bool RPCServer::uploadFile(handle_t binding_handle, const unsigned char* file_name,
	const unsigned int file_size, unsigned char* data) {
	unsigned int copy_file_size = file_size;
	return workWithFile(binding_handle, file_name, &copy_file_size, data, 0);
}

bool RPCServer::deleteFile(handle_t binding_handle, const unsigned char* file_name) {
	unsigned int file_size = 0;
	return workWithFile(binding_handle, file_name, &file_size, nullptr, 2);
}

bool RPCServer::getFileSize(handle_t binding_handle, const unsigned char* file_name, unsigned int* file_size) {
	return workWithFile(binding_handle, file_name, file_size, nullptr, 3);
}

bool RPCServer::workWithFile(handle_t binding_handle, const unsigned char* file_name, unsigned int* file_size,
	unsigned char* data, int action) {
	auto client = _clients.find(binding_handle);
	if (client == _clients.end())  // not founded -> ignore
		return false;
	std::string local_file_name;
	try {
		if ((local_file_name = convertUNCToLocal(file_name)) == "") {
			return false;
		}
		std::cout << "[*] Client tries to upload file" << std::endl;
		// create thread which will impersonate client
		std::future<bool> future = std::async(std::launch::async, [&]() {
			if (!ImpersonateLoggedOnUser(client->second)) {  // action from user face
				std::cout << "[!] ImpersonateLoggedOnUser failed, error code: " << GetLastError() << std::endl;
				return false;
			}
			bool res = true;
			switch (action) {
			case 0: {  // upload
				std::ofstream output_stream(local_file_name, std::ifstream::trunc | std::ifstream::binary);
				if (!output_stream.good()) {
					std::cout << "[!] File cannot be opened for writing" << std::endl;
					res = false;
				}
				else {
					std::copy(data, &data[*file_size], std::ostreambuf_iterator<char>(output_stream));
					std::cout << "[*] File is uploaded" << std::endl;
				}
				break;
			}
			case 1: {  // download
				std::ifstream input_stream(local_file_name, std::ifstream::binary);
				if (!input_stream.good()) {
					std::cout << "[!] File cannot be opened" << std::endl;
					res = false;
				}
				else {
					std::istreambuf_iterator<char> end;
					unsigned int cnt = 0;
					for (std::istreambuf_iterator<char> ptr(input_stream); ptr != end; ++ptr) {
						data[cnt++] = *ptr;
					}
					data[cnt] = 0;
					std::cout << "[*] File is downloaded" << std::endl;
				}
				break;
			}
			case 2: {  // delete
				int result = remove(local_file_name.c_str());
				if (result == 0) {
					std::cout << "[*] File is deleted" << std::endl;
					res = true;
				}
				else {
					std::cout << "[!] File cannot be deleted, error code: " << result << std::endl;
					res = false;
				}
				break;
			}
			case 3: {
				std::ifstream input_stream(local_file_name, std::ifstream::binary);
				int size = input_stream.tellg();
				*file_size = size;
				std::cout << "[*] File size is got" << std::endl;
				res = true;
				break;
			}
			}
			if (!RevertToSelf()) {
				std::cout << "[!] RevertToSelf failed, error code: " << GetLastError() << std::endl;
				return false;
			}
			return res;
			});
		return future.get();
	}
	catch (std::exception& e)
	{
		std::cout << "[!] " << e.what() << std::endl;
		return false;
	}
}

std::string RPCServer::convertUNCToLocal(const unsigned char* file_name)
{
	// check begin of UNC name
	std::string string_file_name((char*)file_name);
	if (string_file_name[0] != '\\' || string_file_name[1] != '\\') {
		std::cout << "[!] Not UNC format" << std::endl;
		return "";
	}
	// check server name before first '\'
	auto server_end = string_file_name.find('\\', 2);
	std::string received_server_name(string_file_name.begin() + 2, string_file_name.begin() + server_end);
	if (received_server_name != _server_name) {
		std::cout << "[!] Wrong server name" << std::endl;
		return "";
	}
	auto local_path = string_file_name.substr(server_end + 1);
	return local_path;
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
