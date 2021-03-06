/*
   EMRE MALTAS(Þ) Biyomedikal Muhendisligi 4. Sinif Ögrencisi  
*/


#include<16f877a.h>
#device ADC=10
#use delay(clock=4000000)
#fuses XT,NOLVP,NODEBUG,NOPROTECT,NOWDT,NOBROWNOUT,NOCPD

#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
#use fast_io(d)


int disp[10]={0XBF,0X06,0X5B,0X4F,0X66,0X6D,0XFD,0X07,0XFF,0X6F};
unsigned long int adc_degeri=0;
int sag_sol_durum=0;    // durmus=0     sag =1  sol = 2


void baslangic_ayarlari()
{
   setup_psp(PSP_DISABLED);
   setup_timer_1(T1_DISABLED);
   
   setup_adc(ADC_CLOCK_DIV_32);
   setup_adc_ports(AN0);
   
   setup_timer_2(T2_DIV_BY_16,255,1);
   set_timer2(0);
   
   setup_CCP1(CCP_PWM);
   setup_CCP2(CCP_PWM);
   
   setup_timer_0(RTCC_DIV_1 | RTCC_EXT_H_TO_L);
   set_timer0(255);
   
   
   set_tris_a(0x15);    output_a(0x00);
   set_tris_b(0x00);    output_b(0x00);
   set_tris_c(0x00);    output_c(0x00);
   set_tris_d(0x00);    output_d(0x00);
   
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
}

void saga_kayan_isik()
{
   int durum=0X0F;
     for(int i=0;i<=4;i++)
     {
          output_d(durum);
          durum=durum<<1;
          delay_ms(80);
     }
}

void sola_kayan_isik()
{
       int durum=0x0F;
       output_d(durum);   delay_ms(80);
       for(int i=0;i<=4;i++)
       {
         output_d(durum);
         durum=durum>>1;
         delay_ms(80);
       }
}


void adc_araligi()
{
    if(adc_degeri>=0 && adc_degeri<100)               { output_b(disp[0]); }
    else if(adc_degeri >=100 && adc_degeri<200)       { output_b(disp[1]); }
    else if(adc_degeri >=200 && adc_degeri<300)       { output_b(disp[2]); }
    else if(adc_degeri >=300 && adc_degeri<400)       { output_b(disp[3]); }
    else if(adc_degeri >=400 && adc_degeri<500)       { output_b(disp[4]); }
    else if(adc_degeri >=500 && adc_degeri<600)       { output_b(disp[5]); }
    else if(adc_degeri >=600 && adc_degeri<700)       { output_b(disp[6]); }
    else if(adc_degeri >=700 && adc_degeri<800)       { output_b(disp[7]); }
    else if(adc_degeri >=800 && adc_degeri<900)       { output_b(disp[8]); }
    else if(adc_degeri >=900 && adc_degeri<1023)      { output_b(disp[9]); }
 
}
void adc_oku()
{
   set_adc_channel(0);
   delay_us(20);
   adc_degeri = read_adc();
   adc_araligi();
}

unsigned int map(unsigned long int A,unsigned long int B,unsigned long int C,unsigned long int D,unsigned long int E)
{
   return D + ((E - D ) / (C / A));
}
void motor_sur()
{
      switch(sag_sol_durum)
      {    
         case 1:
         set_pwm2_duty(map(adc_degeri,0,1023,0,255));
         set_pwm1_duty(0);
         saga_kayan_isik();
         delay_ms(5);
         break;
         
         case 2:
         set_pwm2_duty(0);
         set_pwm1_duty(map(adc_degeri,0,1023,0,255));
         sola_kayan_isik();
         delay_ms(5);
         break;
            
      }
}

#INT_TIMER0
void timer0_kesmesi()
{
    set_timer0(255);
    if(sag_sol_durum == 1) {  sag_sol_durum = 2; }
    else if(sag_sol_durum == 2) {  sag_sol_durum = 1;}
    else { sag_sol_durum = 1;  }       
}

void main()
{
   baslangic_ayarlari();
   
   while(true)
   {
       
       if(input(pin_a2))
       {
            delay_ms(20);
            set_pwm1_duty(0);
            set_pwm2_duty(0);
            sag_sol_durum = 0; 
            while(input(pin_a2));
       }
       adc_oku();
       motor_sur();
       
   }
}
