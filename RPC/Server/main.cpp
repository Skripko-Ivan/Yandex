#include "RPCServer.h"
#include <iostream>

RPCServer server;

int main() {
    server.listen();
	if (int isError = server.isError())
		return isError;
}

unsigned char login(
    /* [in] */ handle_t binding_handle,
    /* [string][in] */ const unsigned char* username,
    /* [string][in] */ const unsigned char* password) {
    return server.login(binding_handle, username, password);
}

void logout(
    /* [in] */ handle_t binding_handle) {
    server.logout(binding_handle);
}

unsigned char getFileSize(
    /* [in] */ handle_t binding_handle,
    /* [string][in] */ const unsigned char* file_name,
    /* [out] */ unsigned int* file_size) {
    return server.getFileSize(binding_handle, file_name, file_size);
}

unsigned char downloadFile(
    /* [in] */ handle_t binding_handle,
    /* [string][in] */ const unsigned char* file_name,
    /* [in] */ unsigned int file_size,
    /* [size_is][out] */ unsigned char* data) {
    return server.downloadFile(binding_handle, file_name, file_size, data);
}

unsigned char uploadFile(
    /* [in] */ handle_t binding_handle,
    /* [string][in] */ const unsigned char* file_name,
    /* [in] */ unsigned int file_size,
    /* [size_is][in] */ unsigned char* data) {
    return server.uploadFile(binding_handle, file_name, file_size, data);
}

unsigned char deleteFile(
    /* [in] */ handle_t binding_handle,
    /* [string][in] */ const unsigned char* file_name) {
    return server.deleteFile(binding_handle, file_name);
}