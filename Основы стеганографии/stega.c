#define _CRT_SECURE_NO_WARNINGS

#include "stega.h"
#include <stdio.h>
#include <Windows.h>

unsigned char* result_text;
int length_of_result_text=0;

void binary_text(int number)
{
	int i = 0;
	result_text = (unsigned char*)realloc(result_text, length_of_result_text + 8 * sizeof(unsigned char));//��������� ������
	for (i = 0;i < 8;i++)
	{
		result_text[length_of_result_text++] = (unsigned char)(number % 2);
		number /= 2;
	}
}

int coder()
{
	int i=0,j=0;
	char name_of_picture[SIZE_OF_NAME] = { 0 };
	char name_of_text[SIZE_OF_NAME] = { 0 };
	printf("������� ��� �����-��������: ");
	scanf("%s", name_of_picture);
	FILE* picture = fopen(name_of_picture, "r");							//���� ����� ����� � ��������� � �������� ����� ��� ������
	if (picture == NULL)
	{
		fclose(picture);
		return FOPEN_ERROR;
	}

	printf("������� ��� �����-��������, ���� ����� ���������� �����: ");	
	scanf("%s", name_of_picture);
	FILE* changed_picture = fopen("cryptopicture.bmp", "r+");				//���� ����� ����� ��������, ���� ����� ���������� �����, � �������� ����� ��� ������ � ��������������
	if (changed_picture == NULL)
	{
		fclose(picture);
		fclose(changed_picture);
		return FOPEN_ERROR;
	}
	
	printf("������� ��� ����� � ��������� ����������, ������� ����� ����������� � ��������: ");
	scanf("%s", name_of_text);
	FILE* text = fopen(name_of_text,"r");
	if (text == NULL)
	{																									//���� ����� ����� � ��������� ����������, ������� ���������� �����������
		fclose(picture);
		fclose(changed_picture);
		fclose(text);
		return FOPEN_ERROR;
	}
	fseek(text, 0, SEEK_END);//������������� ������� � ����� �����
	int lsize = ftell(text);//�������� ������ � ������
	printf("������: %d\n", lsize*BIT_IN_BYTE);
	unsigned char* crypto = (unsigned char*)malloc(lsize*sizeof(unsigned char));//�������� ������
	rewind(text);//������ ������� � ������ �����
	fread(crypto, sizeof(unsigned char), lsize, text);//��������� �� ����� � ������ ��������
	fseek(picture, 0, SEEK_END);//������������� ������� � ����� �����
	int picture_size = ftell(picture);//�������� ������ � ������
	printf("���������: ");
	for (i = 0;i < lsize;i++)
		printf("%c", crypto[i]);																		//����� ���������, ������� ����� �����������
	printf("\n");
	if (lsize == 0)
	{
		printf("��������� �����������");
		return ZERO_SIZE_OF_MESSAGE;
	}
	for (i = 0;i < lsize;i++) binary_text((int)crypto[i]);												//������� ��������� � �������� ���

	printf("������� ������� ��������: ");																//������� ��� �� ���� ����� �����������
	int degree = 0;
	scanf("%d", &degree);
	if (degree == 0 || degree > 8)
	{
		printf("������� �������� �� ����� ��������� ��������� ���� ��������\n"
				"������ �������� ���������: %d\n"
				"������������ ����������� ���������������: %d\n", length_of_result_text,(picture_size-54)* BIT_IN_BYTE);//�������� ����� 8 �� 8 ��� ��������
		fclose(text);
		fclose(picture);
		fclose(changed_picture);
		fclose(text);
		free(crypto);
		return ERROR_DEGREE;
	}

	/*� ����� .bmp ������� ���� BITMAPFILEHEADER - ���� ������(2 �����), ����� ������ �����(4 �����), ����� ����������������� ����(4 �����), ������� �� ������������
	����� �������� ���������� ���������� ������ (4 �����)*/ 
	/*����� ���� ���� BITMAPINFOHEADER - ������� ���� ������ ��������� (4 �����), ����� ������(4 �����) � ������ (4 �����) �����������
	����� ����� ���������� (2 �����), ����� ���������� ��� �� �������(2 �����), ����� ������(4 �����), ������ ���������� ������ (4 �����)
	���������� �������� �� ���� �� ��(4 �����), ���������� �������� �� ���� �� �Y(4 �����), ���������� ������ � �������(4 �����), ���������� ������ ������(4 �����), ������� �� ������������*/

	if (length_of_result_text % degree)//���������, ������� ��� ���������� ����� ��������� � ��������� �������
	{
		int extrasize = degree - length_of_result_text % degree;
		printf("��� �����: %d\n", extrasize);
		result_text = (unsigned char*)realloc(result_text, length_of_result_text + extrasize * sizeof(unsigned char));//��������� ���������, ����� � ��������� ����� ����� ���� ������ ���������� ���
		for (i = 0;i < extrasize;i++) result_text[length_of_result_text++] = (unsigned char)0;
	}

	unsigned char k='\0';
	if ((picture_size - BMP_INFO) < length_of_result_text/degree || length_of_result_text >= BYTE_RANGE * BYTE_RANGE)
	{
		printf("������ ��������� �� ����� ���� ��������� � ����\n"
			"������ �������� ���������: %d\n"
			"������������ ����������� ���������������: %d\n", length_of_result_text, (picture_size-54) * BIT_IN_BYTE);//�������� ����� 8 �� ������ ��� ��������
		fclose(text);
		fclose(picture);
		fclose(changed_picture);
		fclose(text);
		free(crypto);
		return ERROR_SIZE_OF_MESSAGE;
	}

	printf("��������� ����� ���� ���������\n");
	rewind(picture);//������������� ������� � ������ �����
	for (i = 0;i < picture_size;i++)																				//����������� ��������
	{
		fscanf(picture, "%c", &k);//����� �� �����
		fprintf(changed_picture, "%c", k);//��������� � ����� ����
	}
	printf("�������� �����������\n");	

	printf("� ��������� ������� �������������� ���� ��� ����������� ����������\n");
	fseek(changed_picture, 6, SEEK_SET);//������������� ������ �� �������������� ����������������� ����
	fprintf(changed_picture, "%c", degree);//�������� ������� ��������
	fprintf(changed_picture, "%c", length_of_result_text / BYTE_RANGE);//�������� ������, �������� �� 256
	fprintf(changed_picture, "%c", length_of_result_text % BYTE_RANGE);//�������� ������� �������
	printf("������ � ������������� ��������� ������� � ����\n");

	fseek(picture, BMP_INFO, SEEK_SET);//������������� ������ �� ������ �����������
	fseek(changed_picture, BMP_INFO, SEEK_SET);//������������� ������ �� ������ �����������
	printf("���� ����� � ���������� � ���� ���������\n");
	
	for (i = 0;i < length_of_result_text;)
	{
		fscanf(picture, "%c", &k);//��������� �������
		k >>= degree;//������� ������� ����
		for (j = 0;j < degree;j++)//���������� ������� ���� �� �������� �������������� ���������
		{
			k <<= 1;
			k += result_text[i++];
		}
		fprintf(changed_picture, "%c", k);//��������� ���������� �������
	}

	printf("��������� ������� �����������\n");
	
	fclose(text);
	fclose(picture);
	fclose(changed_picture);
	fclose(text);
	free(crypto);
	printf("��������� � ������� �������������\n");
	for (i = 0;i < length_of_result_text;i++)								//����� �������������� ��������� � �������� ����
	{
		if (i % 8 == 0) printf(" ");
		printf("%d", result_text[i]);
	}
	return SUCCESS;
}

int decoder()
{
	char name_of_picture[SIZE_OF_NAME] = { 0 };
	char name_of_text[SIZE_OF_NAME] = { 0 };
	printf("������� ��� �����-��������: ");
	scanf("%s", name_of_picture);
	int memory1 = 0, memory2 = 0, memory=0, degree=0;
	int i = 0, j = 0, z=0;
	unsigned char k = '\0';
	FILE* changed_picture = fopen(name_of_picture, "r");								//�������� ����� �������� � ������������� ����������
	if (changed_picture == NULL)
	{
		fclose(changed_picture);
		return FOPEN_ERROR;
	}
	printf("������� ��� �����, ���� ����� ��������� �������������� ���������: ");
	scanf("%s", name_of_text);
	FILE* text = fopen(name_of_text, "w");
	if (text == NULL)																	//�������� ����� ��� ���������� ���������
	{
		fclose(changed_picture);
		fclose(text);
		return FOPEN_ERROR;
	}
	fseek(changed_picture, 6, SEEK_SET);//������������� ������ �� �������������� ����������������� ����
	fscanf(changed_picture, "%c", &degree);//��������� ������� ��������
	if (!degree)
	{
		fclose(changed_picture);
		fclose(text);
		return ZERO_SIZE_OF_MESSAGE;
	}
	printf("������� ��������: %d\n",degree);
	fscanf(changed_picture, "%c", &memory1);//��������� ������, �������� �� 256
	fscanf(changed_picture, "%c", &memory2);//��������� ������� �������
	memory = memory1 * BYTE_RANGE + memory2;//������ ������ ���������
	printf("������ ���������=%d\n", memory);
	unsigned char* crypto = (unsigned char*)malloc(memory * sizeof(unsigned char));//�������� ������ ��� ���������� �������������
	unsigned char* decode_message = (unsigned char*)malloc((memory * sizeof(unsigned char))/ BIT_IN_BYTE);//�������� ������ ��� ��������� � ���������� ����
	memset(decode_message, 0, (memory * sizeof(unsigned char))/BIT_IN_BYTE);
	memset(crypto, 0, memory * sizeof(unsigned char));
	fseek(changed_picture, BMP_INFO, SEEK_SET);//������������� ������ �� ������ �����������
	for (i = 0;i < memory;)
	{
		fscanf(changed_picture, "%c", &k);//��������� �������
		for (j = 0;j < degree;j++)//��������� ����������
		{
			crypto[i++] = k % 2;										//��������� ������� ��� � ������ ����� ��� ���������� ���������� �������� ����
			k >>= 1;
		}
	}
	
	for (i = 0;i < memory/degree;i++)//���������� �����/��������...
		for (j = degree*i,z=0;z <degree/2;z++)//������ ������ �� ������ ������/��������...
		{
			unsigned char t = crypto[j+z];														//������������� ���������� ��� �� ������� �����, ��� ��� ���������� ����������� ��������
			crypto[j+z] = crypto[j+degree - z-1];//����� ����� ����
			crypto[j+ degree - z-1] = t;//������ ����� ����
		}
	printf("��������� ������� ����� �� �����������\n");
	printf("��������� � ������� �������������\n");
	for (i = 0;i < memory;i++)
	{																							//����� ��������� � ������� �������������
		if (i % 8 == 0) printf(" ");
		printf("%d", crypto[i]);
	}
	z = 0;
	for (i = 0;i < memory/8;i++)
	{
		for (j = BIT_IN_BYTE-1;j >=0;j--)
		{
			decode_message[z] += crypto[i*8+j];
			if (j!=0) decode_message[z] <<= 1;													//������� �� 8 ��� � ������������ �����
		}
		z++;
	}
	printf("\n��������� ������� ������������\n");
	printf("�������������� ��������� ");
	for (i = 0;i < memory / 8;i++)
	{
		printf("%c", decode_message[i]);														//����� ��������������� ���������
		fprintf(text, "%c", decode_message[i]);
	}
	free(decode_message);
	free(crypto);
	return SUCCESS;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	printf("�������� ������ ���������:\n"
		"1) ���������-�����\n"
		"2) ���������-�������\n");
	int choose = 0;
	scanf("%d", &choose);
	if (choose == 1)
	{
		int result = coder();
		if (result != SUCCESS) return result;
	}
	else
	{
		int result = decoder();
		if (result != SUCCESS) return result;
	}
}