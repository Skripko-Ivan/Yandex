#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ADC_MAX 1024
#define ADC_H_STEP 42//чуть меньше, чтобы можно было максимальное получить
#define ADC_P_STEP 69//чусть больше, чтобы нельзя было максимальное получить (всегда прибавляется 1)
#define OCR2_H_STEP 10
#define OCR2_P_STEP 17
#define PARAM_CYCLES 1500

uint8_t setting_mode; //режим работы: 0 - гирлянда, 1 - настройка 

unsigned int PERIOD,//Период мигания параметра
cycles, param_cycles = 0;
uint8_t x, y, garland_state = 1;
uint8_t a, param_mode = 1;

unsigned char high_adc = 0, low_adc = 0;//для временного хранения младшего и старшего байта регистровой пары в АЦП-прерывании
unsigned int adc_value;

//0 - 9, A - F, -
const unsigned char codes[17] =
{
	0x3f, //0
	0x06, //1
	0x5b, //2
	0x4f, //3
	0x66, //4
	0x6d, //5
	0x7d, //6
	0x07, //7
	0x7f, //8
	0x6f, //9
	0x77, //A
	0x7C, //b
	0x39, //C
	0x5E, //d
	0x79, //E
	0x71, //F
	0x40, //-
};

//массив, который выводим на семисегментник
unsigned char data[3] = { 0x00,0x00,0x00 };

uint8_t b[3] = { 0x0F, 0xFF, 0x00 };//число b - выводится на гирлянду

uint8_t h_p_mode = 1;//0 - h, 1 - p
uint8_t p = 1;//количество миганий в 2 секунды
uint8_t h = 12;//шаг
uint8_t d = 1;//начальное смещение

//значения для семисегментного индикатора
#define h_char 0b01110110;
#define p_char 0b01110011;
#define point  0b10000000;

void output_mode()
{
	if (h_p_mode == 0)
	{
		data[0] = h_char;
	}
	else
	{
		data[0] = p_char;
	}
	data[0] += point;
}

void output_num()
{
	//h
	if (h_p_mode == 0)
	{
		if (h < 12)
		{
			data[1] = codes[16];
			data[2] = codes[12 - h];
		}
		else
			data[1] = codes[h - 12];
	}
	else//p
		data[1] = codes[p];
}

void set_param()
{
	data[2] = 0x00;//0-1 байты всегда будут меняться в функции, 2-ый не всегда - тушим его
	output_mode();
	output_num();
}

void output_param()
{
	uint8_t p_C = PORTC, p_A = PORTA, ind = 0, num = 1 << 3;
	a = 255;
	//мини ожидание, пользователю будет незаметно
	if (param_cycles >= PARAM_CYCLES)
	{
		param_mode = (param_mode + 1) % 2;//мигание
		param_cycles = 0;
	}
	//вывод на семисегментный индикатор 4 символа
	if (param_mode == 1)
	{
		for (ind = 0; ind < 3; ind++)
		{
			PORTA = num;
			PORTC = data[ind];
			_delay_ms(20);
			param_cycles += 20;
			num >>= 1;
		}
		PORTA = num;
		PORTC = 0x00;
		_delay_ms(20);
		param_cycles += 20;
	}
	//вывод параметра, значение не выводится
	else
	{
		PORTA = num;
		PORTC = data[0];
		_delay_ms(20);
		param_cycles += 20;
		num >>= 1;
		for (ind = 1; ind < 3; ind++)
		{
			PORTA = num;
			PORTC = 0x00;
			_delay_ms(20);
			param_cycles += 20;
			num >>= 1;
		}
		PORTA = num;
		PORTC = 0x00;
		_delay_ms(20);
		param_cycles += 20;
	}
	PORTC = p_C;
	PORTA = p_A;
}

void start()
{
	// настройка портов ввода-вывода
	DDRA = 0xdf;//Вывод PA5 настроен на ввод, для работы гирлянды он игнорируется (изменения в логику работы гирлянды не вносятся).
	DDRB = 0xff;
	DDRC = 0xff;
	DDRD = 0xf3;//PD2 - смена режима, PD3 - смена настраиваемого параметра
	//яркость PD7 показывает степень отдаленности настраиваемого параметра от левой границы\
	яркость PD4 - от правой границы

	// установка глобальных переменных
	setting_mode = 0; // при включении запускается режим гирлянды

	//делается начальный сдвиг
	for (unsigned char i = 0; i < d; i++)
	{
		uint8_t C = b[0] / 128;
		b[0] <<= 1;
		b[0] += b[1] / 128;//самый старший бит
		b[1] <<= 1;
		b[1] += b[2] / 128;
		b[2] <<= 1;
		b[2] += C;
	}

	PORTA = b[0];
	PORTB = b[1];
	PORTC = b[2];
	PORTD = 0x90;//включаем PD7 и PD4 для показания отдаленности настраиваемого параметра

	// настройка внешних прерываний
	MCUCR |= (3 << ISC00) | (3 << ISC10); // устанавливаем внешние прерывания INT0 и INT1 на срабатывание по переднему фронту (переход 0 -> 1)

	GIFR = (1 << INT0) | (1 << INT1); // сброс флагов в регистре GIFR (исключение ложного срабатывания прерываний в момент включения)
	GICR |= (1 << INT0) | (1 << INT1); // разрешение прерываний INT0

	sei(); // разрешаем работу прерываний

	set_param();
}

void change_garland_state()
{
	cycles = 0;
	garland_state = (garland_state + 1) % 2;//определяем новое состояние
	if (garland_state)//делаем сдвиг и зажигаем
	{
		if (h < 12)
		{
			uint8_t h_cpy = 12 - h;
			for (unsigned char i = 0; i < h_cpy; i++)
			{
				uint8_t C = b[2] % 2;
				b[2] >>= 1;
				b[2] += b[1] % 2 << 7;
				b[1] >>= 1;
				b[1] += b[0] % 2 << 7;
				b[0] >>= 1;
				b[0] += C << 7;
			}
		}
		else
		{
			uint8_t h_cpy = h - 12;
			for (unsigned char i = 0; i < h_cpy; i++)
			{
				uint8_t C = b[0] / 128;
				b[0] <<= 1;
				b[0] += b[1] / 128;
				b[1] <<= 1;
				b[1] += b[2] / 128;
				b[2] <<= 1;
				b[2] += C;
			}
		}
		PORTA = b[0];
		PORTB = b[1];
		PORTC = b[2];
	}
	else//тушим
	{
		PORTA = 0;
		PORTB = 0;
		PORTC = 0;
	}
}

void ADC_Init(void)
{
	ADCSRA |= (1 << ADEN) // Разрешение использования АЦП
		| (1 << ADSC)//Запуск преобразования
		| (1 << ADATE)//Непрерывный режим работы АЦП
		| (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)//Делитель 8МГц/128 = 64 кГц
		| (1 << ADIE);//Разрешение прерывания от АЦП
	ADMUX |= (1 << REFS1) | (1 << REFS0)//Внутренний Источник ОН 2,56в, вход ADC0
		| (1 << MUX2) | (1 << MUX0);//0101 для включения ADC5 - идет от PA5
}

//OCR2 работает с PD7
void init_PWM_timer(void)
{
	ASSR = 0x00;
	TCCR2 = 0b01101110; //Fast PWM, Clear OC2 on Compare Match, clkT2S/256 (From prescaler)\
	(реальная частота получится 8МГц/256 = 31250 Гц)
	TCNT2 = 0x00; // Timer Value = 0 - Обнуление счетчика
	OCR2 = 0x00;//скважность шим (0 — это 0%, а FF — 100 %)
	TIMSK |= 0x00;
	// Timer/Counter 1 initialization
	//0001 (WGM) - 8-bit mode
	TCCR1A = 0b00000001;
	TCCR1B = 0b00010100;
	//Timer Value
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	//скважность - OCR1AH не пользуемся, поэтому 0
	OCR1AH = 0x00;
	OCR1AL = 0x00;
	//изначально 0, далее будет меняться, пользуемся только правым
	OCR1BH = 0x00;
	OCR1BL = 0x00;
}

int main()
{
	ADC_Init();//Инициализируем АЦП
	start();//настраиваем порты, значения переменных, внешние прерывания INT0 и INT1
	init_PWM_timer();
	PERIOD = 6000 / (p);//каждые PERIOD мс будет мигать параметр
	cycles = 0;
	while (1)
	{
		if (setting_mode == 0)//гирлянда
		{
			_delay_ms(50);
			cycles += 50;
			if (++cycles >= PERIOD)//часть ожидания прошла
			{
				change_garland_state();
				cycles = 0;
			}
		}
		else//режим настройки
		{
			set_param();
			output_param();
		}
	}
	return 0;
}

//PD2 - изменение режима работы
ISR(INT0_vect)
{
	setting_mode = (setting_mode + 1) % 2;
}

//PD3 - изменение настраиваемого параметра
ISR(INT1_vect)
{
	//работает только в режиме настройки
	if (setting_mode == 1)
		h_p_mode = (h_p_mode + 1) % 2;
}

//прерывания АЦП для настройки параметров
ISR(ADC_vect)
{
	//работает только в режиме настройки
	if (setting_mode == 1)
	{
		//Читать нужно сначала младшую часть, так как следующее преобразование автоматически\
		начинается именно в случае такого порядка считывания регистров.
		low_adc = ADCL;
		high_adc = ADCH;
		adc_value = high_adc * 256 + low_adc;//собираем итоговое значение
		//итоговое значение 10-битное->1024 отрезка
		if (h_p_mode == 0)//h (-12;12) (0;24)
		{
			h = adc_value / ADC_H_STEP;
			OCR2 = 255 - h * OCR2_H_STEP;
			OCR1BH = h * OCR2_H_STEP;
		}
		else//p (1-15)
		{
			p = adc_value / ADC_P_STEP + 1;
			OCR2 = 255 - p * OCR2_P_STEP;
			OCR1BH = p * OCR2_P_STEP;
			PERIOD = 6000 / (p / 2);
		}
	}
}
