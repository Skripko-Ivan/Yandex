#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include <stdio.h>
#include <windows.h>

typedef struct data//информация о книге
{
	int year, price, rating;//год/цена/оценка читателей
	char book[NAME_LENGTH], author[NAME_LENGTH], publisher[NAME_LENGTH], genre[NAME_LENGTH], descr[DECR_LENGTH];//название/автор/издательство/жанр/описание
} data;

data* mass[MAX_BOOKS] = { NULL };
int number_of_book = 0;

typedef struct mytreename 
{
	struct mytreename* left, * right;//указатели на левое/правое поддерево и на родителя
	unsigned char height;//высота дерева
	int key;//ключ для чисел
	char* strkey;//ключ для строк
	data* book;
} mytree;

mytree* root_book = NULL;//дерево для поиска по названию книги
mytree* root_author = NULL;//дерево для поиска по автору книги
mytree* root_publisher = NULL;//дерево для поиска по издательству книги
mytree* root_genre = NULL;//дерево для поиска по жанру книги
mytree* root_rating = NULL;//дерево для поиска по рейтингу книги
mytree* root_price = NULL;//дерево для поиска по цене книги
mytree* root_year = NULL;//дерево для поиска по году издания книги

unsigned char height(mytree* p)//функция узнает высоту дерева, которая установлена в узле
{
	return p ? p->height : 0;//если высота не 0, то возвращается высота, иначе 0
}

char bfactor(mytree* p)//вычисляет разницу между высотами правого и левого поддерева
{
	return (height(p->right) - height(p->left));
}

void fixheight(mytree* p)//восстанавливает корректное значение поля height
{
	unsigned char hl = height(p->left);//узнаем высоту левого поддерева
	unsigned char hr = height(p->right);//узнаем высоту правого поддерева
	p->height = (hl > hr ? hl : hr) + 1;//устанавливаем высоту больше и прибавляем к результату 1 (правое/левое поддерево также узел)
}

//Данное вращение используется тогда, когда высота p-left-поддерева = 2 и высота p-right-right-поддерева <= высота p-right-left-поддерева.
mytree* rotateright(mytree* p) // малый правый поворот вокруг p
{
	mytree* q = p->left;//берем левое поддерево
	p->left = q->right;//на место левого поддерева ставим p-left-right
	q->right = p;//на место правого сына взятого поддерева ставим корень
	fixheight(p);//корень опустился, поэтому меняем ему высоту
	fixheight(q);//новый корень, также устанавливаем ему высоту
	return q;//возвращаем указатель на вершину измененного дерева
}

//Данное вращение используется тогда, когда высота p-right-поддерева = 2 и высота p-right-left <= высота p-right-right
mytree* rotateleft(mytree* q) // левый поворот вокруг q
{
	mytree* p = q->right;//берем правое поддерево
	q->right = p->left;//на место правого поддерева ставим p-right-left
	p->left = q;//ставим на место левого сына взятого поддерева корень дерева
	fixheight(q);//устанавливаем корректные значения высот
	fixheight(p);
	return p;//возвращаем указатель на новый корень дерева
}

mytree* balance(mytree* p) //балансировка узла p
{
	fixheight(p);//устанавливаем корректное значение высоты, данная операция нужна, так как функция вызывается после действий, так что значения могут быть не правильные
	if (bfactor(p) == 2)//если нужно выполнить левый поворот
	{
		if (bfactor(p->right) < 0)//если нужно выполнить большое левое вращение
			p->right = rotateright(p->right);//правый потомок делает малое правое вращение
		return rotateleft(p);//делается левое малое вращение, так как теперь условие для выполнения малого левого вращения выполняется
	}
	if (bfactor(p) == -2)//если нужно выполнить правое вращение
	{
		if (bfactor(p->left) > 0)//если нужно выполнить правое большое вращение
			p->left = rotateleft(p->left);//левый сын делает левое малое вращение
		return rotateright(p);//делается правое малое вращение, так как теперь условие для выполнения малого правого вращения выполняется
	}
	return p; //балансировка не нужна
}

mytree* insert(mytree* p, int k, char* kk, data* book) // вставка ключа k или kk о книге book в дерево с корнем p
{														//kk - массив, k - число
	if (!p)//если прошли все дерево и находимся на месте листа или это первый элемент дерева
	{
		mytree* new = (mytree*)malloc(sizeof(mytree));//создаем структуру под дерево
		if (new == NULL) exit(MALLOC_ERROR);
		new->height = 1;//устанавливаем высоту листу
		if (kk == NULL)//проверяем какой ключ
			new->key = k;//устанавливаем значение числового ключа
		else new->strkey = kk;
		new->book = book;//добавляем в дерево указатель на книгу
		new->right = NULL;
		new->left = NULL;
		return new;//возвращаем указатель на созданную структуру
	}
	if (kk == NULL) if (k < p->key)//если ключ меньше корня дерева
		p->left = insert(p->left, k, NULL, book);//тогда идем влево
	else//если ключ больше либо равен
		p->right = insert(p->right, k, NULL, book);//идем вправо
	else if (strcmp(kk, p->strkey) < 0)//если ключ в виде строки
		p->left = insert(p->left, k, kk, book);//идем влево
	else p->right = insert(p->right, k, kk, book);//идем вправо
	return balance(p);//балансируем каждую вершину после вставки
}

mytree* findmin(mytree* p) // поиск узла с минимальным ключом в дереве p 
{
	return p->left ? findmin(p->left) : p;//идем до минимума, по свойству АВЛ-дерева он слева
}

mytree* removemin(mytree* p) // удаление узла с минимальным ключом из дерева p
{
	if (p->left == 0)//если левого поддерева нет, то минимум найден и возвращаем правое, чтобы поставить на его место
		return p->right;
	p->left = removemin(p->left);//в противном случае продолжаем идти до минимума в дереве
	return balance(p);//после удаления элемента производим балансировку
}

mytree* removee(mytree* p, int k, char* kk, data* book)// удаление ключа k или kk из дерева p книги book
{
	if (!p) return NOT_FOUND;//если не найден
	if (kk==NULL)
	if (k < p->key)//если меньше чем в корне
		p->left = removee(p->left, k,kk,book);//идем налево
		else if (k > p->key)//если больше, то направо
			p->right = removee(p->right, k,kk,book);
	else //  k == p->key 
	{
		if (p->book == book)//если найденный элемент является элементом книги, которую удаляем
		{
			mytree* q = p->left;//запоминаем левое поддерево
			mytree* r = p->right;//запоминаем правое поддерево
			free(p);//удаляем найденный элемент
			if (!r) return q;//если правого поддерева нет, то возвращаем указатель на левое поддерево
			mytree* min = findmin(r);//если правое поддерево есть, то находим минимум в правом поддереве
			min->right = removemin(r);//ставим найденный элемент на место удаленного
			min->left = q;
			return balance(min);//производим балансировку
		}
		else
		{
			p->right = removee(p->right, k, kk, book);//если существует несколько книг с таким полем, ищем справа
			p->left = removee(p->left, k, kk, book);//ищем слева
		}
	}
	else//удаление по строке
	{
		if (strcmp(kk, p->strkey) < 0)//если меньше чем в корне
			p->left = removee(p->left, k, kk,book);//идем налево
		else if (strcmp(kk, p->strkey) > 0)//если больше, то направо
			p->right = removee(p->right, k, kk,book);
		else //  k == p->strkey 
		{
			if (p->book == book)
			{
				mytree* q = p->left;//запоминаем левое поддерево
				mytree* r = p->right;//запоминаем правое поддерево
				free(p);//удаляем найденный элемент
				if (!r) return q;//если правого поддерева нет, то возвращаем указатель на левое поддерево
				mytree* min = findmin(r);//если правое поддерево есть, то находим минимум в правом поддереве
				min->right = removemin(r);//ставим найденный элемент на место удаленного
				min->left = q;
				return balance(min);//производим балансировку
			}
			else
			{
				p->right = removee(p->right, k, kk, book);//ищем справа 
				p->left = removee(p->left, k, kk, book);//идем налево
			}
		}
	}
	return balance(p);//производим балансировку каждой вершины
}

mytree* find(mytree* p, int k, char* kk)//поиск по дереву p по одному из ключей k или kk, ищет первое вхождение в дереве
{
	if (!p) return NOT_FOUND;//если не найден
	if (kk == NULL)//если дерево ищет по числам
		if (k < p->key)//если меньше чем в корне
			find(p->left, k, kk);//идем налево
		else if (k > p->key)//если больше, то направо
			find(p->right, k, kk);
		else return p;//если нашли
	else if (strcmp(kk, p->strkey) < 0) find(p->left, k, kk);//если строковый ключ
		else if (strcmp(kk, p->strkey) > 0) find(p->right, k, kk);
			else return p;//возвращаем указатель на книгу 
}
void add();//функция добавления книги
void change();//редактирование книги
void delete();//функция удаления книги
void free_roots(mytree* root);//освобождение памяти деревьев
void free_books();//освобождение памяти структур книг
void finder();//поиск по базе данных
void info(data* book);
void finderstr(mytree* book, char* action, int* check);
void findernumber(mytree* book, int action, int* check);
void sort();
void forsort(mytree* tree);//функция, которая сделает обход дерева и выведет всю информацию в отсортированном порядке
void save();//функция для сохранения бд в файл
void forsave(FILE* file,mytree* bd);
void bd_from_file();
void forbd(FILE* bd);

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	while (1)
	{
		menu();
		int act = 0;
		scanf("%d", &act);
		switch (act)
		{
		case ADD_BOOK:
		{
			add();
			break;
		}
		case DELETE_BOOK:
		{
			delete();
			break;
		}
		case CHANGE_BOOK:
		{
			change();
			break;
		}
		case FIND:
		{
			finder();
			break;
		}
		case SORT:
		{
			sort();
			break;
		}
		case SAVE:
		{
			save();
			break;
		}
		case BD_FROM_FILE:
		{
			bd_from_file();
			break;
		}
		default: 
		{
			free_roots(root_year);//освобождение памяти от деревьев для поиска
			free_roots(root_price);
			free_roots(root_rating);
			free_roots(root_genre);
			free_roots(root_author);
			free_roots(root_book);
			free_roots(root_publisher);
			free_books();//освобождение памяти от книг
			return;
		}
		}
	}
}

void add()//добавление книги в дерево с корнем p
{
	data* first = (data*)malloc(sizeof(data));
	if (first == NULL) exit(MALLOC_ERROR);
	mass[number_of_book] = first;//сохраняем указатель, чтобы потом очистить всю выделенную память
	number_of_book++;//увеличиваем количество книг
	getchar();
	while (1)
	{
		printf("Введите имя книги: ");
		char book_name[NAME_LENGTH] = { '0' };
		fgets(book_name, NAME_LENGTH, stdin);
		book_name[strlen(book_name) - 1] = '\0';
		if (!strlen(book_name))
		{
			printf("Некорректный вид (поле не может быть пустым). Попробуйте еще раз.\n");
			continue;
		}
		strcpy(first->book, book_name);
		break;
	}
	while (1)
	{
		printf("Введите автора: ");
		char book_author[NAME_LENGTH] = { '0' };
		fgets(book_author, NAME_LENGTH, stdin);				//пользователь вводит данные о книге
		book_author[strlen(book_author) - 1] = '\0';
		if (!strlen(book_author))
		{
			printf("Некорректный вид (поле не может быть пустым). Попробуйте еще раз.\n");
			continue;
		}
		strcpy(first->author, book_author);
		break;
	}
	while (1)
	{
		printf("Введите жанр книги: ");
		char book_genre[NAME_LENGTH] = { '0' };
		fgets(book_genre, NAME_LENGTH, stdin);
		book_genre[strlen(book_genre) - 1] = '\0';
		if (!strlen(book_genre))
		{
			printf("Некорректный вид (поле не может быть пустым). Попробуйте еще раз.\n");
			continue;
		}
		strcpy(first->genre, book_genre);
		break;
	}
	while (1)
	{
		printf("Введите год выпуска: ");
		int year = 0;
		scanf("%d", &year);
		if (year < 0)
		{
			printf("Некорректно указан год (отрицательный год). Попробуйте еще раз.\n");
			continue;
		}
		first->year = year;
		break;
	}
	while (1)
	{
		printf("Введите оценку читателей (по шкале от 0 до 10): ");
		int rating = 0;
		scanf("%d", &rating);
		if (rating < 0 || rating>10)
		{
			printf("Некорректно указан рейтинг. Попробуйте еще раз.\n");
			continue;
		}
		first->rating = rating;
		break;
	}
	while (1)
	{
		printf("Введите цену книги: ");
		int price = 0;
		scanf("%d", &price);
		if (price < 0)
		{
			printf("Некорректно указана цена (отрицательная цена). Попробуйте еще раз.\n");
			continue;
		}
		first->price = price;
		break;
	}
	getchar();
	while (1)
	{
		printf("Введите издательство: ");
		char book_publ[NAME_LENGTH] = { '0' };
		fgets(book_publ, NAME_LENGTH, stdin);
		book_publ[strlen(book_publ) - 1] = '\0';
		if (!strlen(book_publ))
		{
			printf("Некорректный вид (поле не может быть пустым). Попробуйте еще раз.\n");
			continue;
		}
		strcpy(first->publisher, book_publ);
		break;
	}
	while (1)
	{
		printf("Введите описание книги: ");
		char book_descr[DECR_LENGTH] = { '0' };
		fgets(book_descr, DECR_LENGTH, stdin);
		book_descr[strlen(book_descr) - 1] = '\0';
		if (!strlen(book_descr))
		{
			printf("Некорректный вид (поле не может быть пустым). Попробуйте еще раз.\n");
			continue;
		}
		strcpy(first->descr, book_descr);
		break;
	}
	root_year=insert(root_year, first->year,NULL, first);//вставляем в дерево поиска по году
	root_rating =insert(root_rating, first->rating,NULL,first);//вставляем в дерево поиска по рейтингу
	root_price=insert(root_price, first->price,NULL,first);//вставляем в дерево поиска по цене
	root_author = insert(root_author, STR_KEY, first->author, first);//вставляем в дерево поиска по автору
	root_genre = insert(root_genre, STR_KEY, first->genre, first);//вставляем в дерево поиска по жанру
	root_publisher = insert(root_publisher, STR_KEY, first->publisher, first);//вставляем в дерево поиска по издательству
	root_book = insert(root_book, STR_KEY, first->book, first);//вставляем в дерево поиска по названию
}
void delete()
{
	printf("Удаление книги производится по названию.\nВведите название книги, которую хотите удалить: ");
	char name[NAME_LENGTH] = { 0 };
	getchar();
	fgets(name, NAME_LENGTH, stdin);
	name[strlen(name) - 1] = '\0';
	mytree* book=find(root_book, STR_KEY, name);
	if (!book)
	{
		printf("Книга не найдена\n");
		return;
	}
	root_year=removee(root_year, book->book->year,NULL,book->book);
	root_price = removee(root_price, book->book->price, NULL,book->book);
	root_rating = removee(root_rating, book->book->rating, NULL,book->book);
	root_author = removee(root_author, STR_KEY, book->book->author,book->book);
	root_genre = removee(root_genre, STR_KEY, book->book->genre,book->book);
	root_publisher = removee(root_publisher, STR_KEY, book->book->publisher,book->book);
	root_book = removee(root_book, STR_KEY, book->book->book,book->book);
	printf("Удаление завершено\n");
}
void change()
{
	printf("Поиск книги, которую вам нужно отредактировать, производится по названию.\nВведите название книги, которую хотите отредактировать: ");
	char name[NAME_LENGTH] = { 0 };
	getchar();
	fgets(name, NAME_LENGTH, stdin);
	name[strlen(name) - 1] = '\0';
	mytree* book = find(root_book, STR_KEY, name);
	if (!book)
	{
		printf("Книга не найдена\n");
		return;
	}
	while (1)
	{
		change_menu();
		int act = 0;
		scanf("%d", &act);
		switch (act)
		{
		case YEAR:
		{
			int year = 0;
			printf("Текущий год: %d\nВведите год, который хотите установить данной книге: ",book->book->year);
			scanf("%d", &year);
			root_year = removee(root_year, book->book->year, NULL,book);//удаляем из дерева элемент
			book->book->year = year;
			root_year = insert(root_year, book->book->year, NULL, book);//добавляем в дерево элемент
			break;
		}
		case RATING:
		{
			int rating = 0;
			printf("Текущая оценка читателей: %d\nВведите оценку, которую хотите установить данной книге: ", book->book->rating);
			scanf("%d", &rating);
			root_rating = removee(root_rating, book->book->rating, NULL,book);//удаляем из дерева элемент
			book->book->rating = rating;
			root_rating = insert(root_rating, book->book->rating, NULL, book);//добавляем в дерево элемент
			break;
		}
		case PRICE:
		{
			int price = 0;
			printf("Текущая цена: %d\nВведите цену, которую хотите установить данной книге: ", book->book->price);
			scanf("%d", &price);
			root_price = removee(root_price, book->book->price, NULL,book);//удаляем из дерева элемент
			book->book->price = price;
			root_price = insert(root_price, book->book->price, NULL, book);//добавляем в дерево элемент
			break;
		}
		case PUBLISHER:
		{
			char publisher[NAME_LENGTH] = { 0 };
			printf("Текущее издательство: %s\nВведите издательство, которое хотите установить данной книге: ", book->book->publisher);
			getchar();
			fgets(publisher, NAME_LENGTH, stdin);
			publisher[strlen(publisher) - 1] = '\0';
			root_publisher = removee(root_publisher, STR_KEY, book->book->publisher,book);//удаляем из дерева элемент
			strcpy(book->book->publisher,publisher);
			root_publisher = insert(root_publisher, STR_KEY, book->book->publisher, book);//добавляем в дерево элемент
			break;
		}
		case AUTHOR:
		{
			char author[NAME_LENGTH] = { 0 };
			printf("Текущий автор: %s\nВведите автора, которого хотите установить данной книге: ", book->book->author);
			getchar();
			fgets(author, NAME_LENGTH, stdin);
			author[strlen(author) - 1] = '\0';
			root_author = removee(root_author, STR_KEY, book->book->author,book);//удаляем из дерева элемент
			strcpy(book->book->author, author);
			root_author = insert(root_author, STR_KEY, book->book->author, book);//добавляем в дерево элемент
			break;
		}
		case BOOK:
		{
			char book_name[NAME_LENGTH] = { 0 };
			printf("Текущее название книги: %s\nВведите название, которого хотите установить данной книге: ", book->book->book);
			getchar();
			fgets(book_name, NAME_LENGTH, stdin);
			book_name[strlen(book_name) - 1] = '\0';
			root_book = removee(root_book, STR_KEY, book->book->book,book);//удаляем из дерева элемент
			strcpy(book->book->book, book_name);
			root_book = insert(root_book, STR_KEY, book->book->book, book);//добавляем в дерево элемент
			break;
		}
		case GENRE:
		{
			char genre[NAME_LENGTH] = { 0 };
			printf("Текущий жанр: %s\nВведите жанр, который хотите установить данной книге: ", book->book->genre);
			getchar();
			fgets(genre, NAME_LENGTH, stdin);
			genre[strlen(genre) - 1] = '\0';
			root_genre = removee(root_genre, STR_KEY, book->book->genre,book);//удаляем из дерева элемент
			strcpy(book->book->genre, genre);
			root_genre = insert(root_genre, STR_KEY, book->book->genre, book);//добавляем в дерево элемент
			break;
		}
		case DESCR:
		{
			char descr[DECR_LENGTH] = { 0 };
			printf("Текущее описание: %s\nВведите описание, которое хотите установить данной книге: ", book->book->descr);
			getchar();
			fgets(descr, NAME_LENGTH, stdin);
			descr[strlen(descr) - 1] = '\0';
			strcpy(book->book->descr, descr);
			break;
		}
		default: return;
		}
	}
}
void finder()
{
	finder_menu();
	int act = 0;
	scanf("%d", &act);
	switch (act)
	{
	case YEAR:
	{
		int year = 0, check=0;
		printf("Введите год, по которому будет производиться поиск: ");
		scanf("%d", &year);
		mytree* book=root_year;//ставим указатель на начало дерева
		findernumber(book, year, &check);
		if (!check) printf("Ничего не найдено\n");
		break;
	}
	case RATING:
	{
		int rating = 0, check=0;
		printf("Введите рейтинг, по которому будет производиться поиск (от 0 до 10): ");
		scanf("%d", &rating);
		mytree* book = root_rating;//ставим указатель на начало дерева
		findernumber(book, rating, &check);
		if (!check) printf("Ничего не найдено\n");
		break;
	}
	case PRICE:
	{
		int price = 0, check=0;
		printf("Введите цену книги, по которой будет производиться поиск: ");
		scanf("%d", &price);
		mytree* book = root_price;//ставим указатель на начало дерева
		findernumber(book, price, &check);//ищем первое вхождение и выводим информацию о найденной книге, если она найдена
		if (!check) printf("Ничего не найдено\n");
		break;
	}
	case PUBLISHER:
	{
		int check = 0;
		char publisher[NAME_LENGTH] = { 0 };
		printf("Введите издательство, по которому будет производиться поиск: ");
		getchar();
		fgets(publisher, NAME_LENGTH, stdin);
		publisher[strlen(publisher) - 1] = '\0';
		mytree* book = root_publisher;//ставим указатель на начало дерева
		finderstr(book, publisher, &check);
		if (!check) printf("Ничего не найдено\n");
		break;
	}
	case AUTHOR:
	{
		int check = 0;
		char author[NAME_LENGTH] = { 0 };
		printf("Введите автора, по которому будет производиться поиск: ");
		getchar();
		fgets(author, NAME_LENGTH, stdin);
		author[strlen(author) - 1] = '\0';
		mytree* book = root_author;//ставим указатель на начало дерева
		finderstr(book, author, &check);
		if (!check) printf("Ничего не найдено\n");
		break;
	}
	case BOOK:
	{
		int check = 0;
		char book_name[NAME_LENGTH] = { 0 };
		printf("Введите название книги, по которому будет производиться поиск: ");
		getchar();
		fgets(book_name, NAME_LENGTH, stdin);
		book_name[strlen(book_name) - 1] = '\0';
		mytree* book = root_book;//ставим указатель на начало дерева
		finderstr(book, book_name, &check);
		if (!check) printf("Ничего не найдено\n");
		break;
	}
	case GENRE:
	{
		int check = 0;
		char genre[NAME_LENGTH] = { 0 };
		printf("Введите жанр, по которому будет производиться поиск: ");
		getchar();
		fgets(genre, NAME_LENGTH, stdin);
		genre[strlen(genre) - 1] = '\0';
		mytree* book = root_genre;//ставим указатель на начало дерева
		finderstr(book, genre, &check);
		if (!check) printf("Ничего не найдено\n");
		break;
	}
	default: return;
	}
}
void sort()
{
	sort_menu();//пользователь выбирает по какому полю будет сортировка
	int act = 0;
	scanf("%d", &act);
	switch (act)
	{
	case YEAR:
	{
		forsort(root_year);
		break;
	}
	case RATING:
	{
		forsort(root_rating);
		break;
	}
	case PRICE:
	{
		forsort(root_price);
		break;
	}
	case PUBLISHER:
	{
		forsort(root_publisher);
		break;
	}
	case AUTHOR:
	{
		forsort(root_author);
		break;
	}
	case BOOK:
	{
		forsort(root_book);
		break;
	}
	case GENRE:
	{
		forsort(root_genre);
		break;
	}
	default: return;
	}
}
void save()
{
	printf("Будет создан файл, куда сохранится база данных. Дайте имя файлу.\n"
		"Если хотите сохранить базу данных в уже существующий файл, то введите его имя.\n");
	char file_name[NAME_LENGTH] = { 0 };
	getchar();
	fgets(file_name, NAME_LENGTH, stdin);
	file_name[strlen(file_name) - 1] = '\0';
	FILE* file_for_save = fopen(file_name, "w");//создаем файл
	if (file_for_save == NULL)
	{
		printf("Не удалось создать файл.\n");
		return;
	}
	printf("Файл успешно создан.\n");
	forsave(file_for_save,root_book);
	printf("База данных сохранена в файл.\n");
}
void bd_from_file()
{
	printf("Правила формата хранения базы данных в файле:\n"
	"1) Каждое поле книги каходится на отдельной строке\n2) Между книгами не ставятся дополнительных переносов строк\n"
	"3) Поля написаны в следующем порядке: \n\ta) Название книги\n\tб) Автор\n\tв) Оценка пользователей (от 0 до 10)\n\t"
	"г) Жанр\n\tд) Цена\n\tе) Издательство\n\tж) Год издания\n\tз) Описание\nВведите имя файла с базой данных: ");
	char file_name[NAME_LENGTH] = { 0 };
	getchar();
	fgets(file_name, NAME_LENGTH, stdin);
	file_name[strlen(file_name) - 1] = '\0';
	FILE* bd = fopen(file_name, "r");//открываем файл
	if (bd == NULL)
	{
		printf("Не удалось открыть файл.\n");
		return;
	}
	forbd(bd);
	printf("База данных успешно взята из файл.\n");
}

void free_roots(mytree* root)
{
	if (!root) return;//если больше некуда идти, то заканчиваем рекурсию
	if (root->left)//если можно идти еще влево, то идем на максимум
		free_roots(root->left);
	if (root->right)//если уже нельзя идти влево, но можно идти вправо, то идем
		free_roots(root->right);
	//это листья, ни в право, ни влево идти же нельзя, удаляются листья, потом по цепочке все, что выше
	free(root);//удаляем структуру для поиска
}
void free_books()
{
	for (int i = 0;i < number_of_book;i++) free(mass[i]);
}
void info(data* book)
{
	printf("%s, %s, %d, %s, %d, %s, %d\n%s\n", book->book, book->author, book->rating, book->genre, book->price, book->publisher, book->year, book->descr);
}
void iffinderstr(mytree* book, char* action)//поиск для строковых ключей
{
	mytree* right = find(book->right, STR_KEY, action);//ищем в правом поддереве
	if (right) //если найдено
	{
		info(right->book);//выводим информацию
		iffinderstr(right, action);//запускаем для правого
	}
	mytree* left = find(book->left, STR_KEY, action);//ищем в левом поддереве
	if (left)
	{
		info(left->book);
		iffinderstr(left, action);
	}
}
void finderstr(mytree* book, char* action, int* check)//функция для поиска всех значений по числовому ключу
{
	book = find(book, STR_KEY, action);//ищем первое вхождение и меняем начало дерева на это вхождение
	if (book)//если найдено
	{
		(*check)++;
		info(book->book);//выводим всю информацию о книге
		iffinderstr(book, action);
	}
}
void iffinder(mytree* book, int action)//поиск для числовых ключей
{
	mytree* right = find(book->right, action, NULL);//ищем в правом поддереве
	if (right) //если найдено
	{
		info(right->book);//выводим информацию
		iffinder(right,action);//запускаем для правого
	}
	mytree* left = find(book->left, action, NULL);//ищем в левом поддереве
	if (left)
	{
		info(left->book);
		iffinder(left, action);
	}
}
void findernumber(mytree* book, int action,int*check)//функция для поиска всех вхождений по строковому ключу
{
	book = find(book, action, NULL);//ищем первое вхождение и меняем начало дерева на это вхождение
	if (book)//если найдено
	{
		(*check)++;
		info(book->book);//выводим всю информацию о книге
		iffinder(book, action);
	}
}
void forsort(mytree* tree)//вывод информации в отсортированном порядке
{
	if (tree->left) forsort(tree->left);//идем на максимум влево
	info(tree->book);//когда дошли, то выводим информацию
	if (tree->right) forsort(tree->right);//делаем шаг вправо, если можно
}
void forsave(FILE* file,mytree* bd)//сохранение в файл происходит с сортировкой по названию книги
{
	if (bd->left) forsave(file,bd->left);//идем на максимум влево
	fprintf(file, "%s\n%s\n%d\n%s\n%d\n%s\n%d\n%s\n", bd->book->book, bd->book->author, bd->book->rating,
		bd->book->genre, bd->book->price, bd->book->publisher, bd->book->year, bd->book->descr);//выводим в файл информацию
	if (bd->right) forsave(file,bd->right);//делаем шаг вправо, если можно
}
void forbd(FILE* bd)
{
	char letter=getc(bd);
	while (letter!=EOF)//идем до конца файла
	{
		data* first = (data*)malloc(sizeof(data));
		if (first == NULL) exit(MALLOC_ERROR);
		mass[number_of_book] = first;//сохраняем указатель, чтобы потом очистить всю выделенную память
		number_of_book++;//увеличиваем количество книг
		int i = 0;
		while (letter != '\n' && letter != EOF)//идем по строкам
		{//считываем название книги
			first->book[i++] = letter;
			letter = getc(bd);
		}
		first->book[i] = STRING_END;
		i = 0; letter = getc(bd);
		while (letter != '\n' && letter != EOF)
		{//считываем автора книги
			first->author[i++] = letter;
			letter = getc(bd);
		}
		first->author[i] = STRING_END;
		fscanf(bd, "%d", &i);//считываем оценку читателей
		first->rating = i;
		i = 0;letter = getc(bd);letter = getc(bd);
		while (letter != '\n' && letter != EOF)
		{//считываем жанр книги
			first->genre[i++] = letter;
			letter = getc(bd);
		}
		first->genre[i] = STRING_END;
		fscanf(bd, "%d", &i);//считываем цену книги
		first->price = i;
		i = 0;letter = getc(bd);letter = getc(bd);
		while (letter != '\n' && letter != EOF)
		{//считываем издательство книги
			first->publisher[i++] = letter;
			letter = getc(bd);
		}
		first->publisher[i] = STRING_END;
		fscanf(bd, "%d", &i);//считываем год издания книги
		first->year = i;
		i = 0;letter = getc(bd);letter = getc(bd);
		while (letter != '\n' && letter != EOF)
		{//считываем описание книги
			first->descr[i++] = letter;
			letter = getc(bd);
		}
		first->descr[i] = STRING_END;
		letter = getc(bd);
		root_year = insert(root_year, first->year, NULL, first);//вставляем в дерево поиска по году
		root_rating = insert(root_rating, first->rating, NULL, first);//вставляем в дерево поиска по рейтингу
		root_price = insert(root_price, first->price, NULL, first);//вставляем в дерево поиска по цене
		root_author = insert(root_author, STR_KEY, first->author, first);//вставляем в дерево поиска по автору
		root_genre = insert(root_genre, STR_KEY, first->genre, first);//вставляем в дерево поиска по жанру
		root_publisher = insert(root_publisher, STR_KEY, first->publisher, first);//вставляем в дерево поиска по издательству
		root_book = insert(root_book, STR_KEY, first->book, first);//вставляем в дерево поиска по названию
	}
}