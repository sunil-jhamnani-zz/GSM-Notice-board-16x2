#include<reg51.h>
#include<string.h>
sbit rs=P1^0;
sbit en=P1^1;
sbit led=P1^0;
void lcd_cmd(unsigned char x1);
void lcd_data(unsigned char x2);
void delay(unsigned int time);
void lcd_string(unsigned char *str);
void send_byte(unsigned char ch);
unsigned char recv_byte();
void enter();
void signal();
void string_hypr(unsigned char *str);
unsigned char message[40];
char i,j,l,z;
int c,t,m=0,n=0;
void main()
{
 led=0;
 TMOD=0x20;
 SCON=0x50;
 TH1=0xfd;
 TR1=1;
 delay(50);
 lcd_cmd(0x38);
 lcd_cmd(0x0c);
 lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_string("Wireless");
 lcd_cmd(0xc2);
 lcd_string("Notice Board");
 string_hypr("AT");
 enter();
 delay(50);
 string_hypr("ATE0");
 enter();
 delay(50);
 string_hypr("AT+CMGF=1");
 enter();
 delay(50);
 
 while(1)
 {
  string_hypr("AT+CMGD=1");
  enter();
  delay(50);
  i=0;j=0;l=0;
  do
  {
   i=recv_byte();
  }while(i!='+');
  delay(500);
  signal();
  string_hypr("AT+CMGR=1");
  enter();
  delay(2);
  do
  {
   i=recv_byte();
  }while(i!='*');
  
  do
  {
   j=recv_byte();
   message[l++]=j;
  }while(j!='#');
  message[--l]='\0';
  z=strlen(message);
  lcd_cmd(0x01);
                     if(z>16)
						{
						lcd_cmd(0x80);
						delay(10);

						for(i=0;i<16;i++)
						{
							
							lcd_data(message[i]);
							
							}
						lcd_cmd(0xc0);
						delay(10);

						for(i=16;i<z;i++)
						    {
							lcd_data(message[i]);
							delay(10);
							}
						}
						
						else{
						 lcd_cmd(0x80);
						 delay(10);

						for(i=0;i<z;i++)
						{
							
							lcd_data(message[i]);
							delay(10);
							}

						  }
  delay(30);
 }
}
void send_byte(unsigned char ch)
{
   
   SBUF = ch;
   while(!TI);
   TI = 0;
 
}
unsigned char recv_byte()
{
   unsigned char c;
   while(!RI);                 
   c = SBUF;
   RI = 0;
   return(c);
}
void string_hypr(unsigned char *str)
{
   while(*str != '\0')
   {
      send_byte(*str);
	  str++;
   }
}
void enter()
{
   send_byte(0x0d);
   send_byte(0x0a);
}

void signal()
{
  led=1;
  delay(20);
  led=0;
  delay(20);
  led=1;
  delay(20);
  led=0;
  delay(20);
  led=1;
  delay(20);
  led=0;
  delay(20);
}

void lcd_cmd(unsigned char x1)
{
 P0=x1;
 rs=0;
 en=1;
 en=0;
 delay(1);
}
void lcd_data(unsigned char x2)
{
 P0=x2;
 rs=1;
 en=1;
 en=0;
 delay(1);
}
void lcd_string(unsigned char *str)
{
 	int i,j,k;
	j=strlen(str);
	lcd_cmd(0x80);
	delay(100);
	lcd_cmd(0x01);
	delay(100);
	if(strlen(str)<16)
	{
		for(i=0;str[i]!='\0';i++)
		{
			lcd_data(str[i]);
			delay(100);
		}
	}
	else
	{
		for(i=0;i<16;i++)
		{
			lcd_data(str[i]);
			delay(100);
		}
		for(i=1,j=16;str[j]!='\0';i++,j++)
		{
		 lcd_cmd(0x80);
		 lcd_cmd(0x01);
		 for(k=i;k<=j;k++)
		 {
		 	lcd_data(str[k]);
			delay(100);
		 }
		 delay(1300);
		}
	}
}
void delay(unsigned int time)
{ 
 int x,y;
 for(x=0;x<time;x++)
 for(y=0;y<1275;y++);
}