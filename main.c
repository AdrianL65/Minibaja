#include "derivative.h" /* include peripheral declarations */


unsigned long centesimas=0;
unsigned long tiempo_final=0;
unsigned char aux_LPTMR=0;

void LPTimer_IRQHandler() //ISR para LPTMR0
{
	
	LPTMR0_CSR|=(1<<7);    //b7=1 apagar la bandera, b6=1siga intr hab, b0=1 siga hab intr
	centesimas=centesimas+1;
	
	aux_LPTMR=aux_LPTMR+1; //incrementa en 1 el auxiliar para llegar a 101 cuentas
	
	if (aux_LPTMR>100){ //tiene que llegar a 101 para iniciar mandar, no es 100 porque empieza en 1 el auxiliar
		
	aux_LPTMR=0; //se reinicia la cuenta
	send_inf(centesimas); //se manda info
	
	}
	
	//salto_linea_corredera();
}




void PORTD_IRQHandler(){
	
	if ((((PORTD_PCR4 & (1<<24)))!=0)  & ((GPIOD_PDIR & (1<<3))==0)){ //lógica del sensor 1 inicio
		PORTD_PCR4|= (1<<24); //apagar bandera
	
	
		//FALTA PONER EN LÓGICA INVERSA CON !=1
		
		LPTMR0_CSR|=(1<<6)+1;	//Hab Intr LPtmr (bit 6), Hab timer (bit 0 = 1) //tomar ESTEEEE
		NVIC_ISER|=(1<<28);				//Hab intr LPTMR0 desde NVIC (pag 53)
		
		//while ((UART0_S1&(1<<7)) == 0 ); //esperar para mandar información
		//UART0_D ='C'; //mandar C cuando se pase el inicio con llantas bien posicionadas
		
		//salto_linea_corredera();
	
		
	}
	
	else if ((PORTD_PCR0 & (1<<24))!=0){ //lógica del sensor 2 final, se llegó al final
		PORTD_PCR0|= (1<<24);
		
		//while ((UART0_S1&(1<<7)) == 0 ); //esperar para mandar información
		//UART0_D ='P'; //mandar una P en la cuando se llegue al objetivo final
		
		//salto_linea_corredera();
			 
		tiempo_final = centesimas;
			 
	    send_inf(tiempo_final);
	    //salto_linea_corredera();
	
	    NVIC_ICER=(1<<28);		
	}
	else if ((PORTD_PCR5 & (1<<24))!=0){
			PORTD_PCR5|= (1<<24); //apagar banderaPORTD_PCR4|= (1<<24); //apagar bandera
			centesimas=0;
			tiempo_final=0;
			aux_LPTMR=0;
			
		}
	

}

void LPTMER_init(){
	SIM_SCGC5|=(1<<0);		//LPTMR

	LPTMR0_PSR|=5;					//101 : bypass del preescaler, seleccion de LPO (1 KHz)
	//LPTMR0_CSR=(1<<6)+1;			//Hab Intr LPtmr (bit 6), Hab timer (bit 0 = 1) //tomar ESTEEEE
	LPTMR0_CMR=10-1;
	
	//NVIC_ISER|=(1<<28);				//Hab intr LPTMR0 desde NVIC (pag 53)
}

void port_D_init(){
	SIM_SCGC5=(1<<12); //PORT D
	
	//pines de lectura con int, default input todos
	PORTD_PCR4 = (10<<16)+(1<<8)+3; //INT en falling edge, pin 4 GPIO, lectura de sensor 1 incio
	PORTD_PCR0 = (10<<16)+(1<<8)+3; //INT en falling edge, pin 0 GPIO, lectura de sensor 2 final
	PORTD_PCR3 = (1<<8);//lectura de sensor 3 llantas alineadas - pura lectura de GPIO
	PORTD_PCR5 = (9<<16)+(1<<8)+3; //INT en rising edge, pin 4 GPIO, lectura de reset
	
	NVIC_ISER|=(1<<31); //interrupción del puerto D
}
void clk_init (void)
{
// FIRC = 4 MHz. BusClk = 4 MHz
// UART0: FIRC. UART1: BusClk. UART2: BusClk. TPM: FIRC. IIC: BusClk

MCG_C1|=(1<<6) + (1<<1);	//MCGOUTCLK : IRCLK. CG: Clock gate, MCGIRCLK enable pag 116
MCG_C2|=1;					//Mux IRCLK : FIRC (4 MHz) pag 116
MCG_SC=0;					//Preescaler FIRC 1:1 pag 116

SIM_CLKDIV1=0;				//OUTDIV4=OUTDIV1= 1:1 pag 116. Busclk 4 MHz
SIM_SOPT2|=15<<24;			//Seleccion MCGIRCLK tanto para UART0 como para TPM
}

void UART_init(){
	SIM_SCGC5|=(1<<9);//Puerto A
	PORTA_PCR1|=(2<<8);//UART0 Rx
	PORTA_PCR2|=(2<<8);//UART0 Tx
	
	SIM_SCGC4|=(1<<10);//clk interfaz UART0UART0_BDH=0;
	UART0_BDH=0;
	UART0_BDL=26;//ecuación para definir baud rate
	UART0_C2=12+(1<<7);// TE=RE=1, hab intr THRE(TDRE)
}


void send_inf(unsigned long centesimas)
{
	int digit_size = getDigitsize(centesimas);
	unsigned char msg[digit_size];
	
	loadInfo(msg,digit_size,centesimas);
	
	unsigned char i = 0;
				
	while (i < digit_size)
	{
		while ((UART0_S1&(1<<7)) == 0 );
		UART0_D = msg[i++];
	}
		
}

void loadInfo(unsigned char msg[], int digit_size, int centesimas)
{
	int ind;
	for(ind=1; (ind <= digit_size); ind++)
	{
		msg[digit_size-ind] = (centesimas % 10)+ 48;
		centesimas /= 10;  
	}
	
}

int getDigitsize(int centesimas)
{
	
	int digits = 0;
	int divider = 1;
	if(centesimas == 0)
	{
		return 1;
	}
	while(centesimas/divider !=0)
	{
		digits++;
		divider *=10;
		
	}
	return digits;
}

void salto_linea_corredera(){
	do{}while((UART0_S1&(1<<7))==0); //esperar para mandar siguiente dato
		UART0_D= '\n';//mandar info
	do{}while((UART0_S1&(1<<7))==0); //esperar para mandar siguiente dato
		UART0_D= '\r';//mandar info
}

int main(void)
{

	clk_init();
	UART_init();
	port_D_init();
	LPTMER_init();
	
	return 0;
}
