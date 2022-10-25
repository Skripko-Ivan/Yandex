//��� ���������� ����������� ������ ���� ������, �.�. ���� ��� ������������� � ��������,
//� ��������� �������� ���������� �� ������ ������ ���������� ��������� ������ ���� ��.
__asm("jmp kmain");

#define ADDR_VIDEO_BUF 0xb8000 //����� ������ ������ ��� ������ � �������������

#define PIC1_PORT 0x20

//������� ���� ���������� �������� ���������� ������
#define CURSOR_PORT 0x3D4
#define VIDEO_WIDTH 80//������ ���������� ������
#define VIDEO_HEIGHT 48//������ ���������� ������

//���� ��������
#define BACKSPACE 14
#define ENTER 28
#define SPACE 57

//��� ������ � ��������
unsigned char line = 0;
unsigned char column = 0;

//���� ������
unsigned char color = 0;

//�������� ������ ����, ������������� ����������� ��
#define GDT_CS 0x8

#define IDT_TYPE_INTR 0x0E
//#define IDT_TYPE_TRAP 0x0F

//������ ��� �������� �������� � ascii
unsigned char scan_codes[] =
{
        0,
        0,  // ESC(this button starts with number "1")
        '1','2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        0,  // BACKSPACE(14)
        0,  // TAB(15)
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0, 0,
        ' ',// ENTER(28)
        0,  // CTRL(29)
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0,0,'+',
        0,  // left SHIFT(42)
        0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', '+', '*', '/',
        0,  // right SHIFT(54)
        '*',// NUMPAD *
        0,  // ALT
        ' ', // SPACE
        0, //CAPSLOCK
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //F1 - F10
        0, //NUMLOCK
        0, //SCROLLOCK
        0, //HOME
        0,
        0, //PAGE UP
        '-', //NUMPAD
        0, 0,
        0, //(r)
        '+', //NUMPAD
};

//��������� ��������� ������ �� ����������� ����������
struct idt_entry
{
    unsigned short base_lo;//������� ���� ������ �����������
    unsigned short segm_sel;//�������� �������� ����
    unsigned char always0;//���� ���� ������ 0
    unsigned char flags;//����� ���. �����: P, DPL, ���� - ��� ��������� - IDT_TYPE...
    unsigned short base_hi;//������� ���� ������ �����������
} __attribute__((packed));//������������ ���������

//���������, ����� ������� ���������� ��� �������� ������� lidt
struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));//������������ ���������

struct idt_entry g_idt[256];//�������� ������� IDT
struct idt_ptr g_idtp;//��������� ������� ��� ������� lidt

//typedef void (*intr_handler)();
// ������ ���������� ����������
void default_intr_handler()
{
    asm("pusha");
    asm("popa; leave; iret");
}
typedef void (*intr_handler)();

//����������
//void default_intr_handler();
void intr_reg_handler(int num, unsigned short segm_sel, unsigned short flags, intr_handler hndlr);
void intr_init();
void intr_start();
void intr_enable();
void intr_disable();

//������ � ���������
static inline unsigned char inb(unsigned short port);
static inline void outb(unsigned short port, unsigned char data);
static inline void outw(unsigned int port, unsigned int data);
void keyb_init();
void keyb_handler();
void keyb_process_keys();

//��� �������
void write_string(unsigned char* text);//������� ������ �� �����
void init_text_clr();//�������������� ���� ������
void clear_screen();//������� ���� �����
void set_cursor();//������������� ������
void info();
void on_key(unsigned char scan_code);
void on_backspase();
void on_enter();
void on_symbol(const unsigned char symbol);
void choose_command();
short strcmp(const unsigned char* first, const unsigned char* second);
unsigned short strlen(const unsigned char* str);
char strchr(const unsigned char* str, unsigned char letter);
void print_error();
short str_to_number(const unsigned char* str, const unsigned char size);
void number_to_str(short n, unsigned char* str);
short nod(short n1, short n2);
void print_error_arg();
void print_error_overflow();
void make_fraction(const unsigned char* num, const unsigned char* den, unsigned char* out);

void intr_reg_handler(int num, unsigned short segm_sel, unsigned short flags, intr_handler hndlr)
{
    unsigned int hndlr_addr = (unsigned int)hndlr;
    g_idt[num].base_lo = (unsigned short)(hndlr_addr & 0xFFFF);
    g_idt[num].segm_sel = segm_sel;
    g_idt[num].always0 = 0;
    g_idt[num].flags = flags;
    g_idt[num].base_hi = (unsigned short)(hndlr_addr >> 16);
}


// ������� ������������� ������� ����������: ���������� ������� � �������� ������������
void intr_init()
{
    int i;
    int idt_count = sizeof(g_idt) / sizeof(g_idt[0]);
    for (i = 0; i < idt_count; i++)
        intr_reg_handler(i, GDT_CS, 0x80 | IDT_TYPE_INTR, default_intr_handler); // segm_sel=0x8, P=1, DPL=0, Type=Intr
}

// ������� ����������� ������� ������������ ����������
void intr_start()
{
    int idt_count = sizeof(g_idt) / sizeof(g_idt[0]);//���������� ��������� ������� IDT
    g_idtp.base = (unsigned int)(&g_idt[0]);
    g_idtp.limit = (sizeof(struct idt_entry) * idt_count) - 1;
    asm("lidt %0" : : "m" (g_idtp));
}

//��������� ����������
void intr_enable()
{
    asm("sti");
}

//��������� ����������
void intr_disable()
{
    asm("cli");
}

//������ �� �����
static inline unsigned char inb(unsigned short port)
{
    unsigned char data;
    asm volatile ("inb %w1, %b0" : "=a" (data) : "Nd" (port));
    return data;
}

//������ � ����� ������
static inline void outb(unsigned short port, unsigned char data) // ������
{
    asm volatile ("outb %b0, %w1" : : "a" (data), "Nd" (port));
}

//��� shutdown
static inline void outw(unsigned int port, unsigned int data)
{
    asm volatile ("outw %w0, %w1" : : "a" (data), "Nd" (port));
}

// ������������ ���������� ���������� ���������� � ��������� ����������� ���������� ��� �������� � ������ ������� ������������� ������� ����������
void keyb_init()
{
    // ����������� ����������� ����������
    intr_reg_handler(0x09, GDT_CS, 0x80 | IDT_TYPE_INTR, keyb_handler);
    // segm_sel=0x8, P=1, DPL=0, Type=Intr
        // ���������� ������ ���������� ���������� �� ����������� 8259
    outb(PIC1_PORT + 1, 0xFF ^ 0x02); // 0xFF - ��� ����������, 0x02 - ��� IRQ1 (����������).
// ��������� ����� ������ ����������, ��� ���� ����������� � 0
}

// ���������� ����������
void keyb_handler()
{
    asm("pusha");
    // ��������� ����������� ������
    keyb_process_keys();
    // �������� ����������� 8259 ����������� � ���, ��� ���������� ����������
    outb(PIC1_PORT, 0x20);
    asm("popa; leave; iret");
}

// ��������� ����������� �� ������������ ������
void keyb_process_keys()
{
    // �������� ��� ����� PS/2 ���������� �� ���� (������� ��� ������������)
    if (inb(0x64) & 0x01)
    {
        unsigned char scan_code;
        scan_code = inb(0x60); // ���������� ������� � PS/2 ����������
        if (scan_code < 128) // ����-���� ���� 128 - ��� ���������� �������
            on_key(scan_code);
    }
}

void on_key(unsigned char scan_code)
{
    unsigned char symbol = scan_codes[(unsigned int)scan_code];
    if (scan_code == BACKSPACE)
    {
        on_backspase();
        return;
    }
    if (scan_code == ENTER)
    {
        on_enter();
        return;
    }
    on_symbol(symbol);
}

void on_backspase()
{
    if (!column)//���� ����� ����� ������� � ������, �� ������ �� ��������
        return;
    unsigned char* vb = (unsigned char*)ADDR_VIDEO_BUF + 2 * (VIDEO_WIDTH * line + column);
    vb[0] = 0;//������� ������ � ������
    column--;//������ ������� ������� �� 1 �����
    set_cursor();//������������� ������ �����
}

void on_enter()
{
    unsigned char* vb = (unsigned char*)ADDR_VIDEO_BUF + 2 * (VIDEO_WIDTH * line + column);
    vb[0] = column = 0;//������������� ����� ������ � ���������� �������
    line++;//��������� ������
    set_cursor();//������������� ������
    choose_command();//��� ��� ������������ ������ �������, �� ������������ � ���������
}

void on_symbol(const unsigned char symbol)
{
    if (column == 40)//�� ������� ������������ �� ����� ������ ����� 40 ��������, ������� ����� ��������������
        return;
    unsigned char* vb = (unsigned char*)ADDR_VIDEO_BUF + 2 * (VIDEO_WIDTH * line + column);
    vb[0] = symbol;
    column++;
    set_cursor();
}

void choose_command()
{
    //���� ���������� ��� ��������� ������, ������� ������� �� ������ ����
    unsigned char* vb = (unsigned char*)ADDR_VIDEO_BUF + 2 * VIDEO_WIDTH * (line - 1);
    //41 ������ - 40 ��������� � 1 ������ ��������� ������
    unsigned char command[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    //�������� ��� ������ � ��������� ��������, ������� ���� ������������
    unsigned char j=0;
    for (unsigned char i = 0; vb[i] != '\0'; i += 2)
        command[j++] = vb[i];
    command[j]=0;

    if (*vb == 'i')
    {
        write_string(command);
        unsigned char ch[]="info";
        if (!strcmp(command, ch))//��������� ��������� �� ������� ������������ �������
        {
            info();
            return;
        }
    }
    if (*vb == 's')
    {
        unsigned char ch[]="shutdown";
        if (!strcmp(command, ch))//��������� ��������� �� ������� ������������ �������
        {
            outw(0x604, 0x2000);
            return;
        }
    }
    //���� ����� �������� ���������
    if (strlen(command) >= 4)
    {//���� ��� ���������� �������
        //���� ��� gcd
        if (command[0] == 'g' && command[1] == 'c' && command[2] == 'd' && command[3] == ' ')
        {
            unsigned char* ptr = command + 4;//������ ��������� �� ������� � �������� 4 - ������ ������� �����
            char space_pos = strchr(ptr,(unsigned char)' ');//������� ������� ������� �����
            if (space_pos == -1)
            {
                print_error();//������������ ���� ������ 1 ��������
                return;
            }
            //��������� 2 ���������
            short num[2] = { 0,0 };
            for (unsigned char i = 0; i < 2; i++)
            {
                if (*ptr == '-')
                {
                    print_error_arg();//������������ ���� ������������� �����
                    return;
                }
                num[i] = str_to_number(ptr, (unsigned char)space_pos);
                if (num[i] == -1)
                {
                    print_error_arg();//������������ ���� ������������ ��������
                    return;
                }
                if (num[i] == -2)
                {
                    print_error_overflow();//������������ ���� ������� �������/������� �����
                    return;
                }
                ptr += space_pos+1;//��������� �� ������� ���������
                space_pos = (char)strlen(ptr);//������� ����� ������� ���������
            }
            short res=nod(num[0], num[1]);
            unsigned char out[6];
            number_to_str(res, out);
            if (line > 22)
                clear_screen();
            write_string(out);
            return;
        }
        //���� ��� solve
        if (command[0] == 's' && command[1] == 'o' && command[2] == 'l' && command[3] == 'v'&&
            command[4] == 'e'&& command[5] == ' ')
        {
            unsigned char* ptr = command + 6;//������ ��������� �� ������� � �������� 6 - ������ ���������

            unsigned char negative = 0;
            short num[3] = { 0,0,0 };
            for (unsigned char i = 0; i < 3; i++)
            {
                //ax+b=c
                //������ � �/b/c
                if (*ptr == '-')
                {
                    negative = 1;//����������, ��� a/b �������������
                    ptr++;//������ ��������� �� ������ �/b
                }
                else if (*ptr == '+')
                    ptr++;//������ ��������� �� ������ �/b
                    else if (*ptr=='=')
                        ptr++;
                //���� ����� |ax| � ������ ������, b - �� ������
                char try_end_arg=-1, end_arg=-1;
                if (i == 0)
                {
                    try_end_arg = strchr(ptr, (unsigned char)'+');
                    end_arg = strchr(ptr, (unsigned char)'-');
                }
                if (i==1)
                    end_arg = strchr(ptr, (unsigned char)'=');
                if (i == 2)
                    end_arg = strlen(ptr);
                if ((try_end_arg != -1 && try_end_arg < end_arg)||end_arg==-1)
                    end_arg = try_end_arg;
                if (i == 0&&end_arg!=-1) end_arg--;//� ������������� �� ����
                if (end_arg == -1 || end_arg > 5)
                {
                    print_error_arg();//������������ ���� ������������ ��������
                    return;
                }
                if (end_arg==0) num[i]=1;
                else num[i] = str_to_number(ptr, (unsigned char)(end_arg));
                if (num[i] == -1)
                {
                    print_error_arg();//������������ ���� ������������ ��������
                    return;
                }
                if (num[i] == -2)
                {
                    print_error_overflow();//������������ ���� ������� �������/������� �����
                    return;
                }
                if (negative)//���� �/b/c �������������
                    num[i] *= -1;
                ptr += end_arg;//��������� � b/c
                if (i == 0) ptr++;//���� ���������� �
                negative = 0;
            }
            //num[0] - a, num[1] - b, num[2] - c
            short right = num[2] - num[1];//��������� b � ������ �������
            short left = num[0];
            short res;
            char var = 1;
            if (right % left == 0)//���� ����� �������������
            {
                res = right / left;
            }
            else//����� �� �������������
            {
                short del;
                //���� ��� �� ����� 1, ������� ��� � ����� �� ����
                while ((del=nod(right, left)) != 1)
                {
                    right /= del;
                    left /= del;
                }
                var++;//����������, ��� ����� ���������������
            }
            if (line > 22)
                clear_screen();
            unsigned char out[13];
            unsigned char* ptr_out = out;
            if (var == 1)//���� ������������� �����
            {
                if (res < 0)//���� ������������� ���������
                {
                    *ptr_out = '-';
                    res *= -1;
                    ptr_out++;
                }
                number_to_str(res, ptr_out);
            }
            else//��������������� �����
            {
                unsigned char numerator[6];
                unsigned char denominator[6];
                if (right / left < 0)
                {
                    *ptr_out = '-';
                    ptr_out++;
                }
                if (right < 0) right *= -1;
                if (left < 0) left *= -1;
                number_to_str(right, numerator);
                number_to_str(left, denominator);
                make_fraction(numerator, denominator, ptr_out);
            }
            write_string(out);
            return;
        }
    }
    print_error();//���� ������� �� ������ � ������� �� �����������, �� ����� ����������� ������� ������
}

void make_fraction(const unsigned char* num, const unsigned char* den, unsigned char* out)
{
    while (*num)
    {
        *out = *num;
        num++;
        out++;
    }
    *out = '/';
    out++;
    while (*den)
    {
        *out = *den;
        den++;
        out++;
    }
    *out = 0;
}

//����� ����������� ������ ��������
short nod(short n1, short n2)
{
    while (n1 > 0 && n2 > 0)
        if (n1 > n2)
            n1 %= n2;
        else
            n2 %= n1;
    short res = n1 + n2;
    return res;
}

void number_to_str(short n, unsigned char* str)
{
    unsigned char size = 0;
    short cpy = n;
    //������ ���������� ��������
    while (cpy > 0)
    {
        size++;
        cpy /= 10;
    }
    //��������� ����� � ������
    str[size] = 0;
    while (n > 0)
    {
        str[size - 1] = n % 10 + 48;
        n /= 10;
        size--;
    }
}

//������� ������ � �����
short str_to_number(const unsigned char* str, unsigned char size)
{
    //��������� ����� ������
    if (size == 0 || size > 5) return -2;
    //��������� ������������ ������
    for (unsigned char i = 0; i < size; i++)
    {
        if (str[i] < 48 || str[i]>57)
            return -1;
    }
    //��������� ������������: >32767
    if (size == 5)
    {
        unsigned char max[]="32767";
        if (strcmp(str, max) > 0)
            return -2;
    }
    short n = 0;
    for (unsigned char i=0;i<size;i++)
    {
        n*=10;
        n += (short)(str[i]) - 48;
    }
    return n;
}

short strcmp(const unsigned char* first, const unsigned char* second)
{
    while (*first)
    {
        if (!*second) return 1;
        if (*second > *first) return -1;
        if (*second < *first) return 1;
        second++;
        first++;
    }
    if (*second) return -1;
    return 0;
}

unsigned short strlen(const unsigned char* str)
{
    unsigned short len = 0;
    while (*str != '\0')
    {
        str++;
        len++;
    }
    return len;
}

//������� ������� ������� � ������, ���� ������ �� ������, �� ���������� -1
char strchr(const unsigned char* str, unsigned char letter)
{
    char pos = 0;
    while (*str)
        if (*str == letter)
            return pos;
        else
        {
            pos++;
            str++;
        }
    return -1;
}

//����� ������ �� �����
void print_error()
{
    if (line > 20)
        clear_screen();
    unsigned char error[] = "Incorrect command";
    write_string(error);
    unsigned char commands[] = "Correct commands: info, gcd dig1 dig2, solve equation, shutdown";
    write_string(commands);
}

void print_error_arg()
{
    if (line > 21)
        clear_screen();
    unsigned char error[] = "Incorrect argument";
    write_string(error);
}

void print_error_overflow()
{
    if (line > 21)
        clear_screen();
    unsigned char error[] = "Short overflow";
    write_string(error);
}

//��������� �����, ������� ������ ������������, ������
void init_text_clr()
{
    //��������� ���� � ���������� �����, ������ ���� ����������, ��� ����� ��������� ������
    unsigned char* vb = (unsigned char*)ADDR_VIDEO_BUF;
    for (unsigned short i = 1; i < VIDEO_HEIGHT * VIDEO_WIDTH; i += 2)
        vb[i] = color;
}

//������� ������ � ��������� ������� � ������
void clear_screen()
{
    //��������� ������ ������ ���� � ����� �������, ������ � ���������� �� �������
    unsigned char* vb = (unsigned char*)ADDR_VIDEO_BUF;
    for (unsigned short i = 0; i < VIDEO_HEIGHT * VIDEO_WIDTH; i += 2)
        vb[i] = 0;
    line = column = 0;
    set_cursor();
}

//��������� ������� � ����� ����� �� ������
void set_cursor()
{
    //���������� �� 2, ��� ��� �� 2 ����� �� ������
    unsigned char* vb = (unsigned char*)ADDR_VIDEO_BUF + 2 * (VIDEO_WIDTH * line + column);
    vb[0] = ' ';

    unsigned short new_pos = (line * VIDEO_WIDTH) + column;
    outb(CURSOR_PORT, 0x0F);
    outb(CURSOR_PORT + 1, (unsigned char)(new_pos & 0xFF));
    outb(CURSOR_PORT, 0x0E);
    outb(CURSOR_PORT + 1, (unsigned char)((new_pos >> 8) & 0xFF));
}

//����� ������ �� �����
void write_string(unsigned char* text)
{
    unsigned char* vb = (unsigned char*)ADDR_VIDEO_BUF + 2 * VIDEO_WIDTH * line;
    while (*text)
    {
        vb[0] = *text;
        vb += 2;
        text++;
    }
    column = 0;
    line++;
    set_cursor();
}

//����� ����������
void info()
{
    if (line > 20)
        clear_screen();
    unsigned char info_1[] = "Author: Skripko Ivan; Group: 4851003/00002";
    write_string(info_1);
    unsigned char info_2[] = "Compiler: gcc; Assembly Language Translator: YASM";
    write_string(info_2);
    if (color == 0x0F)
    {
        unsigned char info_3[] = "Text color in console: white";
        write_string(info_3);
        return;
    }
    if (color == 0x0E)
    {
        unsigned char info_3[] = "Text color in console: yellow";
        write_string(info_3);
        return;
    }
    if (color == 0x03)
    {
        unsigned char info_3[] = "Text color in console: cian";
        write_string(info_3);
        return;
    }
    if (color == 0x0B)
    {
        unsigned char info_3[] = "Text color in console: magenta";
        write_string(info_3);
        return;
    }
    if (color == 0x02)
    {
        unsigned char info_3[] = "Text color in console: green";
        write_string(info_3);
        return;
    }
    else
    {
        unsigned char info_3[] = "Text color in console: gray";
        write_string(info_3);
        return;
    }
}

extern "C" int kmain()
{
    //init text color
    if (*(unsigned char*)ADDR_VIDEO_BUF == 1)
        color = 0x08;
    if (*(unsigned char*)ADDR_VIDEO_BUF == 2)
        color = 0x0F;
    if (*(unsigned char*)ADDR_VIDEO_BUF == 3)
        color = 0x0E;
    if (*(unsigned char*)ADDR_VIDEO_BUF == 4)
        color = 0x03;
    if (*(unsigned char*)ADDR_VIDEO_BUF == 5)
        color = 0x0B;
    if (*(unsigned char*)ADDR_VIDEO_BUF == 6)
        color = 0x02;
    clear_screen();
    init_text_clr();
    unsigned char welcome[] = "Welcome to SolverOS!";
    write_string(welcome);
    intr_init();
    keyb_init();
    intr_start();
    intr_enable();

    while (1)
    {
        asm("hlt");
    }
    return 0;
}
