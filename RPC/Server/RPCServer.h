#ifndef RPCSERVER_H
#define RPCSERVER_H

#include "InterfaceRPC_h.h"
#include <unordered_map>
#include <string>

class RPCServer
{
public:
	RPCServer();
    bool listen();
	int isError() { return _error_code; };
    bool login(handle_t binding_handle, const unsigned char* username,
        const unsigned char* password);
    void logout(handle_t binding_handle);
    bool downloadFile(handle_t binding_handle, const unsigned char* file_name, const unsigned int file_size, unsigned char* data);
    bool uploadFile(handle_t binding_handle, const unsigned char* file_name, const unsigned int file_size, unsigned char* data);
    bool deleteFile(handle_t binding_handle, const unsigned char* file_name);
    bool getFileSize(handle_t binding_handle, const unsigned char* file_name, unsigned int* file_size);
private:
    std::string convertUNCToLocal(const unsigned char* file_name);
    bool workWithFile(handle_t binding_handle, const unsigned char* file_name, unsigned int* file_size, 
        unsigned char* data, int action);
    std::string _server_name;
	int _error_code;
    std::unordered_map<RPC_BINDING_HANDLE, HANDLE> _clients;
};
#endif

