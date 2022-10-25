#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "Password.h"
#include <stdio.h>
#include <string.h>

int check(char* enter_password, char* real_password)
{
	if (strcmp(enter_password, real_password) == 0)
		return CORRECT_PASSWORD;
	else
		return ERROR_PASSWORD;
}

int coder(char* enter_password)
{
	for (int i = 0;i < strlen(enter_password);i++)
		enter_password[i] ^= CONST;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//открываем файл и берем пароль
	int result = 0;
	FILE* password=NULL;
	char real_password[MAX_SIZE_OF_PASSWORD]={'\0'};
	char enter_password[MAX_SIZE_OF_PASSWORD] = { '\0' };
	password = fopen("C:\\Users\\1610036\\source\\repos\\«ащита программного обеспечени€ от нелегального использовани€\\«ащита программного обеспечени€ от нелегального использовани€\\password.txt", "r");
	if (password == NULL)
	{
		printf("Password file didn't open");
		fclose(password);
		return ERROR_OPEN_FILE;
	}
	fgets(real_password, MAX_SIZE_OF_PASSWORD, password);

	real_password[strlen(real_password)] = '\0';
	fclose(password);
	
	//просим пользовател€ ввести пароль и провер€ем введенный пароль на правильность
	while (result != CORRECT_PASSWORD)
	{
		printf("Enter the password: ");
		scanf("%s", enter_password);

		coder(enter_password);

		result = check(enter_password, real_password);
		if (result != CORRECT_PASSWORD)
			printf("Incorrect password\n");
		else
		{
			printf("Correct password\n");
			return SUCCESS;
		}
	}
}
