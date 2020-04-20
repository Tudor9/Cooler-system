int acdl, acdh, adc, T1, ms=0;
 float Vin, tmp;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
  Serial.begin(9600);
  DDRD = 0b00001000;
  DDRB = 0b00010011;
  DDRC = 0b00000000;
  PORTC = 0b00000001;
  init_timer();
  init_ADC();
  init_PWM();
 
}
void loop()
{

}

void init_PWM(){
  TCCR2A=0b10100011;// fast pwm mode
  TCCR2B=0b00000101;// prescalar
  }

void init_ADC(){
  ADMUX = 0b00000000;
  ADCSRA= 0b10000111;
   ADCSRA |= (1<<3);
   SREG |= (1<<7);
  }
  
int readADC (){
  
  ADMUX &=0b11100000;
  ADCSRA |=(1<<6);
  
  }

ISR(ADC_vect){
  acdl = ADCL;
  acdh = ADCH;
  adc = (acdh << 8) | acdl;
  termistor();
}

ISR(TIMER0_COMPA_vect)
{
  PORTB |= 1<<0;
if(ms>500)
 {ms=0;
 readADC();
 if(Tc > 30){
     PORTB = 1<<4;
     OCR2B = 255;  
 }
  else 
  {
    PORTB &= ~(1<<4);
    OCR2B = 0;  
 }
 }
  else ms++;
  
  
}

void init_timer(){
  TCCR0A = 0b00000010;
  TCCR0B = 0b00000011;
  TCNT0 = 0;
  OCR0A = 249;
  TIMSK0 |= 0b00000010;
  
  }
  
void termistor(){
  Vo = adc;
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Serial.println(Tc);
  }
