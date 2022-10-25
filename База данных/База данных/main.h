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
	printf("Выберите нужное действие:\n"
		"1) Добавить книгу\n"
		"2) Удалить книгу\n"
		"3) Редактирование книги\n"
		"4) Поиск книги\n"
		"5) Отсортировать книги\n"
		"6) Сохранение базы данных в файл\n"
		"7) Загрузить базу данных из файла\n"
		"8) Выйти из программы\n");
}
void change_menu()
{
	printf("Выберите поле, которое хотите отредактировать:\n"
	"1) Год выпуска\n"
	"2) Оценка читателей\n"
	"3) Цена\n"
	"4) Издательство\n"
	"5) Автор\n"
	"6) Название книги\n"
	"7) Жанр\n"
	"8) Краткое описание\n"
	"9) Закончить редактирование книги\n");
}
void finder_menu()
{
	printf("Выберите поле, по которому хотите искать книгу:\n"
		"1) Год выпуска\n"
		"2) Оценка читателей\n"
		"3) Цена\n"
		"4) Издательство\n"
		"5) Автор\n"
		"6) Название книги\n"
		"7) Жанр\n");
}
void sort_menu()
{
	printf("Выберите поле, по которому хотите отсортировать книги:\n"
		"1) Год выпуска\n"
		"2) Оценка читателей\n"
		"3) Цена\n"
		"4) Издательство\n"
		"5) Автор\n"
		"6) Название книги\n"
		"7) Жанр\n");
}

