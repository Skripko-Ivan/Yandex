.org 0x7c00 #указание адреса загрузки кода
.code16 #переход в 16-битный режим

_start:
#инициализация адресов сегментов
#синтаксис: инструкция приемник, источник
#перед регистрами ставится знак "%"
#перед константами ставится знак "$"
#каждая команда пишется с суффиксом, указывающим размер операндов команды, movw - суффикс w - размер 2 байта
	movw %cs, %ax #сохранение адреса сегмента кода в ax
	movw %ax, %ds #сохранение этого адреса как начало сегмента данных
	movw %ax, %ss #и как начало сегмента стека
	movw $_start, %sp #сохранение адреса стека в качестве адреса первой инструкции кода, после чего стек будет расти вверх

	movb $0x01, %ch

#нужная строка помещается в bx (строка зависит от выбранного цвета)
#далее вызывается функция puts для вывова цвета на экран
#так выводятся по очереди все цвета, у выбранного цвета дополнительно выводится стрелочка
#после чего происходит прыжок в choose для выбора цвета
	movb $0x01, %ch #счетчик для выбора цвета
menu:
	movw $0x0003, %ax #очистить экран
	int $0x10

	#вывести меню
	cmp $0x01, %ch #если выбран этот цвет
	jne g #если выбран другой цвет
	call this #если этот, то вывести стрелочку перед цветом
g:
	movw $gray, %bx
	call puts
	call next_str
	cmp $0x02, %ch
	jne wh
	call this
wh:
	movw $white, %bx
	call puts
	call next_str	
	cmp $0x03, %ch
	jne y
	call this
y:
	movw $yellow, %bx
	call puts
	call next_str
	cmp $0x04, %ch
	jne c
	call this
c:
	movw $cian, %bx
	call puts
	call next_str
	cmp $0x05, %ch
	jne m
	call this
m:
	movw $magenta, %bx
	call puts
	call next_str
	cmp $0x06, %ch
	jne gr
	call this
gr:
	movw $green, %bx
	call puts
	call next_str
	jmp choose

#функция для выбора цвета
choose:
	movb $0x00, %ah #ожидание нажатия и считывание нажатой пользователем клавиши
	int $0x16 #работа с клавиатурой
	cmp $0x48, %ah #up
	je up
	cmp $0x50, %ah #down
	je down
	cmp $0x1c, %ah #enter
	je loader
	jmp choose

#функция выводит стрелочку, указывая выбранный цвет
this:
	movb $0x3e, %al #символ, который будет напечатан
	movb $0x0e, %ah #печать символа на экран
	int $0x10 #вызов прерывания для печати
	ret

#вывод цвета
puts:
	movb 0(%bx), %al #записываем в al нынешний символ
	test %al, %al #логическое и (результат находится в флаге ZF)
	jz end_puts #если было 0 и 0

	movb $0x0e, %ah #печать символа на экран
	int $0x10 #вызов прерывания для печати

	addw $1, %bx #переход к следующему символу
	jmp puts #вывод следующего символа

end_puts:
	ret

#выбор нижнего цвета
down:
	addb $0x01, %ch
	cmp $7, %ch
	je so_many_down
	jmp menu
so_many_down:
	movb $6, %ch
	jmp menu

#выбор верхнего цвета
up:
	subb $0x01, %ch
	cmp $0, %ch
	je so_many_up
	jmp menu
so_many_up:
	movb $1, %ch
	jmp menu

#перенос строки и откат каретки для вывода следующего цвета на след строке
next_str:
	movb $0x0a, %al #символ переноса строки
	movb $0x0e, %ah #печать символа на экран
	int $0x10 #вызов прерывания для печати
	movb $0x0d, %al #символ отката каретки
	movb $0x0e, %ah #печать символа на экран
	int $0x10 #вызов прерывания для печати
	ret

#объявление строк, после их автоматически ставится нулевой байт
gray:
	.asciz "Gray" 
white:
	.asciz "White"
yellow:
	.asciz "Yellow"
cian:
	.asciz "Cian"
magenta:
	.asciz "Magenta"
green:
	.asciz "Green"


loader:
	movw $0x0003, %ax #очистить экран
	int $0x10
	movb %ch, %al #символ переноса строки
	movb $0x0e, %ah #печать символа на экран
	int $0x10 #вызов прерывания для печати
	movb $1, %dl #номер диска
	movb $0, %dh #номер головки
	movb $0, %ch #номер цилиндра
	movb $1, %cl #номер сегмента
	movb $17, %al #сколько секторов считать
	movw $0x1000, %bx
	movw %bx, %es
	movw $0, %bx
	movb $0x02, %ah
	int $0x13

	cli #отключение прерываний

	lgdt gdt_info #загрузить таблицу

	#Включение адресной линии A20
	inb $0x92, %al
	orb $2, %al
	outb %al, $0x92

	#Установка бита PE регистра CR0 - процессор перейдет в защищенный режим
	movl %cr0, %eax
	orb $1, %al
	movl %eax, %cr0

	jmp $0x8, $protected_mode

gdt:
	#Нулевой дескриптор
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	#Сегмент кода
	.byte 0xff, 0xff, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00
	#Сегмент данных
	.byte 0xff, 0xff, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00

gdt_info: #Данные о таблице GDT (размер, положение в памяти)
	.word gdt_info - gdt #Размер таблицы
	.word gdt, 0 #32-битный физический адрес таблицы

	.code32
protected_mode:
	#Загрузка селекторов сегментов для стека и данных в регистры
	movw $0x10, %ax
	movw %ax, %es
	movw %ax, %ds
	movw %ax, %ss
	#Передача управления ядру
	call 0x10000

	#Сектор будет считаться загрузочным, если содержит в конце своих 512 байтов два следующих байта: 0x55 и 0xAA
	.zero (512 -(. - _start) - 2) #заполнение 510 изи 512 байтов нулями
	.byte 0x55, 0xAA #заполнение последних вух байт так, чтобы сектор считался загрузочным