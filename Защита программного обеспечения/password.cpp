#include "password.h"
#include <fstream>
#include <cassert>
#include <iostream>

using namespace std;

string gPassword = "";

void create_serial_number() {
	string fname = "yoxckf$~r~";
	fname = get_string(fname);
	ofstream output(fname);
	assert(output);
	string serial = "AOS.";
	for (int i = 0; i < 10; i++) {
		int act = rand();
		if (act % 2 == 0)
			serial += (char)(65 + rand() % 26) ^ CONST;
		else 
			serial += (char)(48 + rand() % 10) ^ CONST;
	}
	
	serial += "A";
	output << serial;
}

string get_string(string str) {
	for (int i = 0; i < str.length(); i++)
		str[i] ^= CONST;
	return str;
}

string type_data() {
	char data[31];
	cin.getline(data, 30, '\n');
	string str(data);
	return str;
}

void print_info(string info) {
	cout << info << endl;
}

int check_CRC() {
	unsigned char *ptr = (unsigned char*)get_password;
	long long crc = 0;
	for (int i = 0; i < 691; i++) {
		crc += (long long)(*((unsigned char*)ptr));
		ptr++;
	}
	//cout << crc;
	if (crc == 66095)
		return 1;
	else return 0;
}

int check_password() {
	string password, realPassword;
	string info5 = "Xcmb~*zkyy}exn";
	info5 = get_string(info5);
	while (1) {
		string info1 = "Q W*Od~ox*k*zkyy}exn0*";
		info1 = get_string(info1);
		print_info(info1);
		gPassword = type_data();
		string cpygPassword = "{\"password\":\"" + gPassword + "\"}";
		string realgPassword = gPassword;
		gPassword = cpygPassword;
		if (get_password())
			print_info(info5);
		gPassword = realgPassword;
		if (gPassword == "1qaz@WSX3edc$RFV")
			cout << "Right password";
		if (!get_password()) {
			string info3 = "Q+W*]xedm*zkyy}exn&*~szo*('odn(*~e*orc~*ex*kds*e~box*~e*~xs*kmkcd0*";
			info3 = get_string(info3);
			print_info(info3);
			string end;
			end = type_data();
			string info4 = "'odn";
			info4 = get_string(info4);
			if (end == info4)
				return 0;
			if (gPassword == "I will be back!@#123") {
				print_info(info5);
			}
		}
		else
			return 1;
	}
}

//00422C90 - начало 4336784 - в десятичной\
00422F43 - конец 4337475 - в десятичной\
691 - количество байт для функции
int get_password() {
	string fname = "zkyy}exn$~r~";
	fname = get_string(fname);
	ifstream input(fname);
	string realPassword;
	assert(input);
	input >> realPassword;
	realPassword = get_string(realPassword);
	if (gPassword == realPassword) {
		create_serial_number();
		return 1;
	}
	return 0;
}