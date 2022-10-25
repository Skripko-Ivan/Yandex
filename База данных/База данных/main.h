#define NAME_LENGTH 0x7f
#define DECR_LENGTH 0x1ff
#define MALLOC_ERROR 0xff
#define NOT_FOUND 0x00
#define ADD_BOOK 0x01
#define DELETE_BOOK 0x02
#define CHANGE_BOOK 0x03
#define FIND 0x04
#define SORT 0x05
#define SAVE 0x06
#define BD_FROM_FILE 0x07
#define MAX_BOOKS 0xfff
#define STR_KEY 0x0
#define YEAR 0x01
#define RATING 0x02
#define PRICE 0x03
#define PUBLISHER 0x04
#define AUTHOR 0x05
#define BOOK 0x06
#define GENRE 0x07
#define DESCR 0x08
#define STRING_END 0x00

void menu()
{
	printf("�������� ������ ��������:\n"
		"1) �������� �����\n"
		"2) ������� �����\n"
		"3) �������������� �����\n"
		"4) ����� �����\n"
		"5) ������������� �����\n"
		"6) ���������� ���� ������ � ����\n"
		"7) ��������� ���� ������ �� �����\n"
		"8) ����� �� ���������\n");
}
void change_menu()
{
	printf("�������� ����, ������� ������ ���������������:\n"
	"1) ��� �������\n"
	"2) ������ ���������\n"
	"3) ����\n"
	"4) ������������\n"
	"5) �����\n"
	"6) �������� �����\n"
	"7) ����\n"
	"8) ������� ��������\n"
	"9) ��������� �������������� �����\n");
}
void finder_menu()
{
	printf("�������� ����, �� �������� ������ ������ �����:\n"
		"1) ��� �������\n"
		"2) ������ ���������\n"
		"3) ����\n"
		"4) ������������\n"
		"5) �����\n"
		"6) �������� �����\n"
		"7) ����\n");
}
void sort_menu()
{
	printf("�������� ����, �� �������� ������ ������������� �����:\n"
		"1) ��� �������\n"
		"2) ������ ���������\n"
		"3) ����\n"
		"4) ������������\n"
		"5) �����\n"
		"6) �������� �����\n"
		"7) ����\n");
}

