#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include <stdio.h>
#include <windows.h>

typedef struct data//���������� � �����
{
	int year, price, rating;//���/����/������ ���������
	char book[NAME_LENGTH], author[NAME_LENGTH], publisher[NAME_LENGTH], genre[NAME_LENGTH], descr[DECR_LENGTH];//��������/�����/������������/����/��������
} data;

data* mass[MAX_BOOKS] = { NULL };
int number_of_book = 0;

typedef struct mytreename 
{
	struct mytreename* left, * right;//��������� �� �����/������ ��������� � �� ��������
	unsigned char height;//������ ������
	int key;//���� ��� �����
	char* strkey;//���� ��� �����
	data* book;
} mytree;

mytree* root_book = NULL;//������ ��� ������ �� �������� �����
mytree* root_author = NULL;//������ ��� ������ �� ������ �����
mytree* root_publisher = NULL;//������ ��� ������ �� ������������ �����
mytree* root_genre = NULL;//������ ��� ������ �� ����� �����
mytree* root_rating = NULL;//������ ��� ������ �� �������� �����
mytree* root_price = NULL;//������ ��� ������ �� ���� �����
mytree* root_year = NULL;//������ ��� ������ �� ���� ������� �����

unsigned char height(mytree* p)//������� ������ ������ ������, ������� ����������� � ����
{
	return p ? p->height : 0;//���� ������ �� 0, �� ������������ ������, ����� 0
}

char bfactor(mytree* p)//��������� ������� ����� �������� ������� � ������ ���������
{
	return (height(p->right) - height(p->left));
}

void fixheight(mytree* p)//��������������� ���������� �������� ���� height
{
	unsigned char hl = height(p->left);//������ ������ ������ ���������
	unsigned char hr = height(p->right);//������ ������ ������� ���������
	p->height = (hl > hr ? hl : hr) + 1;//������������� ������ ������ � ���������� � ���������� 1 (������/����� ��������� ����� ����)
}

//������ �������� ������������ �����, ����� ������ p-left-��������� = 2 � ������ p-right-right-��������� <= ������ p-right-left-���������.
mytree* rotateright(mytree* p) // ����� ������ ������� ������ p
{
	mytree* q = p->left;//����� ����� ���������
	p->left = q->right;//�� ����� ������ ��������� ������ p-left-right
	q->right = p;//�� ����� ������� ���� ������� ��������� ������ ������
	fixheight(p);//������ ���������, ������� ������ ��� ������
	fixheight(q);//����� ������, ����� ������������� ��� ������
	return q;//���������� ��������� �� ������� ����������� ������
}

//������ �������� ������������ �����, ����� ������ p-right-��������� = 2 � ������ p-right-left <= ������ p-right-right
mytree* rotateleft(mytree* q) // ����� ������� ������ q
{
	mytree* p = q->right;//����� ������ ���������
	q->right = p->left;//�� ����� ������� ��������� ������ p-right-left
	p->left = q;//������ �� ����� ������ ���� ������� ��������� ������ ������
	fixheight(q);//������������� ���������� �������� �����
	fixheight(p);
	return p;//���������� ��������� �� ����� ������ ������
}

mytree* balance(mytree* p) //������������ ���� p
{
	fixheight(p);//������������� ���������� �������� ������, ������ �������� �����, ��� ��� ������� ���������� ����� ��������, ��� ��� �������� ����� ���� �� ����������
	if (bfactor(p) == 2)//���� ����� ��������� ����� �������
	{
		if (bfactor(p->right) < 0)//���� ����� ��������� ������� ����� ��������
			p->right = rotateright(p->right);//������ ������� ������ ����� ������ ��������
		return rotateleft(p);//�������� ����� ����� ��������, ��� ��� ������ ������� ��� ���������� ������ ������ �������� �����������
	}
	if (bfactor(p) == -2)//���� ����� ��������� ������ ��������
	{
		if (bfactor(p->left) > 0)//���� ����� ��������� ������ ������� ��������
			p->left = rotateleft(p->left);//����� ��� ������ ����� ����� ��������
		return rotateright(p);//�������� ������ ����� ��������, ��� ��� ������ ������� ��� ���������� ������ ������� �������� �����������
	}
	return p; //������������ �� �����
}

mytree* insert(mytree* p, int k, char* kk, data* book) // ������� ����� k ��� kk � ����� book � ������ � ������ p
{														//kk - ������, k - �����
	if (!p)//���� ������ ��� ������ � ��������� �� ����� ����� ��� ��� ������ ������� ������
	{
		mytree* new = (mytree*)malloc(sizeof(mytree));//������� ��������� ��� ������
		if (new == NULL) exit(MALLOC_ERROR);
		new->height = 1;//������������� ������ �����
		if (kk == NULL)//��������� ����� ����
			new->key = k;//������������� �������� ��������� �����
		else new->strkey = kk;
		new->book = book;//��������� � ������ ��������� �� �����
		new->right = NULL;
		new->left = NULL;
		return new;//���������� ��������� �� ��������� ���������
	}
	if (kk == NULL) if (k < p->key)//���� ���� ������ ����� ������
		p->left = insert(p->left, k, NULL, book);//����� ���� �����
	else//���� ���� ������ ���� �����
		p->right = insert(p->right, k, NULL, book);//���� ������
	else if (strcmp(kk, p->strkey) < 0)//���� ���� � ���� ������
		p->left = insert(p->left, k, kk, book);//���� �����
	else p->right = insert(p->right, k, kk, book);//���� ������
	return balance(p);//����������� ������ ������� ����� �������
}

mytree* findmin(mytree* p) // ����� ���� � ����������� ������ � ������ p 
{
	return p->left ? findmin(p->left) : p;//���� �� ��������, �� �������� ���-������ �� �����
}

mytree* removemin(mytree* p) // �������� ���� � ����������� ������ �� ������ p
{
	if (p->left == 0)//���� ������ ��������� ���, �� ������� ������ � ���������� ������, ����� ��������� �� ��� �����
		return p->right;
	p->left = removemin(p->left);//� ��������� ������ ���������� ���� �� �������� � ������
	return balance(p);//����� �������� �������� ���������� ������������
}

mytree* removee(mytree* p, int k, char* kk, data* book)// �������� ����� k ��� kk �� ������ p ����� book
{
	if (!p) return NOT_FOUND;//���� �� ������
	if (kk==NULL)
	if (k < p->key)//���� ������ ��� � �����
		p->left = removee(p->left, k,kk,book);//���� ������
		else if (k > p->key)//���� ������, �� �������
			p->right = removee(p->right, k,kk,book);
	else //  k == p->key 
	{
		if (p->book == book)//���� ��������� ������� �������� ��������� �����, ������� �������
		{
			mytree* q = p->left;//���������� ����� ���������
			mytree* r = p->right;//���������� ������ ���������
			free(p);//������� ��������� �������
			if (!r) return q;//���� ������� ��������� ���, �� ���������� ��������� �� ����� ���������
			mytree* min = findmin(r);//���� ������ ��������� ����, �� ������� ������� � ������ ���������
			min->right = removemin(r);//������ ��������� ������� �� ����� ����������
			min->left = q;
			return balance(min);//���������� ������������
		}
		else
		{
			p->right = removee(p->right, k, kk, book);//���� ���������� ��������� ���� � ����� �����, ���� ������
			p->left = removee(p->left, k, kk, book);//���� �����
		}
	}
	else//�������� �� ������
	{
		if (strcmp(kk, p->strkey) < 0)//���� ������ ��� � �����
			p->left = removee(p->left, k, kk,book);//���� ������
		else if (strcmp(kk, p->strkey) > 0)//���� ������, �� �������
			p->right = removee(p->right, k, kk,book);
		else //  k == p->strkey 
		{
			if (p->book == book)
			{
				mytree* q = p->left;//���������� ����� ���������
				mytree* r = p->right;//���������� ������ ���������
				free(p);//������� ��������� �������
				if (!r) return q;//���� ������� ��������� ���, �� ���������� ��������� �� ����� ���������
				mytree* min = findmin(r);//���� ������ ��������� ����, �� ������� ������� � ������ ���������
				min->right = removemin(r);//������ ��������� ������� �� ����� ����������
				min->left = q;
				return balance(min);//���������� ������������
			}
			else
			{
				p->right = removee(p->right, k, kk, book);//���� ������ 
				p->left = removee(p->left, k, kk, book);//���� ������
			}
		}
	}
	return balance(p);//���������� ������������ ������ �������
}

mytree* find(mytree* p, int k, char* kk)//����� �� ������ p �� ������ �� ������ k ��� kk, ���� ������ ��������� � ������
{
	if (!p) return NOT_FOUND;//���� �� ������
	if (kk == NULL)//���� ������ ���� �� ������
		if (k < p->key)//���� ������ ��� � �����
			find(p->left, k, kk);//���� ������
		else if (k > p->key)//���� ������, �� �������
			find(p->right, k, kk);
		else return p;//���� �����
	else if (strcmp(kk, p->strkey) < 0) find(p->left, k, kk);//���� ��������� ����
		else if (strcmp(kk, p->strkey) > 0) find(p->right, k, kk);
			else return p;//���������� ��������� �� ����� 
}
void add();//������� ���������� �����
void change();//�������������� �����
void delete();//������� �������� �����
void free_roots(mytree* root);//������������ ������ ��������
void free_books();//������������ ������ �������� ����
void finder();//����� �� ���� ������
void info(data* book);
void finderstr(mytree* book, char* action, int* check);
void findernumber(mytree* book, int action, int* check);
void sort();
void forsort(mytree* tree);//�������, ������� ������� ����� ������ � ������� ��� ���������� � ��������������� �������
void save();//������� ��� ���������� �� � ����
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
			free_roots(root_year);//������������ ������ �� �������� ��� ������
			free_roots(root_price);
			free_roots(root_rating);
			free_roots(root_genre);
			free_roots(root_author);
			free_roots(root_book);
			free_roots(root_publisher);
			free_books();//������������ ������ �� ����
			return;
		}
		}
	}
}

void add()//���������� ����� � ������ � ������ p
{
	data* first = (data*)malloc(sizeof(data));
	if (first == NULL) exit(MALLOC_ERROR);
	mass[number_of_book] = first;//��������� ���������, ����� ����� �������� ��� ���������� ������
	number_of_book++;//����������� ���������� ����
	getchar();
	while (1)
	{
		printf("������� ��� �����: ");
		char book_name[NAME_LENGTH] = { '0' };
		fgets(book_name, NAME_LENGTH, stdin);
		book_name[strlen(book_name) - 1] = '\0';
		if (!strlen(book_name))
		{
			printf("������������ ��� (���� �� ����� ���� ������). ���������� ��� ���.\n");
			continue;
		}
		strcpy(first->book, book_name);
		break;
	}
	while (1)
	{
		printf("������� ������: ");
		char book_author[NAME_LENGTH] = { '0' };
		fgets(book_author, NAME_LENGTH, stdin);				//������������ ������ ������ � �����
		book_author[strlen(book_author) - 1] = '\0';
		if (!strlen(book_author))
		{
			printf("������������ ��� (���� �� ����� ���� ������). ���������� ��� ���.\n");
			continue;
		}
		strcpy(first->author, book_author);
		break;
	}
	while (1)
	{
		printf("������� ���� �����: ");
		char book_genre[NAME_LENGTH] = { '0' };
		fgets(book_genre, NAME_LENGTH, stdin);
		book_genre[strlen(book_genre) - 1] = '\0';
		if (!strlen(book_genre))
		{
			printf("������������ ��� (���� �� ����� ���� ������). ���������� ��� ���.\n");
			continue;
		}
		strcpy(first->genre, book_genre);
		break;
	}
	while (1)
	{
		printf("������� ��� �������: ");
		int year = 0;
		scanf("%d", &year);
		if (year < 0)
		{
			printf("����������� ������ ��� (������������� ���). ���������� ��� ���.\n");
			continue;
		}
		first->year = year;
		break;
	}
	while (1)
	{
		printf("������� ������ ��������� (�� ����� �� 0 �� 10): ");
		int rating = 0;
		scanf("%d", &rating);
		if (rating < 0 || rating>10)
		{
			printf("����������� ������ �������. ���������� ��� ���.\n");
			continue;
		}
		first->rating = rating;
		break;
	}
	while (1)
	{
		printf("������� ���� �����: ");
		int price = 0;
		scanf("%d", &price);
		if (price < 0)
		{
			printf("����������� ������� ���� (������������� ����). ���������� ��� ���.\n");
			continue;
		}
		first->price = price;
		break;
	}
	getchar();
	while (1)
	{
		printf("������� ������������: ");
		char book_publ[NAME_LENGTH] = { '0' };
		fgets(book_publ, NAME_LENGTH, stdin);
		book_publ[strlen(book_publ) - 1] = '\0';
		if (!strlen(book_publ))
		{
			printf("������������ ��� (���� �� ����� ���� ������). ���������� ��� ���.\n");
			continue;
		}
		strcpy(first->publisher, book_publ);
		break;
	}
	while (1)
	{
		printf("������� �������� �����: ");
		char book_descr[DECR_LENGTH] = { '0' };
		fgets(book_descr, DECR_LENGTH, stdin);
		book_descr[strlen(book_descr) - 1] = '\0';
		if (!strlen(book_descr))
		{
			printf("������������ ��� (���� �� ����� ���� ������). ���������� ��� ���.\n");
			continue;
		}
		strcpy(first->descr, book_descr);
		break;
	}
	root_year=insert(root_year, first->year,NULL, first);//��������� � ������ ������ �� ����
	root_rating =insert(root_rating, first->rating,NULL,first);//��������� � ������ ������ �� ��������
	root_price=insert(root_price, first->price,NULL,first);//��������� � ������ ������ �� ����
	root_author = insert(root_author, STR_KEY, first->author, first);//��������� � ������ ������ �� ������
	root_genre = insert(root_genre, STR_KEY, first->genre, first);//��������� � ������ ������ �� �����
	root_publisher = insert(root_publisher, STR_KEY, first->publisher, first);//��������� � ������ ������ �� ������������
	root_book = insert(root_book, STR_KEY, first->book, first);//��������� � ������ ������ �� ��������
}
void delete()
{
	printf("�������� ����� ������������ �� ��������.\n������� �������� �����, ������� ������ �������: ");
	char name[NAME_LENGTH] = { 0 };
	getchar();
	fgets(name, NAME_LENGTH, stdin);
	name[strlen(name) - 1] = '\0';
	mytree* book=find(root_book, STR_KEY, name);
	if (!book)
	{
		printf("����� �� �������\n");
		return;
	}
	root_year=removee(root_year, book->book->year,NULL,book->book);
	root_price = removee(root_price, book->book->price, NULL,book->book);
	root_rating = removee(root_rating, book->book->rating, NULL,book->book);
	root_author = removee(root_author, STR_KEY, book->book->author,book->book);
	root_genre = removee(root_genre, STR_KEY, book->book->genre,book->book);
	root_publisher = removee(root_publisher, STR_KEY, book->book->publisher,book->book);
	root_book = removee(root_book, STR_KEY, book->book->book,book->book);
	printf("�������� ���������\n");
}
void change()
{
	printf("����� �����, ������� ��� ����� ���������������, ������������ �� ��������.\n������� �������� �����, ������� ������ ���������������: ");
	char name[NAME_LENGTH] = { 0 };
	getchar();
	fgets(name, NAME_LENGTH, stdin);
	name[strlen(name) - 1] = '\0';
	mytree* book = find(root_book, STR_KEY, name);
	if (!book)
	{
		printf("����� �� �������\n");
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
			printf("������� ���: %d\n������� ���, ������� ������ ���������� ������ �����: ",book->book->year);
			scanf("%d", &year);
			root_year = removee(root_year, book->book->year, NULL,book);//������� �� ������ �������
			book->book->year = year;
			root_year = insert(root_year, book->book->year, NULL, book);//��������� � ������ �������
			break;
		}
		case RATING:
		{
			int rating = 0;
			printf("������� ������ ���������: %d\n������� ������, ������� ������ ���������� ������ �����: ", book->book->rating);
			scanf("%d", &rating);
			root_rating = removee(root_rating, book->book->rating, NULL,book);//������� �� ������ �������
			book->book->rating = rating;
			root_rating = insert(root_rating, book->book->rating, NULL, book);//��������� � ������ �������
			break;
		}
		case PRICE:
		{
			int price = 0;
			printf("������� ����: %d\n������� ����, ������� ������ ���������� ������ �����: ", book->book->price);
			scanf("%d", &price);
			root_price = removee(root_price, book->book->price, NULL,book);//������� �� ������ �������
			book->book->price = price;
			root_price = insert(root_price, book->book->price, NULL, book);//��������� � ������ �������
			break;
		}
		case PUBLISHER:
		{
			char publisher[NAME_LENGTH] = { 0 };
			printf("������� ������������: %s\n������� ������������, ������� ������ ���������� ������ �����: ", book->book->publisher);
			getchar();
			fgets(publisher, NAME_LENGTH, stdin);
			publisher[strlen(publisher) - 1] = '\0';
			root_publisher = removee(root_publisher, STR_KEY, book->book->publisher,book);//������� �� ������ �������
			strcpy(book->book->publisher,publisher);
			root_publisher = insert(root_publisher, STR_KEY, book->book->publisher, book);//��������� � ������ �������
			break;
		}
		case AUTHOR:
		{
			char author[NAME_LENGTH] = { 0 };
			printf("������� �����: %s\n������� ������, �������� ������ ���������� ������ �����: ", book->book->author);
			getchar();
			fgets(author, NAME_LENGTH, stdin);
			author[strlen(author) - 1] = '\0';
			root_author = removee(root_author, STR_KEY, book->book->author,book);//������� �� ������ �������
			strcpy(book->book->author, author);
			root_author = insert(root_author, STR_KEY, book->book->author, book);//��������� � ������ �������
			break;
		}
		case BOOK:
		{
			char book_name[NAME_LENGTH] = { 0 };
			printf("������� �������� �����: %s\n������� ��������, �������� ������ ���������� ������ �����: ", book->book->book);
			getchar();
			fgets(book_name, NAME_LENGTH, stdin);
			book_name[strlen(book_name) - 1] = '\0';
			root_book = removee(root_book, STR_KEY, book->book->book,book);//������� �� ������ �������
			strcpy(book->book->book, book_name);
			root_book = insert(root_book, STR_KEY, book->book->book, book);//��������� � ������ �������
			break;
		}
		case GENRE:
		{
			char genre[NAME_LENGTH] = { 0 };
			printf("������� ����: %s\n������� ����, ������� ������ ���������� ������ �����: ", book->book->genre);
			getchar();
			fgets(genre, NAME_LENGTH, stdin);
			genre[strlen(genre) - 1] = '\0';
			root_genre = removee(root_genre, STR_KEY, book->book->genre,book);//������� �� ������ �������
			strcpy(book->book->genre, genre);
			root_genre = insert(root_genre, STR_KEY, book->book->genre, book);//��������� � ������ �������
			break;
		}
		case DESCR:
		{
			char descr[DECR_LENGTH] = { 0 };
			printf("������� ��������: %s\n������� ��������, ������� ������ ���������� ������ �����: ", book->book->descr);
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
		printf("������� ���, �� �������� ����� ������������� �����: ");
		scanf("%d", &year);
		mytree* book=root_year;//������ ��������� �� ������ ������
		findernumber(book, year, &check);
		if (!check) printf("������ �� �������\n");
		break;
	}
	case RATING:
	{
		int rating = 0, check=0;
		printf("������� �������, �� �������� ����� ������������� ����� (�� 0 �� 10): ");
		scanf("%d", &rating);
		mytree* book = root_rating;//������ ��������� �� ������ ������
		findernumber(book, rating, &check);
		if (!check) printf("������ �� �������\n");
		break;
	}
	case PRICE:
	{
		int price = 0, check=0;
		printf("������� ���� �����, �� ������� ����� ������������� �����: ");
		scanf("%d", &price);
		mytree* book = root_price;//������ ��������� �� ������ ������
		findernumber(book, price, &check);//���� ������ ��������� � ������� ���������� � ��������� �����, ���� ��� �������
		if (!check) printf("������ �� �������\n");
		break;
	}
	case PUBLISHER:
	{
		int check = 0;
		char publisher[NAME_LENGTH] = { 0 };
		printf("������� ������������, �� �������� ����� ������������� �����: ");
		getchar();
		fgets(publisher, NAME_LENGTH, stdin);
		publisher[strlen(publisher) - 1] = '\0';
		mytree* book = root_publisher;//������ ��������� �� ������ ������
		finderstr(book, publisher, &check);
		if (!check) printf("������ �� �������\n");
		break;
	}
	case AUTHOR:
	{
		int check = 0;
		char author[NAME_LENGTH] = { 0 };
		printf("������� ������, �� �������� ����� ������������� �����: ");
		getchar();
		fgets(author, NAME_LENGTH, stdin);
		author[strlen(author) - 1] = '\0';
		mytree* book = root_author;//������ ��������� �� ������ ������
		finderstr(book, author, &check);
		if (!check) printf("������ �� �������\n");
		break;
	}
	case BOOK:
	{
		int check = 0;
		char book_name[NAME_LENGTH] = { 0 };
		printf("������� �������� �����, �� �������� ����� ������������� �����: ");
		getchar();
		fgets(book_name, NAME_LENGTH, stdin);
		book_name[strlen(book_name) - 1] = '\0';
		mytree* book = root_book;//������ ��������� �� ������ ������
		finderstr(book, book_name, &check);
		if (!check) printf("������ �� �������\n");
		break;
	}
	case GENRE:
	{
		int check = 0;
		char genre[NAME_LENGTH] = { 0 };
		printf("������� ����, �� �������� ����� ������������� �����: ");
		getchar();
		fgets(genre, NAME_LENGTH, stdin);
		genre[strlen(genre) - 1] = '\0';
		mytree* book = root_genre;//������ ��������� �� ������ ������
		finderstr(book, genre, &check);
		if (!check) printf("������ �� �������\n");
		break;
	}
	default: return;
	}
}
void sort()
{
	sort_menu();//������������ �������� �� ������ ���� ����� ����������
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
	printf("����� ������ ����, ���� ���������� ���� ������. ����� ��� �����.\n"
		"���� ������ ��������� ���� ������ � ��� ������������ ����, �� ������� ��� ���.\n");
	char file_name[NAME_LENGTH] = { 0 };
	getchar();
	fgets(file_name, NAME_LENGTH, stdin);
	file_name[strlen(file_name) - 1] = '\0';
	FILE* file_for_save = fopen(file_name, "w");//������� ����
	if (file_for_save == NULL)
	{
		printf("�� ������� ������� ����.\n");
		return;
	}
	printf("���� ������� ������.\n");
	forsave(file_for_save,root_book);
	printf("���� ������ ��������� � ����.\n");
}
void bd_from_file()
{
	printf("������� ������� �������� ���� ������ � �����:\n"
	"1) ������ ���� ����� ��������� �� ��������� ������\n2) ����� ������� �� �������� �������������� ��������� �����\n"
	"3) ���� �������� � ��������� �������: \n\ta) �������� �����\n\t�) �����\n\t�) ������ ������������� (�� 0 �� 10)\n\t"
	"�) ����\n\t�) ����\n\t�) ������������\n\t�) ��� �������\n\t�) ��������\n������� ��� ����� � ����� ������: ");
	char file_name[NAME_LENGTH] = { 0 };
	getchar();
	fgets(file_name, NAME_LENGTH, stdin);
	file_name[strlen(file_name) - 1] = '\0';
	FILE* bd = fopen(file_name, "r");//��������� ����
	if (bd == NULL)
	{
		printf("�� ������� ������� ����.\n");
		return;
	}
	forbd(bd);
	printf("���� ������ ������� ����� �� ����.\n");
}

void free_roots(mytree* root)
{
	if (!root) return;//���� ������ ������ ����, �� ����������� ��������
	if (root->left)//���� ����� ���� ��� �����, �� ���� �� ��������
		free_roots(root->left);
	if (root->right)//���� ��� ������ ���� �����, �� ����� ���� ������, �� ����
		free_roots(root->right);
	//��� ������, �� � �����, �� ����� ���� �� ������, ��������� ������, ����� �� ������� ���, ��� ����
	free(root);//������� ��������� ��� ������
}
void free_books()
{
	for (int i = 0;i < number_of_book;i++) free(mass[i]);
}
void info(data* book)
{
	printf("%s, %s, %d, %s, %d, %s, %d\n%s\n", book->book, book->author, book->rating, book->genre, book->price, book->publisher, book->year, book->descr);
}
void iffinderstr(mytree* book, char* action)//����� ��� ��������� ������
{
	mytree* right = find(book->right, STR_KEY, action);//���� � ������ ���������
	if (right) //���� �������
	{
		info(right->book);//������� ����������
		iffinderstr(right, action);//��������� ��� �������
	}
	mytree* left = find(book->left, STR_KEY, action);//���� � ����� ���������
	if (left)
	{
		info(left->book);
		iffinderstr(left, action);
	}
}
void finderstr(mytree* book, char* action, int* check)//������� ��� ������ ���� �������� �� ��������� �����
{
	book = find(book, STR_KEY, action);//���� ������ ��������� � ������ ������ ������ �� ��� ���������
	if (book)//���� �������
	{
		(*check)++;
		info(book->book);//������� ��� ���������� � �����
		iffinderstr(book, action);
	}
}
void iffinder(mytree* book, int action)//����� ��� �������� ������
{
	mytree* right = find(book->right, action, NULL);//���� � ������ ���������
	if (right) //���� �������
	{
		info(right->book);//������� ����������
		iffinder(right,action);//��������� ��� �������
	}
	mytree* left = find(book->left, action, NULL);//���� � ����� ���������
	if (left)
	{
		info(left->book);
		iffinder(left, action);
	}
}
void findernumber(mytree* book, int action,int*check)//������� ��� ������ ���� ��������� �� ���������� �����
{
	book = find(book, action, NULL);//���� ������ ��������� � ������ ������ ������ �� ��� ���������
	if (book)//���� �������
	{
		(*check)++;
		info(book->book);//������� ��� ���������� � �����
		iffinder(book, action);
	}
}
void forsort(mytree* tree)//����� ���������� � ��������������� �������
{
	if (tree->left) forsort(tree->left);//���� �� �������� �����
	info(tree->book);//����� �����, �� ������� ����������
	if (tree->right) forsort(tree->right);//������ ��� ������, ���� �����
}
void forsave(FILE* file,mytree* bd)//���������� � ���� ���������� � ����������� �� �������� �����
{
	if (bd->left) forsave(file,bd->left);//���� �� �������� �����
	fprintf(file, "%s\n%s\n%d\n%s\n%d\n%s\n%d\n%s\n", bd->book->book, bd->book->author, bd->book->rating,
		bd->book->genre, bd->book->price, bd->book->publisher, bd->book->year, bd->book->descr);//������� � ���� ����������
	if (bd->right) forsave(file,bd->right);//������ ��� ������, ���� �����
}
void forbd(FILE* bd)
{
	char letter=getc(bd);
	while (letter!=EOF)//���� �� ����� �����
	{
		data* first = (data*)malloc(sizeof(data));
		if (first == NULL) exit(MALLOC_ERROR);
		mass[number_of_book] = first;//��������� ���������, ����� ����� �������� ��� ���������� ������
		number_of_book++;//����������� ���������� ����
		int i = 0;
		while (letter != '\n' && letter != EOF)//���� �� �������
		{//��������� �������� �����
			first->book[i++] = letter;
			letter = getc(bd);
		}
		first->book[i] = STRING_END;
		i = 0; letter = getc(bd);
		while (letter != '\n' && letter != EOF)
		{//��������� ������ �����
			first->author[i++] = letter;
			letter = getc(bd);
		}
		first->author[i] = STRING_END;
		fscanf(bd, "%d", &i);//��������� ������ ���������
		first->rating = i;
		i = 0;letter = getc(bd);letter = getc(bd);
		while (letter != '\n' && letter != EOF)
		{//��������� ���� �����
			first->genre[i++] = letter;
			letter = getc(bd);
		}
		first->genre[i] = STRING_END;
		fscanf(bd, "%d", &i);//��������� ���� �����
		first->price = i;
		i = 0;letter = getc(bd);letter = getc(bd);
		while (letter != '\n' && letter != EOF)
		{//��������� ������������ �����
			first->publisher[i++] = letter;
			letter = getc(bd);
		}
		first->publisher[i] = STRING_END;
		fscanf(bd, "%d", &i);//��������� ��� ������� �����
		first->year = i;
		i = 0;letter = getc(bd);letter = getc(bd);
		while (letter != '\n' && letter != EOF)
		{//��������� �������� �����
			first->descr[i++] = letter;
			letter = getc(bd);
		}
		first->descr[i] = STRING_END;
		letter = getc(bd);
		root_year = insert(root_year, first->year, NULL, first);//��������� � ������ ������ �� ����
		root_rating = insert(root_rating, first->rating, NULL, first);//��������� � ������ ������ �� ��������
		root_price = insert(root_price, first->price, NULL, first);//��������� � ������ ������ �� ����
		root_author = insert(root_author, STR_KEY, first->author, first);//��������� � ������ ������ �� ������
		root_genre = insert(root_genre, STR_KEY, first->genre, first);//��������� � ������ ������ �� �����
		root_publisher = insert(root_publisher, STR_KEY, first->publisher, first);//��������� � ������ ������ �� ������������
		root_book = insert(root_book, STR_KEY, first->book, first);//��������� � ������ ������ �� ��������
	}
}