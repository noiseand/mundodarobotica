
void lcd_comando(unsigned char cmd);
void lcd_envia_byte(unsigned char control, unsigned char dado);
void lcd_escreve(const rom char *matriz_lcd);
void lcd_escreve_bf(char *s_caracteres);
void lcd_posicao(unsigned char linha, unsigned char coluna);
void lcd_inicio();

#define D_EN TRISBbits.TRISB2	//Define o nome D_EN para o bit 2 do reg. TRISB (EN do LCD) // M21
#define D_RS TRISBbits.TRISB5	//Define o nome D_RS para o bit 5 do reg. TRISB (RS do LCD) // M11

#define D_D4 TRISCbits.TRISC7 	//Define o nome D_D4 para o bit 7 do reg. TRISC (dado 4 do LCD) // M42
#define D_D5 TRISDbits.TRISD4 	//Define o nome D_D5 para o bit 4 do reg. TRISD (dado 5 do LCD) // M31
#define D_D6 TRISDbits.TRISD3 	//Define o nome D_D6 para o bit 3 do reg. TRISD (dado 6 do LCD) // M32
#define D_D7 TRISCbits.TRISC6 	//Define o nome D_D7 para o bit 6 do reg. TRISC (dado 7 do LCD) // M41

#define EN PORTBbits.RB2		//Define o nome EN para o bit 2 da porta B (EN do LCD)
#define RS PORTBbits.RB5		//Define o nome RS para o bit 5 da porta B (RS do LCD)

#define D4 PORTCbits.RC7 		//Define o nome D4 para o bit 7 da porta C (dado 4 do LCD) // M42
#define D5 PORTDbits.RD4 		//Define o nome D5 para o bit 4 da porta D (dado 5 do LCD) // M31
#define D6 PORTDbits.RD3 		//Define o nome D6 para o bit 3 da porta D (dado 6 do LCD) // M32
#define D7 PORTCbits.RC6 		//Define o nome D7 para o bit 6 da porta C (dado 7 do LCD) // M41

void lcd_comando(unsigned char cmd)
{
D7 = cmd>>7;		//Desloca em 7 posições a variável cmd e armazena em D7
D6 = cmd>>6;		//Desloca em 6 posições a variável cmd e armazena em D6
D5 = cmd>>5;		//Desloca em 5 posições a variável cmd e armazena em D5
D4 = cmd>>4;		//Desloca em 4 posições a variável cmd e armazena em D4

EN = 1;				//Coloca em 1 o bit RE1 (EN)
EN = 0;				//Coloca em 0 o bit RE1 (EN)

D7 = cmd>>3;		//Desloca em 3 posições a variável cmd e armazena em D3
D6 = cmd>>2;		//Desloca em 2 posições a variável cmd e armazena em D2
D5 = cmd>>1;		//Desloca em 1 posições a variável cmd e armazena em D1
D4 = cmd;

EN = 1;				//Coloca em 1 o bit RE1 (EN)
EN = 0;				//Coloca em 0 o bit RE1 (EN)
}

void lcd_envia_byte(unsigned char control, unsigned char dado)
{
RS = 0;				//Coloca em 0 o bit RE0 (RS)
RS = control;		//Coloca em 0 ou 1 o bit RE0 (RS) dependendo de 'control'
Delay100TCYx(5);	//Delay de aproximadamente 100us (cristal 20MHz)
EN = 0;				//Coloca em 0 o bit RE1 (EN)
lcd_comando(dado);	//desvia para a função lcd_comando com o argumento 'dado'
}

void lcd_escreve(const rom char *matriz_lcd)
{
while (*matriz_lcd!=0)		//Enquanto a posição apontada pela variável
							//matriz_lcd for diferente de 0
	{
	lcd_envia_byte(1,*matriz_lcd); 	//Desvia para a função lcd_envia_byte com
									//o argumento 1 e o apontamento da variável
									//matriz_lcd
	matriz_lcd++;					//incrementa a posição do apontado matriz_lcd
	}
}

void lcd_escreve_bf(char *s_caracteres)
{
unsigned char tam_buffer=16;
	while (tam_buffer--)
	{
		lcd_envia_byte(1,*s_caracteres);
		s_caracteres++;
	}
}

void lcd_inicio(void)
{
char conta;			//Declaração da variável conta com 8 bits

D_RS = 0;			//Direciona o pino 0 da porta E como saída
D_EN = 0;			//Direciona o pino 1 da porta E como saída
D_D4 = 0;			//Direciona o pino 4 da porta D como saída
D_D5 = 0;			//Direciona o pino 5 da porta D como saída
D_D6 = 0;			//Direciona o pino 6 da porta D como saída
D_D7 = 0;			//Direciona o pino 7 da porta D como saída

EN = 0;				//Coloca em 0 o bit RE1(EN)
RS = 0;				//Coloca em 0 o bit RE0(RS)
D4 = 0;				//Coloca em 0 o bit RD4(D4)
D5 = 0;				//Coloca em 0 o bit RD5(D5)
D6 = 0;				//Coloca em 0 o bit RD6(D6)
D7 = 0;				//Coloca em 0 o bit RD7(D7)

Delay10KTCYx(10);	//Gera delay de 15ms (30000 ciclos de máquina)
					//Delay = 15m * 2M = 30000

//desvia para a função lcd_comando como o argumento 0x03 por 3 vezes
//em intervalos de 5ms.
for (conta = 1; conta <= 3; conta++)
	{
	lcd_comando(0x03);		//desvia para a função lcd_comando com o
							//argumento 0x03
	Delay1KTCYx(25);		//Gera delay de 5ms (10000 ciclos de máquina)
     }

lcd_envia_byte(0,0x28);		//desvia p/lcd_envia_byte com os arg. 0 e 0x28
lcd_envia_byte(0,0x0C);		//desvia p/lcd_envia_byte com os arg. 0 e 0x0C
Delay1KTCYx(5); 			//Gera atraso de 1ms;
lcd_envia_byte(0,0x01);		//desvia p/lcd_envia_byte com os arg. 0 e 0x01
lcd_envia_byte(0,0x06);		//desvia p/lcd_envia_byte com os arg. 0 e 0x06
Delay1KTCYx(25);			//Gera delay de 5ms (10000 ciclos de máquina)
}

////

void lcd_posicao(unsigned char linha, unsigned char coluna)
{
	switch (linha)
	{
		case 1:
			lcd_envia_byte (0,0x80+coluna-1);
			break;
		case 2:
			lcd_envia_byte (0,0xc0+coluna-1);
			break;
		case 3:
			lcd_envia_byte (0,0x94+coluna-1);
			break;
		case 4:
			lcd_envia_byte (0,0xd4+coluna-1);
			break;
	}
}
