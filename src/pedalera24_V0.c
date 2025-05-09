#include <pedalera24_V0.h>

//CALIBRAR APPS// (SI ALGÚN VALOR SE PASA, SUBIR EL "XXUP")
#define A1UP 225
#define A1DOWN 9

#define A2UP 200
#define A2DOWN 0

#define A3UP 155
#define A3DOWN 40

//CALIBRAR BRK//

#define BRKUP 95
#define BRKDOWN 23

//CALIBRAR REGEN//

#define REGUP 255
#define REGDOWN 5


#define CAN_USE_EXTENDED_ID FALSE
#define CAN_DO_DEBUG FALSE
#define CAN_BRG_SEG_2_PHASE_TS TRUE  

//parametros CAN
#define CAN_BRG_PRESCALAR 0 
#define CAN_BRG_SYNCH_JUMP_WIDTH 2 
#define CAN_BRG_PROPAGATION_TIME 0 
#define CAN_BRG_PHASE_SEGMENT_1 4 
#define CAN_BRG_PHASE_SEGMENT_2 2 

#define CAN_BRG_WAKE_FILTER TRUE 
#define CAN_BRG_SAM FALSE 

#include <can-18F4580.c>


#define CAN_ID_PEDAL_BOARD  0x01 // board CAN ID
#define CAN_ID_PEDAL_CALIB  0x02 // board CAN ID
#define CAN_ID_CONDICION  0x03    // ID que debe recibirse antes de enviar por can la ID 2
#define CAN_PRIORITY 0x11 // max priority
#define CAN_BUFFER_LENGTH 8 // set buffer to be sent by CAN

#define VENTANA 5   //TamaÃ±o de la ventana de la media movil
#define MAX_IGNORE_VALUE 250

const int16 TIME_INTERVAL_1MS = 65459;
unsigned int8 avgA1 = 0;
unsigned int8 avgA2 = 0;
unsigned int8 avgA3 = 0;
unsigned int8 avgBrk = 0;
unsigned int8 avgSens = 0;
int1 received_id3 = FALSE;  // Variable para saber si se recibió un mensaje de ID 3

//otra prueba

// Each 1ms interrupt
/*#int_RTCC
void RTCC_isr(void) {
    counter++;
    
    if (counter >= 5) {
        // each 5 ms
        counter    = 0;
        readAdc    = 1;
    }
}*/

int8 readChanel(int ch) {
    set_adc_channel(ch);   // enable adc
    delay_us(20);          // wait time
    return read_adc();    // read ADC value (0-255)
}

int8 factorConversion(unsigned int16 MAX, unsigned int16 min, unsigned int16 data){
   if (data<=min) {return 0;}
   else if (data>=MAX) {return 255;}
   else {return ((data - min)*255)/(MAX - min);}
}
   
unsigned int8 media_movil (unsigned int16 data){
 
   unsigned int8 filtered_data=0, i;
   unsigned int16 BRK_valores [VENTANA] = {0};
   for(i=VENTANA-1;i>0;i--){
      BRK_valores[i]=BRK_valores[i-1];      //Empuja hacia la derecha en el array
      filtered_data += BRK_valores[i];      //Suma el anterior dato para la media
   }
   BRK_valores [0] = data;                  //Empuja el nuevo dato en el array
   filtered_data+=BRK_valores [0];            //Suma el nuevo dato para la media
   filtered_data/=VENTANA;
   
   return filtered_data;
}

// Función de interrupción para recibir mensajes CAN
#int_canrx0
void canrx0_int(void) {
   
    disable_interrupts(GLOBAL);
    disable_interrupts(int_canrx0);
    volatile INT32 id;
    volatile UNSIGNED int8 buffer[8]={0};
    int8 len=8;
    struct rx_stat rxstat;
    
    if (can_getd(id, buffer, len, rxstat)) {
        if (id == 0x3) { // Si el mensaje recibido es de ID 3
            received_id3 = 1;
        }
    }
    received_id3 = 1;
    if (received_id3==1) {
        unsigned int8 bufferCalib[8] = {0};
        bufferCalib[0] = 1;
        bufferCalib[1] = 1;
        bufferCalib[2] = 1;
        bufferCalib[3] = 1;
        bufferCalib[4] = 1;
        can_putd(CAN_ID_PEDAL_CALIB, bufferCalib, CAN_BUFFER_LENGTH, CAN_PRIORITY, false, false);
        received_id3 = 0; // Reseteamos la variable después de enviar el mensaje
    }
    enable_interrupts(GLOBAL);
    enable_interrupts(int_canrx0);
    
}




   

void main()
{
    unsigned int8 buffer[8] = {0};
    unsigned int16 prevA1 = 0, prevA2 = 0, prevA3 = 0, prevBrk = 0, prevSens = 0;
    unsigned int16 A1 = 0, A2 = 0, A3 = 0, Brk = 0, Sens = 0;
    unsigned int8 avgA1Calib = 0, avgA2Calib = 0, avgA3Calib = 0,avgBrkCalib = 0, avgSensCalib = 0, BRK_filtered = 0;
    

    set_tris_a(0b00111111); // set AN0 to AN4 as input (APP1,APP2,BRK,APP3 and Simu)
    set_tris_b(0b00001000); // set CANRX as input and CANTX as output
    set_tris_c(0b10000000); // set RC5 as output and RC7 as input
    setup_adc(ADC_CLOCK_INTERNAL);
    setup_spi(SPI_SS_DISABLED);
    //setup_wdt(WDT_OFF);
    
    can_init();
    // Do not listen to any ID
    can_set_mode(CAN_OP_CONFIG);
    can_set_id((unsigned int *)RX0MASK, 0x7FF, 0); // Máscara 0 configura todos los bits como relevantes
      
    // Configuración de Filtros para el Buffer 0
    can_set_id((unsigned int *)RX0FILTER0, 0x03, 0); // Filtro 0 del buffer 0 para la ID 311
      

      
    // Configuración de Filtro para el Buffer 1

    can_set_mode(CAN_OP_NORMAL);
    
    enable_interrupts(GLOBAL);
    enable_interrupts(int_canrx0); // Habilita la interrupción de recepción CAN
    enable_interrupts(INT_TIMER0); 

    SET_TIMER0(TIME_INTERVAL_1MS); //
   //asdasdasdasdasdasdads
    delay_ms(1300);//un poquillo de delay
    
    while(TRUE)
    {

         volatile INT32 id;
         volatile UNSIGNED int8 buffer[8]={0};
         int8 len=8;
         struct rx_stat rxstat;
         if (can_getd(id, buffer, len, rxstat)) {
               buffer[0] = 1;
               buffer[1] = 1;
               buffer[2] = 1;
               buffer[3] = 1;
               buffer[4] = 1;
               buffer[5] = 1;
               can_putd(CAN_ID_PEDAL_BOARD,buffer,CAN_BUFFER_LENGTH,CAN_PRIORITY,false,false);
         }
         //buffer[0] = 1;
         //buffer[1] = 1;
         //buffer[2] = 1;
         //buffer[3] = 1;
         //buffer[4] = 1;
         //buffer[5] = 1;
         //can_putd(CAN_ID_PEDAL_BOARD,buffer,CAN_BUFFER_LENGTH,CAN_PRIORITY,false,false);
         output_toggle(PIN_C4);
         
         // Si se ha recibido el mensaje con ID 3 antes, enviamos el mensaje con ID 5
         
         delay_ms(10);
   }

}
