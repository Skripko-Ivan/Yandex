#define _CRT_SECURE_NO_WARNINGS

#include "stega.h"
#include <stdio.h>
#include <Windows.h>

unsigned char* result_text;
int length_of_result_text=0;

void binary_text(int number)
{
	int i = 0;
	result_text = (unsigned char*)realloc(result_text, length_of_result_text + 8 * sizeof(unsigned char));//добавляем памяти
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
	printf("Введите имя файла-картинки: ");
	scanf("%s", name_of_picture);
	FILE* picture = fopen(name_of_picture, "r");							//ввод имени файла с картинкой и открытие файла для чтения
	if (picture == NULL)
	{
		fclose(picture);
		return FOPEN_ERROR;
	}

	printf("Введите имя файла-картинки, куда будет зашифрован текст: ");	
	scanf("%s", name_of_picture);
	FILE* changed_picture = fopen("cryptopicture.bmp", "r+");				//ввод имени файла картинки, куда будет зашифрован текст, и открытие файла для чтения и редактирования
	if (changed_picture == NULL)
	{
		fclose(picture);
		fclose(changed_picture);
		return FOPEN_ERROR;
	}
	
	printf("Введите имя файла с текстовым сообщением, которое будет зашифровано в картинку: ");
	scanf("%s", name_of_text);
	FILE* text = fopen(name_of_text,"r");
	if (text == NULL)
	{																									//ввод имени файла с текстовым сообщением, которое необходимо зашифровать
		fclose(picture);
		fclose(changed_picture);
		fclose(text);
		return FOPEN_ERROR;
	}
	fseek(text, 0, SEEK_END);//устанавливаем позицию в конце файла
	int lsize = ftell(text);//получаем размер в байтах
	printf("размер: %d\n", lsize*BIT_IN_BYTE);
	unsigned char* crypto = (unsigned char*)malloc(lsize*sizeof(unsigned char));//выделяем память
	rewind(text);//ставим позицию в начало файла
	fread(crypto, sizeof(unsigned char), lsize, text);//считываем из файла в массив символов
	fseek(picture, 0, SEEK_END);//устанавливаем позицию в конец файла
	int picture_size = ftell(picture);//получаем размер в байтах
	printf("Сообщение: ");
	for (i = 0;i < lsize;i++)
		printf("%c", crypto[i]);																		//вывод сообщения, которое будет зашифровано
	printf("\n");
	if (lsize == 0)
	{
		printf("Сообщение отсутствует");
		return ZERO_SIZE_OF_MESSAGE;
	}
	for (i = 0;i < lsize;i++) binary_text((int)crypto[i]);												//перевод сообщения в бинарный вид

	printf("Введите степень упаковки: ");																//сколько бит на байт будет шифроваться
	int degree = 0;
	scanf("%d", &degree);
	if (degree == 0 || degree > 8)
	{
		printf("Степень упаковки не может принимать указанное вами значение\n"
				"Размер текущего сообщения: %d\n"
				"Максимальная вместимость стегоконтейнера: %d\n", length_of_result_text,(picture_size-54)* BIT_IN_BYTE);//максимум можно 8 из 8 бит заменить
		fclose(text);
		fclose(picture);
		fclose(changed_picture);
		fclose(text);
		free(crypto);
		return ERROR_DEGREE;
	}

	/*в файле .bmp сначала блок BITMAPFILEHEADER - идет формат(2 байта), затем размер файла(4 байта), затем зарезервированное поле(4 байта), которое не используется
	затем смещение начальнаых пиксельных данных (4 байта)*/ 
	/*затем идет блок BITMAPINFOHEADER - сначала идет размер структуры (4 байта), далее ширина(4 байта) и высота (4 байта) изображения
	Далее число плоскостей (2 байта), далее количество бит на пиксель(2 байта), метод сжатия(4 байта), размер пиксельных данных (4 байта)
	Разрешение пикселей на метр по ОХ(4 байта), разрешение пикселей на метр по ОY(4 байта), колиечство цветов в палитре(4 байта), количество важных цветов(4 байта), которое не используется*/

	if (length_of_result_text % degree)//проверяем, сколько бит информации будет храниться в последнем пикселе
	{
		int extrasize = degree - length_of_result_text % degree;
		printf("Доп место: %d\n", extrasize);
		result_text = (unsigned char*)realloc(result_text, length_of_result_text + extrasize * sizeof(unsigned char));//дополняем сообщение, чтобы в последнем байте также было нужное количество бит
		for (i = 0;i < extrasize;i++) result_text[length_of_result_text++] = (unsigned char)0;
	}

	unsigned char k='\0';
	if ((picture_size - BMP_INFO) < length_of_result_text/degree || length_of_result_text >= BYTE_RANGE * BYTE_RANGE)
	{
		printf("Данное сообщение не может быть вставлено в файл\n"
			"Размер текущего сообщения: %d\n"
			"Максимальная вместимость стегоконтейнера: %d\n", length_of_result_text, (picture_size-54) * BIT_IN_BYTE);//максимум можно 8 из восьми бит заменить
		fclose(text);
		fclose(picture);
		fclose(changed_picture);
		fclose(text);
		free(crypto);
		return ERROR_SIZE_OF_MESSAGE;
	}

	printf("Сообщение может быть вставлено\n");
	rewind(picture);//устанавливаем позицию в начало файла
	for (i = 0;i < picture_size;i++)																				//копирование картинки
	{
		fscanf(picture, "%c", &k);//берем из файла
		fprintf(changed_picture, "%c", k);//вставляем в новый файл
	}
	printf("Картинка скопирована\n");	

	printf("В сообщение внесены дополнительные биты для корректного шифрования\n");
	fseek(changed_picture, 6, SEEK_SET);//устанавливаем курсор на неиспользуемое зарезервированное поле
	fprintf(changed_picture, "%c", degree);//сообщаем степень упаковки
	fprintf(changed_picture, "%c", length_of_result_text / BYTE_RANGE);//сообщаем размер, деленный на 256
	fprintf(changed_picture, "%c", length_of_result_text % BYTE_RANGE);//сообщаем остаток размера
	printf("Данные о зашифрованном сообщении внесены в файл\n");

	fseek(picture, BMP_INFO, SEEK_SET);//устанавливаем курсор на начало изображения
	fseek(changed_picture, BMP_INFO, SEEK_SET);//устанавливаем курсор на начало изображения
	printf("Файл готов к шифрованию в него сообщения\n");
	
	for (i = 0;i < length_of_result_text;)
	{
		fscanf(picture, "%c", &k);//считываем пиксель
		k >>= degree;//убираем младшие биты
		for (j = 0;j < degree;j++)//возвращаем младшие биты со значения зашифрованного сообщения
		{
			k <<= 1;
			k += result_text[i++];
		}
		fprintf(changed_picture, "%c", k);//вставляем измененный пиксель
	}

	printf("Сообщение успешно зашифровано\n");
	
	fclose(text);
	fclose(picture);
	fclose(changed_picture);
	fclose(text);
	free(crypto);
	printf("Сообщение в битовом представлении\n");
	for (i = 0;i < length_of_result_text;i++)								//вывод зашифрованного сообщения в бинарном виде
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
	printf("Введите имя файла-картинки: ");
	scanf("%s", name_of_picture);
	int memory1 = 0, memory2 = 0, memory=0, degree=0;
	int i = 0, j = 0, z=0;
	unsigned char k = '\0';
	FILE* changed_picture = fopen(name_of_picture, "r");								//открытие файла картинки с зашифрованным сообщением
	if (changed_picture == NULL)
	{
		fclose(changed_picture);
		return FOPEN_ERROR;
	}
	printf("Введите имя файла, куда будет сохранено расшифрованное сообщение: ");
	scanf("%s", name_of_text);
	FILE* text = fopen(name_of_text, "w");
	if (text == NULL)																	//открытие файла для сохранения сообщения
	{
		fclose(changed_picture);
		fclose(text);
		return FOPEN_ERROR;
	}
	fseek(changed_picture, 6, SEEK_SET);//устанавливаем курсор на неиспользуемое зарезервированное поле
	fscanf(changed_picture, "%c", &degree);//считываем степень упаковки
	if (!degree)
	{
		fclose(changed_picture);
		fclose(text);
		return ZERO_SIZE_OF_MESSAGE;
	}
	printf("Степень упаковки: %d\n",degree);
	fscanf(changed_picture, "%c", &memory1);//считываем размер, деленный на 256
	fscanf(changed_picture, "%c", &memory2);//считываем остаток размера
	memory = memory1 * BYTE_RANGE + memory2;//полный размер сообщения
	printf("Размер сообщения=%d\n", memory);
	unsigned char* crypto = (unsigned char*)malloc(memory * sizeof(unsigned char));//выделяем память для побитового представления
	unsigned char* decode_message = (unsigned char*)malloc((memory * sizeof(unsigned char))/ BIT_IN_BYTE);//выделяем память для сообщения в нормальном виде
	memset(decode_message, 0, (memory * sizeof(unsigned char))/BIT_IN_BYTE);
	memset(crypto, 0, memory * sizeof(unsigned char));
	fseek(changed_picture, BMP_INFO, SEEK_SET);//устанавливаем курсор на начало изображения
	for (i = 0;i < memory;)
	{
		fscanf(changed_picture, "%c", &k);//считываем пиксель
		for (j = 0;j < degree;j++)//считываем информацию
		{
			crypto[i++] = k % 2;										//считываем младший бит и делаем сдвиг для считывания следующего младшего бита
			k >>= 1;
		}
	}
	
	for (i = 0;i < memory/degree;i++)//количество троек/четверок...
		for (j = degree*i,z=0;z <degree/2;z++)//ставим индекс на начало тройки/четверки...
		{
			unsigned char t = crypto[j+z];														//разворачиваем количество бит из каждого байта, так как шифрование происходило наоборот
			crypto[j+z] = crypto[j+degree - z-1];//левая часть пары
			crypto[j+ degree - z-1] = t;//правая часть пары
		}
	printf("Сообщение успешно взято из изображения\n");
	printf("Сообщение в битовом представлении\n");
	for (i = 0;i < memory;i++)
	{																							//вывод сообщения в битовом представлении
		if (i % 8 == 0) printf(" ");
		printf("%d", crypto[i]);
	}
	z = 0;
	for (i = 0;i < memory/8;i++)
	{
		for (j = BIT_IN_BYTE-1;j >=0;j--)
		{
			decode_message[z] += crypto[i*8+j];
			if (j!=0) decode_message[z] <<= 1;													//берется по 8 бит и составляется буква
		}
		z++;
	}
	printf("\nСообщение успешно расшифровано\n");
	printf("Расшифрованное сообщение ");
	for (i = 0;i < memory / 8;i++)
	{
		printf("%c", decode_message[i]);														//вывод расшифрованного сообщения
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

	printf("Выберете нужную программу:\n"
		"1) Программа-кодер\n"
		"2) Программа-декодер\n");
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