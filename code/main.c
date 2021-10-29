#include <REGX51.H>

#define LCD_RS  P2_0	
#define LCD_EN  P2_1	 
#define LCD_D4  P2_4	 
#define LCD_D5  P2_5
#define LCD_D6  P2_6
#define LCD_D7  P2_7
#define SDA  P1_0
#define SCL  P1_1
#define menu  P3_7
#define up  P3_6
#define down  P3_5
#define hengio  P3_4
#define Coi_chip  P1_2
#define tb1  P1_3
#define tb2  P1_4
#define tb3  P1_5
#define tb4  P1_6

char giay,phut,gio,ngay,thang,nam,x,gbt,pbt,kt,bbt,gtb1,ptb1,gtb2,ptb2,gtb3,ptb3,giaytb1,giaytb2,giaytb3,hg1,hg2,hg3,gtb4,ptb4,giaytb4,hg4,cd,ctb,ogtb4,optb4,ogiaytb4,ohg4,ogtb3,optb3,ogiaytb3,ohg3,ogtb2,optb2,ogiaytb2,ohg2,ogtb1,optb1,ogiaytb1,ohg1;
//////////////////////////// Cac Ham thiet lap LCD///////////////////////////////////
void delay_us(unsigned int t)  // Ham tre micro giay
{
	unsigned int i;
	for(i=0;i<t;i++);
}
void bip()
{
	unsigned char n;
    for(n=0;n<100;n++)
    {
		Coi_chip=0; delay_us(50);
        Coi_chip=1; delay_us(50); // 100us = 10khz
    }    
}
void delay_ms(unsigned int t)	// Ham tre mini giay
{
	unsigned int i,j;
	for(i=0;i<t;i++)
		for(j=0;j<125;j++);
}
// Ham cho phep hien thi len LCD
void LCD_Enable(void)
{
	LCD_EN = 1;
	delay_us(3);
	LCD_EN = 0;
	delay_us(50); 
}
// Ham gui 4 bit
void LCD_Send4Bit(unsigned char Data)
{
	LCD_D4 = Data & 0x01;
	LCD_D5 = (Data>>1)&1;
	LCD_D6 = (Data>>2)&1;
	LCD_D7 = (Data>>3)&1;
}
// Ham gui 1 Byte du lieu
void LCD_Send1Byte(unsigned char byte)
{
	LCD_Send4Bit(byte >>4);/* Gui 4 bit cao */
	LCD_Enable();
	LCD_Send4Bit(byte); /* Gui 4 bit thap*/
	LCD_Enable();
}
// Ham di chuyen con tro: row=0-1; col=0-15 (2 hang + 16 cot)
void LCD_chonvitri(unsigned char row, unsigned char col)
{
	unsigned char address;   
	if (row == 0) address = (0x80 + col);	 // row=0 - hang 1
    else address = (0xC0 + col);			 // row=1 - hang 2
    delay_us(1000);
    LCD_Send1Byte(address);
    delay_us(50);
}
// Ham hien thi ra man hinh chuoi ki tu
void LCD_guichuoi(char *s)
{
	while (*s)
	{
		LCD_RS=1;
		LCD_Send1Byte(*s);
		LCD_RS=0;
		s++;
	}
}
void LCD_guikitu(int s)
{
		LCD_RS=1;
		LCD_Send1Byte(s);
		LCD_RS=0;	
	
}
// Ham xoa man hinh
void LCD_xoamanhinh()
{
    LCD_chonvitri(0,0);
	LCD_guichuoi("                ");
	LCD_chonvitri(1,0);
	LCD_guichuoi("                ");
	LCD_chonvitri(0,0);
}
// Ham khoi tao LCD
void LCD_khoitao()
{
	LCD_Send4Bit(0x00);
	delay_ms(20);
	LCD_RS = 0;
	LCD_Send4Bit(0x03);	 // 
	LCD_Enable();
	delay_ms(5);
	LCD_Enable();
	delay_ms(100);
	LCD_Enable();
	LCD_Send4Bit(0x02);	// dua con tro ve dau man hinh
	LCD_Enable();
	LCD_Send1Byte(0x28);
	LCD_Send1Byte(0x0C); // Bat hien thi, bat con tro	 0x0C neu muon tat con tro
	LCD_Send1Byte(0x06);
	LCD_xoamanhinh();
	delay_ms(20);
}
void I2C_Start(void)
{
	SDA = 1;
	SCL = 1;
	SDA = 0;
	delay_ms(1);
	SCL = 0;
	SDA = 1;
}
void I2C_STOP()
{
  SDA=0;
  SCL=1;
  SDA=1;
}
bit I2C_Write(unsigned char dat)
{
	unsigned char i;
	bit outbit;
	
	for (i = 1; i <= 8; i++)
	{
		outbit=dat&0x80;
		SDA = outbit;
		dat = dat << 1;
		SCL = 1;
		SCL = 0;
    }	
	SDA = 1;	
	SCL = 1;	
	outbit = SDA;
	SCL = 0;
	return(outbit);
}
unsigned char I2C_Read(bit ack)
{
	unsigned char i, dat;
	bit inbit;	
	dat = 0;
	for(i=1;i<=8;i++)
	 {
		SCL = 1;	
		inbit = SDA;	
		dat = dat << 1;
		dat = dat | inbit;
		SCL = 0;
	 }
	if (ack) SDA = 0;
	else SDA = 1;
	SCL = 1;	
	SCL = 0;	
	SDA = 1;	
	delay_ms(1);
	return(dat);
}
void DSl3O7_Write(unsigned char add, unsigned char dat)
{
	I2C_Start();	
	I2C_Write(0xd0);
	I2C_Write(add);
	I2C_Write(((dat/10)<<4)|(dat%10)); 
	I2C_STOP();
}
unsigned char DSl3O7_Read(unsigned char add)
{
	unsigned char dat;
	
	I2C_Start();
	
	I2C_Write(0xd0);
	I2C_Write(add); 
	I2C_Start(); 
	I2C_Write(0xd1);
	dat = I2C_Read(0);	
	I2C_STOP();	
	dat = (dat & 0x0f) + (dat>>4)*10;
	return (dat);
}
void hienthi()
{
  //lay gia tri
	    giay=DSl3O7_Read(0x00);
		phut=DSl3O7_Read(0x01);
		gio=DSl3O7_Read(0x02);
		ngay=DSl3O7_Read(0x04);
		thang=DSl3O7_Read(0x05);
		nam=DSl3O7_Read(0x06);
		//Hien thi thoi gian
	    LCD_chonvitri(0,7);
		LCD_guikitu(gio/10+0x30);
		LCD_guikitu(gio%10+0x30);
		LCD_chonvitri(0,9);
		LCD_guikitu(':');
		LCD_guikitu(phut/10+0x30);
		LCD_guikitu(phut%10+0x30);
		LCD_chonvitri(0,12);
		LCD_guikitu(':');
		LCD_guikitu(giay/10+0x30);
		LCD_guikitu(giay%10+0x30);
		//Hien thi ngay thang
		LCD_chonvitri(1,7);
		LCD_guikitu(ngay/10+0x30);
		LCD_guikitu(ngay%10+0x30);
		LCD_chonvitri(1,9);
		LCD_guikitu(':');
		LCD_guikitu(thang/10+0x30);
		LCD_guikitu(thang%10+0x30);
		LCD_chonvitri(1,12);
		LCD_guikitu(':');
		LCD_guikitu(nam/10+0x30);
		LCD_guikitu(nam%10+0x30);		
}
void key()
{
  if(menu==0)
  {
   delay_ms(20);
   while(menu==0);
    LCD_xoamanhinh();
	LCD_chonvitri(0,4);
	LCD_guichuoi("SET GIO");
	LCD_chonvitri(1,7);
    LCD_guikitu(gio/10+0x30);
	LCD_guikitu(gio%10+0x30);
   while(1)
   {
	   if(up==0)
	    {
		   delay_ms(20);
		   while(up==0);
		   gio++;if(gio>23){gio=0;}		    
		   LCD_chonvitri(1,7);
		   LCD_guikitu(gio/10+0x30);
		   LCD_guikitu(gio%10+0x30);
		}
		if(down==0)
	    {
		   delay_ms(20);
		   while(down==0);
		   gio--;if(gio<0){gio=23;}		    
		   LCD_chonvitri(1,7);
		   LCD_guikitu(gio/10+0x30);
		   LCD_guikitu(gio%10+0x30);
		}
	 	if(menu==0)
		  {
		   delay_ms(20);
		   while(menu==0);
		   DSl3O7_Write(0x02,gio);				   // luu gio
		   x=1;
	      }
		if(x==1)
		  {
		  
		   LCD_xoamanhinh();
			LCD_chonvitri(0,4);
			LCD_guichuoi("SET PHUT");
			LCD_chonvitri(1,7);
		    LCD_guikitu(phut/10+0x30);
			LCD_guikitu(phut%10+0x30);
		   while(1)
		   {
			   if(up==0)
			    {
				   delay_ms(20);
				   while(up==0);
				   phut++;if(phut>59){phut=0;}		    
				   LCD_chonvitri(1,7);
				   LCD_guikitu(phut/10+0x30);
				   LCD_guikitu(phut%10+0x30);
				}
				if(down==0)
			    {
				   delay_ms(20);
				   while(down==0);
				   phut--;if(phut<0){phut=59;}		    
				   LCD_chonvitri(1,7);
				   LCD_guikitu(phut/10+0x30);
				   LCD_guikitu(phut%10+0x30);
				}
			   if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);
				   DSl3O7_Write(0x01,phut);
				   x=2;
				   break;
			      }
		   }
		  }
		  if(x==2)
		  {
		  
		   LCD_xoamanhinh();
			LCD_chonvitri(0,4);
			LCD_guichuoi("SET GIAY");
			LCD_chonvitri(1,7);
		    LCD_guikitu(giay/10+0x30);
			LCD_guikitu(giay%10+0x30);
		   while(1)
		   {
			   if(up==0)
			    {
				   delay_ms(20);
				   while(up==0);
				   giay++;if(giay>59){giay=0;}		    
				   LCD_chonvitri(1,7);
				   LCD_guikitu(giay/10+0x30);
				   LCD_guikitu(giay%10+0x30);
				}
				if(down==0)
			    {
				   delay_ms(20);
				   while(down==0);
				   giay--;if(giay<0){giay=59;}		    				   
				   LCD_chonvitri(1,7);
				   LCD_guikitu(giay/10+0x30);
				   LCD_guikitu(giay%10+0x30);
				}
			   if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);
				   DSl3O7_Write(0x00,giay);
				   x=3;
				   break;
			      }
		   }
		  }
		if(x==3)
		  {
		  
		   LCD_xoamanhinh();
			LCD_chonvitri(0,4);
			LCD_guichuoi("SET NGAY");
			LCD_chonvitri(1,7);
		    LCD_guikitu(ngay/10+0x30);
			LCD_guikitu(ngay%10+0x30);
		   while(1)
		   {
			   if(up==0)
			    {
				   delay_ms(20);
				   while(up==0);
				   ngay++;if(ngay>31){ngay=1;}		    
				   LCD_chonvitri(1,7);
				   LCD_guikitu(ngay/10+0x30);
				   LCD_guikitu(ngay%10+0x30);
				}
				if(down==0)
			    {
				   delay_ms(20);
				   while(down==0);
				   ngay--;if(ngay<1){ngay=31;}		    				   
				   LCD_chonvitri(1,7);
				   LCD_guikitu(ngay/10+0x30);
				   LCD_guikitu(ngay%10+0x30);
				}
			   if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);
				   DSl3O7_Write(0x04,ngay);
				   x=4;
				   break;
			      }
		   }
		  }
		 if(x==4)
		  {
		  
		    LCD_xoamanhinh();
			LCD_chonvitri(0,3);
			LCD_guichuoi("SET THANG");
			LCD_chonvitri(1,7);
		    LCD_guikitu(thang/10+0x30);
			LCD_guikitu(thang%10+0x30);
		   while(1)
		   {
			   if(up==0)
			    {
				   delay_ms(20);
				   while(up==0);
				   thang++;if(thang>12){thang=1;}		    
				   LCD_chonvitri(1,7);
				   LCD_guikitu(thang/10+0x30);
				   LCD_guikitu(thang%10+0x30);
				}
				if(down==0)
			    {
				   delay_ms(20);
				   while(down==0);
				   thang--;if(thang<1){thang=12;}		    				   
				   LCD_chonvitri(1,7);
				   LCD_guikitu(thang/10+0x30);
				   LCD_guikitu(thang%10+0x30);
				}
			   if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);
				   DSl3O7_Write(0x05,thang);
				   x=5;
				   break;
			      }
		   }
		  }
		 if(x==5)
		  {
		  
		    LCD_xoamanhinh();
			LCD_chonvitri(0,4);
			LCD_guichuoi("SET NAM");
			LCD_chonvitri(1,7);
		    LCD_guikitu(nam/10+0x30);
			LCD_guikitu(nam%10+0x30);
		   while(1)
		   {
			   if(up==0)
			    {
				   delay_ms(20);
				   while(up==0);
				   nam++;if(nam>99){nam=0;}		    
				   LCD_chonvitri(1,7);
				   LCD_guikitu(nam/10+0x30);
				   LCD_guikitu(nam%10+0x30);
				}
				if(down==0)
			    {
				   delay_ms(20);
				   while(down==0);
				   nam--;if(nam>99){nam=0;}		    				   
				   LCD_chonvitri(1,7);
				   LCD_guikitu(nam/10+0x30);
				   LCD_guikitu(nam%10+0x30);
				}
			   if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);
				   DSl3O7_Write(0x06,nam);
				   x=6;
				   break;
			      }
		   }
		  }
          
		 if(x==6)
		 {
		    LCD_xoamanhinh();
			LCD_chonvitri(0,4);
			LCD_guichuoi("BAO THUC");
			LCD_chonvitri(1,0);
			LCD_guichuoi("GIO : ");
			LCD_guikitu(gbt/10+0x30);
		    LCD_guikitu(gbt%10+0x30);
			while(1)
			 {
			   if(up==0)
			    {
				   delay_ms(20);
				   while(up==0);
				   gbt++;if(gbt>23){gbt=0;}
				   LCD_chonvitri(1,6);
				   LCD_guikitu(gbt/10+0x30);
		           LCD_guikitu(gbt%10+0x30);
				}
				if(down==0)
			    {
				   delay_ms(20);
				   while(down==0);
				   gbt--;if(gbt<0){gbt=23;}
				   LCD_chonvitri(1,6);
				   LCD_guikitu(gbt/10+0x30);
		           LCD_guikitu(gbt%10+0x30);
				}
			   if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);
				   x=7;
				   break;
				  }
			 }
		 }
		 if(x==7)
		 {
			LCD_chonvitri(1,0);
			LCD_guichuoi("PHUT : "); 
			LCD_guikitu(pbt/10+0x30);
		    LCD_guikitu(pbt%10+0x30);
			while(1)
			 {
			   if(up==0)
			    {
				   delay_ms(20);
				   while(up==0);
				   pbt++;if(pbt>59){pbt=0;}
				   LCD_chonvitri(1,7);
				   LCD_guikitu(pbt/10+0x30);
		           LCD_guikitu(pbt%10+0x30);
				}
				if(down==0)
			    {
				   delay_ms(20);
				   while(down==0);
				   pbt--;if(pbt<0){pbt=59;}
				   LCD_chonvitri(1,7);
				   LCD_guikitu(pbt/10+0x30);
		           LCD_guikitu(pbt%10+0x30);
				}
			   if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);
				   x=8;
				   break;
				  }
			 }
		 }
		 if(x==8)
		 {
		  LCD_xoamanhinh();
		  LCD_chonvitri(0,2);
		  LCD_guichuoi("BAO THUC ???");
		  LCD_chonvitri(1,6);
	      LCD_guichuoi("OFF");
		  while(1)
		  {
			  if(up==0)
				  {
				   delay_ms(20);while(up==0);bbt=1;
				   LCD_chonvitri(1,6);
				   LCD_guichuoi("ON ");
				  }
			  if(down==0)
				  {
				   delay_ms(20);while(down==0);bbt=0;
				   LCD_chonvitri(1,6);
				   LCD_guichuoi("OFF");
				  }
			  if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);
				   x=9;
				   break;
				  }
		   }
		 }		 		 

		 if(x==9)
		  {
			 
			  x=0;
			  LCD_xoamanhinh();
			  LCD_guichuoi("TIME : ");
	          LCD_chonvitri(1,0);
	          LCD_guichuoi("DATE : ");
			  break;
			
		  }
   }
 }
}
void setuphengio()
{
  if(hengio==0)
  {
   delay_ms(20);
   while(hengio==0);
   LCD_xoamanhinh();
   LCD_chonvitri(0,2);
   LCD_guichuoi("Thiet bi 1");
   LCD_chonvitri(1,6);
   LCD_guichuoi("OK ?");
     while(1)
	 {
	             if(up==0)
				  {
				   delay_ms(20);while(up==0);ctb++;if(ctb>7){ctb=0;}				  
				  }
			  if(down==0)
				  {
				   delay_ms(20);while(down==0);ctb--;if(ctb<0){ctb=7;}				   
				  }
			  if(ctb==0){LCD_chonvitri(0,0);LCD_guichuoi("Thiet bi 1 ON ");}
			  if(ctb==1){LCD_chonvitri(0,0);LCD_guichuoi("Thiet bi 2 ON ");}
			  if(ctb==2){LCD_chonvitri(0,0);LCD_guichuoi("Thiet bi 3 ON ");}
			  if(ctb==3){LCD_chonvitri(0,0);LCD_guichuoi("Thiet bi 4 ON ");}
			  if(ctb==4){LCD_chonvitri(0,0);LCD_guichuoi("Thiet bi 1 OFF");}
			  if(ctb==5){LCD_chonvitri(0,0);LCD_guichuoi("Thiet bi 2 OFF");}
			  if(ctb==6){LCD_chonvitri(0,0);LCD_guichuoi("Thiet bi 3 OFF");}
			  if(ctb==7){LCD_chonvitri(0,0);LCD_guichuoi("Thiet bi 4 OFF");}
			  if(menu==0)
				  {
				   delay_ms(20);
				   while(menu==0);			
				   break;
				  }
	 }
   if(ctb==0)
   {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,2);
	   LCD_guichuoi("Thiet bi 1");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("TB1 00 : 00 : 00");
	   while(1)
			  {
				  if(up==0)
					  {
					   delay_ms(20);while(up==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						gtb1++;if(gtb1>23){gtb1=0;}
						LCD_guikitu(gtb1/10+0x30);
						LCD_guikitu(gtb1%10+0x30);
					   }
					    if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						ptb1++;if(ptb1>59){ptb1=0;}
						LCD_guikitu(ptb1/10+0x30);
						LCD_guikitu(ptb1%10+0x30);
					   }
					    if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						giaytb1++;if(giaytb1>59){giaytb1=0;}
						LCD_guikitu(giaytb1/10+0x30);
						LCD_guikitu(giaytb1%10+0x30);
					   }
					  }
				  if(down==0)
					  {
					   delay_ms(20);while(down==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						gtb1--;if(gtb1<0){gtb1=23;}
						LCD_guikitu(gtb1/10+0x30);
						LCD_guikitu(gtb1%10+0x30);
					   }
					   if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						ptb1--;if(ptb1<0){ptb1=59;}
						LCD_guikitu(ptb1/10+0x30);
						LCD_guikitu(ptb1%10+0x30);
					   }
					   if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						giaytb1--;if(giaytb1<0){giaytb1=59;}
						LCD_guikitu(giaytb1/10+0x30);
						LCD_guikitu(giaytb1%10+0x30);
					   }
					  }
				  if(menu==0)
					  {
					  	delay_ms(20);while(menu==0);
						cd++;		
					  }
				  if(cd==3)
				      {
				   		LCD_xoamanhinh();
						LCD_guichuoi("Hen Gio TB1 ???");
						LCD_chonvitri(1,6);
		                LCD_guichuoi("NO");
						while(1)
						  {
							  if(up==0)
								  {
								   delay_ms(20);while(up==0);hg1=1;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("OK");
								  }
							  if(down==0)
								  {
								   delay_ms(20);while(down==0);hg1=0;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("NO");
								  }
							  if(menu==0)
								  {
								   delay_ms(20);
								   while(menu==0);
								   cd=4;
								   break;
								  }
						   }
				      }	
				   if(cd==4)
				   {
				     cd=0;
					 LCD_xoamanhinh();
				     LCD_guichuoi("TIME : ");
		             LCD_chonvitri(1,0);
		             LCD_guichuoi("DATE : ");
					 break;
				   }
			   }
	 }
	  if(ctb==1)
     {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,2);
	   LCD_guichuoi("Thiet bi 2");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("TB2 00 : 00 : 00");
	   while(1)
			  {
				  if(up==0)
					  {
					   delay_ms(20);while(up==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						gtb2++;if(gtb2>23){gtb2=0;}
						LCD_guikitu(gtb2/10+0x30);
						LCD_guikitu(gtb2%10+0x30);
					   }
					    if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						ptb2++;if(ptb2>59){ptb2=0;}
						LCD_guikitu(ptb2/10+0x30);
						LCD_guikitu(ptb2%10+0x30);
					   }
					    if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						giaytb2++;if(giaytb2>59){giaytb2=0;}
						LCD_guikitu(giaytb2/10+0x30);
						LCD_guikitu(giaytb2%10+0x30);
					   }
					  }
				  if(down==0)
					  {
					   delay_ms(20);while(down==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						gtb2--;if(gtb2<0){gtb2=23;}
						LCD_guikitu(gtb2/10+0x30);
						LCD_guikitu(gtb2%10+0x30);
					   }
					   if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						ptb2--;if(ptb2<0){ptb2=59;}
						LCD_guikitu(ptb2/10+0x30);
						LCD_guikitu(ptb2%10+0x30);
					   }
					   if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						giaytb2--;if(giaytb2){giaytb2=59;}
						LCD_guikitu(giaytb2/10+0x30);
						LCD_guikitu(giaytb2%10+0x30);
					   }
					  }
				  if(menu==0)
					  {
					  	delay_ms(20);while(menu==0);
						cd++;		
					  }
				  if(cd==3)
				      {
				   		LCD_xoamanhinh();
						LCD_guichuoi("Hen Gio TB2 ???");
						LCD_chonvitri(1,6);
		                LCD_guichuoi("NO");
						while(1)
						  {
							  if(up==0)
								  {
								   delay_ms(20);while(up==0);hg2=1;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("OK");
								  }
							  if(down==0)
								  {
								   delay_ms(20);while(down==0);hg2=0;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("NO");
								  }
							  if(menu==0)
								  {
								   delay_ms(20);
								   while(menu==0);
								   cd=4;
								   break;
								  }
						   }
				      }	
				   if(cd==4)
				   {
				     cd=0;
					 LCD_xoamanhinh();
				     LCD_guichuoi("TIME : ");
		             LCD_chonvitri(1,0);
		             LCD_guichuoi("DATE : ");
					 break;
				   }
			   }
	 }
	  if(ctb==2)
     {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,2);
	   LCD_guichuoi("Thiet bi 3");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("TB3 00 : 00 : 00");
	   while(1)
			  {
				  if(up==0)
					  {
					   delay_ms(20);while(up==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						gtb3++;if(gtb3>23){gtb3=0;}
						LCD_guikitu(gtb3/10+0x30);
						LCD_guikitu(gtb3%10+0x30);
					   }
					    if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						ptb3++;if(ptb3>59){ptb3=0;}
						LCD_guikitu(ptb3/10+0x30);
						LCD_guikitu(ptb3%10+0x30);
					   }
					    if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						giaytb3++;if(giaytb3>59){giaytb3=0;}
						LCD_guikitu(giaytb3/10+0x30);
						LCD_guikitu(giaytb3%10+0x30);
					   }
					  }
				  if(down==0)
					  {
					   delay_ms(20);while(down==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						gtb3--;if(gtb3<0){gtb3=23;}
						LCD_guikitu(gtb3/10+0x30);
						LCD_guikitu(gtb3%10+0x30);
					   }
					   if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						ptb3--;if(ptb3<0){ptb3=59;}
						LCD_guikitu(ptb3/10+0x30);
						LCD_guikitu(ptb3%10+0x30);
					   }
					   if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						giaytb3--;if(giaytb3){giaytb3=59;}
						LCD_guikitu(giaytb3/10+0x30);
						LCD_guikitu(giaytb3%10+0x30);
					   }
					  }
				  if(menu==0)
					  {
					  	delay_ms(20);while(menu==0);
						cd++;		
					  }
				  if(cd==3)
				      {
				   		LCD_xoamanhinh();
						LCD_guichuoi("Hen Gio TB3 ???");
						LCD_chonvitri(1,6);
		                LCD_guichuoi("NO");
						while(1)
						  {
							  if(up==0)
								  {
								   delay_ms(20);while(up==0);hg3=1;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("OK");
								  }
							  if(down==0)
								  {
								   delay_ms(20);while(down==0);hg3=0;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("NO");
								  }
							  if(menu==0)
								  {
								   delay_ms(20);
								   while(menu==0);
								   cd=4;
								   break;
								  }
						   }
				      }	
				   if(cd==4)
				   {
				     cd=0;
					 LCD_xoamanhinh();
				     LCD_guichuoi("TIME : ");
		             LCD_chonvitri(1,0);
		             LCD_guichuoi("DATE : ");
					 break;
				   }
			   }
	 }
	 if(ctb==3)
     {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,2);
	   LCD_guichuoi("Thiet bi 4");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("TB4 00 : 00 : 00");
	   while(1)
			  {
				  if(up==0)
					  {
					   delay_ms(20);while(up==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						gtb4++;if(gtb4>23){gtb4=0;}
						LCD_guikitu(gtb4/10+0x30);
						LCD_guikitu(gtb4%10+0x30);
					   }
					    if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						ptb4++;if(ptb4>59){ptb4=0;}
						LCD_guikitu(ptb4/10+0x30);
						LCD_guikitu(ptb4%10+0x30);
					   }
					    if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						giaytb4++;if(giaytb4>59){giaytb4=0;}
						LCD_guikitu(giaytb4/10+0x30);
						LCD_guikitu(giaytb4%10+0x30);
					   }
					  }
				  if(down==0)
					  {
					   delay_ms(20);while(down==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						gtb4--;if(gtb4<0){gtb4=23;}
						LCD_guikitu(gtb4/10+0x30);
						LCD_guikitu(gtb4%10+0x30);
					   }
					   if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						ptb4--;if(ptb4<0){ptb4=59;}
						LCD_guikitu(ptb4/10+0x30);
						LCD_guikitu(ptb4%10+0x30);
					   }
					   if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						giaytb4--;if(giaytb4){giaytb4=59;}
						LCD_guikitu(giaytb4/10+0x30);
						LCD_guikitu(giaytb4%10+0x30);
					   }
					  }
				  if(menu==0)
					  {
					  	delay_ms(20);while(menu==0);
						cd++;		
					  }
				  if(cd==3)
				      {
				   		LCD_xoamanhinh();
						LCD_guichuoi("Hen Gio TB4 ???");
						LCD_chonvitri(1,6);
		                LCD_guichuoi("NO");
						while(1)
						  {
							  if(up==0)
								  {
								   delay_ms(20);while(up==0);hg4=1;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("OK");
								  }
							  if(down==0)
								  {
								   delay_ms(20);while(down==0);hg4=0;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("NO");
								  }
							  if(menu==0)
								  {
								   delay_ms(20);
								   while(menu==0);
								   cd=4;
								   break;
								  }
						   }
				      }
			    if(cd==4)
				   {
				     cd=0;
					 LCD_xoamanhinh();
				     LCD_guichuoi("TIME : ");
		             LCD_chonvitri(1,0);
		             LCD_guichuoi("DATE : ");
					 break;
				   }	
			   }
	 }
	 if(ctb==7)
     {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,2);
	   LCD_guichuoi("Thiet bi 4");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("OFF 00 : 00 : 00");
	   while(1)
			  {
				  if(up==0)
					  {
					   delay_ms(20);while(up==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						ogtb4++;if(ogtb4>23){ogtb4=0;}
						LCD_guikitu(ogtb4/10+0x30);
						LCD_guikitu(ogtb4%10+0x30);
					   }
					    if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						optb4++;if(optb4>59){optb4=0;}
						LCD_guikitu(optb4/10+0x30);
						LCD_guikitu(optb4%10+0x30);
					   }
					    if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						ogiaytb4++;if(ogiaytb4>59){ogiaytb4=0;}
						LCD_guikitu(ogiaytb4/10+0x30);
						LCD_guikitu(ogiaytb4%10+0x30);
					   }
					  }
				  if(down==0)
					  {
					   delay_ms(20);while(down==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						ogtb4--;if(ogtb4<0){ogtb4=23;}
						LCD_guikitu(ogtb4/10+0x30);
						LCD_guikitu(ogtb4%10+0x30);
					   }
					   if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						optb4--;if(optb4<0){optb4=59;}
						LCD_guikitu(optb4/10+0x30);
						LCD_guikitu(optb4%10+0x30);
					   }
					   if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						ogiaytb4--;if(ogiaytb4){ogiaytb4=59;}
						LCD_guikitu(ogiaytb4/10+0x30);
						LCD_guikitu(ogiaytb4%10+0x30);
					   }
					  }
				  if(menu==0)
					  {
					  	delay_ms(20);while(menu==0);
						cd++;		
					  }
				  if(cd==3)
				      {
				   		LCD_xoamanhinh();
						LCD_guichuoi("Hen Gio TB4 ???");
						LCD_chonvitri(1,6);
		                LCD_guichuoi("NO");
						while(1)
						  {
							  if(up==0)
								  {
								   delay_ms(20);while(up==0);ohg4=1;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("OK");
								  }
							  if(down==0)
								  {
								   delay_ms(20);while(down==0);ohg4=0;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("NO");
								  }
							  if(menu==0)
								  {
								   delay_ms(20);
								   while(menu==0);
								   cd=4;
								   break;
								  }
						   }
				      }	
			   if(cd==4)
			   {
			     cd=0;
				 LCD_xoamanhinh();
			     LCD_guichuoi("TIME : ");
	             LCD_chonvitri(1,0);
	             LCD_guichuoi("DATE : ");
				 break;
			   }
			  }
	  
	 }
	 if(ctb==6)
     {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,2);
	   LCD_guichuoi("Thiet bi 3");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("OFF 00 : 00 : 00");
	   while(1)
			  {
				  if(up==0)
					  {
					   delay_ms(20);while(up==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						ogtb3++;if(ogtb3>23){ogtb3=0;}
						LCD_guikitu(ogtb3/10+0x30);
						LCD_guikitu(ogtb3%10+0x30);
					   }
					    if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						optb3++;if(optb3>59){optb3=0;}
						LCD_guikitu(optb3/10+0x30);
						LCD_guikitu(optb3%10+0x30);
					   }
					    if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						ogiaytb3++;if(ogiaytb3>59){ogiaytb3=0;}
						LCD_guikitu(ogiaytb3/10+0x30);
						LCD_guikitu(ogiaytb3%10+0x30);
					   }
					  }
				  if(down==0)
					  {
					   delay_ms(20);while(down==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						ogtb3--;if(ogtb3<0){ogtb3=23;}
						LCD_guikitu(ogtb3/10+0x30);
						LCD_guikitu(ogtb3%10+0x30);
					   }
					   if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						optb3--;if(optb3<0){optb3=59;}
						LCD_guikitu(optb3/10+0x30);
						LCD_guikitu(optb3%10+0x30);
					   }
					   if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						ogiaytb3--;if(ogiaytb3){ogiaytb3=59;}
						LCD_guikitu(ogiaytb3/10+0x30);
						LCD_guikitu(ogiaytb3%10+0x30);
					   }
					  }
				  if(menu==0)
					  {
					  	delay_ms(20);while(menu==0);
						cd++;		
					  }
				  if(cd==3)
				      {
				   		LCD_xoamanhinh();
						LCD_guichuoi("Hen Gio TB3 ???");
						LCD_chonvitri(1,6);
		                LCD_guichuoi("NO");
						while(1)
						  {
							  if(up==0)
								  {
								   delay_ms(20);while(up==0);ohg3=1;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("OK");
								  }
							  if(down==0)
								  {
								   delay_ms(20);while(down==0);ohg3=0;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("NO");
								  }
							  if(menu==0)
								  {
								   delay_ms(20);
								   while(menu==0);
								   cd=4;
								   break;
								  }
						   }
				      }	
				   if(cd==4)
				   {
				     cd=0;
					 LCD_xoamanhinh();
				     LCD_guichuoi("TIME : ");
		             LCD_chonvitri(1,0);
		             LCD_guichuoi("DATE : ");
					 break;
				   }
			   }
	 }
	 if(ctb==5)
     {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,2);
	   LCD_guichuoi("Thiet bi 2");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("OFF 00 : 00 : 00");
	   while(1)
			  {
				  if(up==0)
					  {
					   delay_ms(20);while(up==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						ogtb2++;if(ogtb2>23){ogtb2=0;}
						LCD_guikitu(ogtb2/10+0x30);
						LCD_guikitu(ogtb2%10+0x30);
					   }
					    if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						optb2++;if(optb2>59){optb2=0;}
						LCD_guikitu(optb2/10+0x30);
						LCD_guikitu(optb2%10+0x30);
					   }
					    if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						ogiaytb2++;if(ogiaytb2>59){ogiaytb2=0;}
						LCD_guikitu(ogiaytb2/10+0x30);
						LCD_guikitu(ogiaytb2%10+0x30);
					   }
					  }
				  if(down==0)
					  {
					   delay_ms(20);while(down==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						ogtb2--;if(ogtb2<0){ogtb2=23;}
						LCD_guikitu(ogtb2/10+0x30);
						LCD_guikitu(ogtb2%10+0x30);
					   }
					   if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						optb2--;if(optb2<0){optb2=59;}
						LCD_guikitu(optb2/10+0x30);
						LCD_guikitu(optb2%10+0x30);
					   }
					   if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						ogiaytb2--;if(ogiaytb2){ogiaytb2=59;}
						LCD_guikitu(ogiaytb2/10+0x30);
						LCD_guikitu(ogiaytb2%10+0x30);
					   }
					  }
				  if(menu==0)
					  {
					  	delay_ms(20);while(menu==0);
						cd++;		
					  }
				  if(cd==3)
				      {
				   		LCD_xoamanhinh();
						LCD_guichuoi("Hen Gio TB2 ???");
						LCD_chonvitri(1,6);
		                LCD_guichuoi("NO");
						while(1)
						  {
							  if(up==0)
								  {
								   delay_ms(20);while(up==0);ohg2=1;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("OK");
								  }
							  if(down==0)
								  {
								   delay_ms(20);while(down==0);ohg2=0;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("NO");
								  }
							  if(menu==0)
								  {
								   delay_ms(20);
								   while(menu==0);
								   cd=4;
								   break;
								  }
						   }
				      }	
				   if(cd==4)
				   {
				     cd=0;
					 LCD_xoamanhinh();
				     LCD_guichuoi("TIME : ");
		             LCD_chonvitri(1,0);
		             LCD_guichuoi("DATE : ");
					 break;
				   }
			   }
	 }
	 if(ctb==4)
   {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,2);
	   LCD_guichuoi("Thiet bi 1");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("OFF 00 : 00 : 00");
	   while(1)
			  {
				  if(up==0)
					  {
					   delay_ms(20);while(up==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						ogtb1++;if(ogtb1>23){ogtb1=0;}
						LCD_guikitu(ogtb1/10+0x30);
						LCD_guikitu(ogtb1%10+0x30);
					   }
					    if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						optb1++;if(optb1>59){optb1=0;}
						LCD_guikitu(optb1/10+0x30);
						LCD_guikitu(optb1%10+0x30);
					   }
					    if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						ogiaytb1++;if(ogiaytb1>59){ogiaytb1=0;}
						LCD_guikitu(ogiaytb1/10+0x30);
						LCD_guikitu(ogiaytb1%10+0x30);
					   }
					  }
				  if(down==0)
					  {
					   delay_ms(20);while(down==0);
					   if(cd==0)
					   {
					   	LCD_chonvitri(1,4);
						ogtb1--;if(ogtb1<0){ogtb1=23;}
						LCD_guikitu(ogtb1/10+0x30);
						LCD_guikitu(ogtb1%10+0x30);
					   }
					   if(cd==1)
					   {
					   	LCD_chonvitri(1,9);
						optb1--;if(optb1<0){optb1=59;}
						LCD_guikitu(optb1/10+0x30);
						LCD_guikitu(optb1%10+0x30);
					   }
					   if(cd==2)
					   {
					   	LCD_chonvitri(1,14);
						ogiaytb1--;if(ogiaytb1<0){ogiaytb1=59;}
						LCD_guikitu(ogiaytb1/10+0x30);
						LCD_guikitu(ogiaytb1%10+0x30);
					   }
					  }
				  if(menu==0)
					  {
					  	delay_ms(20);while(menu==0);
						cd++;		
					  }
				  if(cd==3)
				      {
				   		LCD_xoamanhinh();
						LCD_guichuoi("Hen Gio TB1 ???");
						LCD_chonvitri(1,6);
		                LCD_guichuoi("NO");
						while(1)
						  {
							  if(up==0)
								  {
								   delay_ms(20);while(up==0);ohg1=1;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("OK");
								  }
							  if(down==0)
								  {
								   delay_ms(20);while(down==0);ohg1=0;
								   LCD_chonvitri(1,6);
								   LCD_guichuoi("NO");
								  }
							  if(menu==0)
								  {
								   delay_ms(20);
								   while(menu==0);
								   cd=4;
								   break;
								  }
						   }
				      }	
				   if(cd==4)
				   {
				     cd=0;
					 LCD_xoamanhinh();
				     LCD_guichuoi("TIME : ");
		             LCD_chonvitri(1,0);
		             LCD_guichuoi("DATE : ");
					 break;
				   }
			   }
	 }

  }
}
void baothuc()
{
char k;
if(bbt==1)
{
	if(kt==0)
	{
	 if(gio==gbt && phut==pbt)
	  {
	    LCD_xoamanhinh();
		LCD_guichuoi("Day di Sang Roi");
		for(k=0;k<70;k++)
			{
			   bip();
			   if(menu==0){delay_ms(20);while(menu==0);kt=1;break;}
			}	 
		LCD_xoamanhinh();
		LCD_guichuoi("TIME  ");
		LCD_chonvitri(1,0);
		LCD_guichuoi("DATE  ");
	
	  }									   
	}
}
if(gio!=gbt && phut!=pbt){kt=0;}
}
void baohengio()
{
 if(hg1==1)
 {
   if(gtb1==gio && ptb1==phut && giaytb1==giay)
   {
     tb1=0;
   }
 }
 if(hg2==1)
 {
   if(gtb2==gio && ptb2==phut && giaytb2==giay)
   {
     tb2=0;
   }
 }
 if(hg3==1)
 {
   if(gtb3==gio && ptb3==phut && giaytb3==giay)
   {
     tb3=0;
   }
 }
 if(hg4==1)
 {
   if(gtb4==gio && ptb4==phut && giaytb4==giay)
   {
     tb4=0;
   }
 }
// ----------------------------------------------------------------------------
 if(ohg1==1)
 {
   if(ogtb1==gio && optb1==phut && ogiaytb1==giay)
   {
     tb1=1;
   }
 }
 if(ohg2==1)
 {
   if(ogtb2==gio && optb2==phut && ogiaytb2==giay)
   {
     tb2=1;
   }
 }
 if(ohg3==1)
 {
   if(ogtb3==gio && optb3==phut && ogiaytb3==giay)
   {
     tb3=1;
   }
 }
 if(ohg4==1)
 {
   if(ogtb4==gio && optb4==phut && ogiaytb4==giay)
   {
     tb4=1;
   }
 }
 //------------------------------------------------------------------------------------
 if(up==0)
 {
   delay_ms(20);
   while(up==0);
   LCD_xoamanhinh();
   if(hg1==1)
    {
	   LCD_chonvitri(0,0);
	   LCD_guichuoi("TB1 ");
	   LCD_guikitu(gtb1/10+0x30);LCD_guikitu(gtb1%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(ptb1/10+0x30);LCD_guikitu(ptb1%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(giaytb1/10+0x30);LCD_guikitu(giaytb1%10+0x30);
	}
	else {LCD_chonvitri(0,0);LCD_guichuoi("TB1 -- -- --");}
   if(hg2==1)
    {
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("TB2 ");
	   LCD_guikitu(gtb2/10+0x30);LCD_guikitu(gtb2%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(ptb2/10+0x30);LCD_guikitu(ptb2%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(giaytb2/10+0x30);LCD_guikitu(giaytb2%10+0x30);
	}
	else {LCD_chonvitri(1,0);LCD_guichuoi("TB2 -- -- --");}
	delay_ms(3000);
	if(hg3==1)
    {
	   LCD_chonvitri(0,0);
	   LCD_guichuoi("TB3 ");
	   LCD_guikitu(gtb3/10+0x30);LCD_guikitu(gtb3%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(ptb3/10+0x30);LCD_guikitu(ptb3%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(giaytb3/10+0x30);LCD_guikitu(giaytb3%10+0x30);
	}
	else {LCD_chonvitri(0,0);LCD_guichuoi("TB3 -- -- --");}
   if(hg4==1)
    {
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("TB4 ");
	   LCD_guikitu(gtb4/10+0x30);LCD_guikitu(gtb4%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(ptb4/10+0x30);LCD_guikitu(ptb4%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(giaytb4/10+0x30);LCD_guikitu(giaytb4%10+0x30);
	}
	else {LCD_chonvitri(1,0);LCD_guichuoi("TB4 -- -- --");}
	   delay_ms(3000); 
	if(bbt==1)
	 {
	   LCD_xoamanhinh();
	   LCD_chonvitri(0,4);LCD_guichuoi("BAO THUC");
	   LCD_chonvitri(1,5);LCD_guikitu(gbt/10+0x30);LCD_guikitu(gbt%10+0x30);
	   LCD_guikitu(':');
	   LCD_guikitu(pbt/10+0x30);LCD_guikitu(pbt%10+0x30);
	   delay_ms(3000);
	 }
	else {LCD_xoamanhinh();LCD_chonvitri(0,3);LCD_guichuoi("BAO THUC");LCD_chonvitri(1,5);LCD_guichuoi("-- --");}
	   delay_ms(3000);					  
	   LCD_xoamanhinh();								 
	   LCD_guichuoi("TIME  ");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("DATE  ");

 }
 if(down==0)
 {
   delay_ms(20);
   while(down==0);
   LCD_xoamanhinh();
   LCD_guichuoi("KIEM TRA OFF TB");
   delay_ms(1500);LCD_xoamanhinh();
   if(ohg1==1)
    {
	   LCD_chonvitri(0,0);
	   LCD_guichuoi("TB1 ");
	   LCD_guikitu(ogtb1/10+0x30);LCD_guikitu(ogtb1%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(optb1/10+0x30);LCD_guikitu(optb1%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(ogiaytb1/10+0x30);LCD_guikitu(ogiaytb1%10+0x30);
	}
	else {LCD_chonvitri(0,0);LCD_guichuoi("TB1 -- -- --");}
   if(ohg2==1)
    {
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("TB2 ");
	   LCD_guikitu(ogtb2/10+0x30);LCD_guikitu(ogtb2%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(optb2/10+0x30);LCD_guikitu(optb2%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(ogiaytb2/10+0x30);LCD_guikitu(ogiaytb2%10+0x30);
	}
	else {LCD_chonvitri(1,0);LCD_guichuoi("TB2 -- -- --");}
	delay_ms(3000);
	if(ohg3==1)
    {
	   LCD_chonvitri(0,0);
	   LCD_guichuoi("TB3 ");
	   LCD_guikitu(ogtb3/10+0x30);LCD_guikitu(ogtb3%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(optb3/10+0x30);LCD_guikitu(optb3%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(ogiaytb3/10+0x30);LCD_guikitu(ogiaytb3%10+0x30);
	}
	else {LCD_chonvitri(0,0);LCD_guichuoi("TB3 -- -- --");}
   if(ohg4==1)
    {
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("TB4 ");
	   LCD_guikitu(ogtb4/10+0x30);LCD_guikitu(ogtb4%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(optb4/10+0x30);LCD_guikitu(optb4%10+0x30);LCD_guikitu(':');
	   LCD_guikitu(ogiaytb4/10+0x30);LCD_guikitu(ogiaytb4%10+0x30);
	}
	else {LCD_chonvitri(1,0);LCD_guichuoi("TB4 -- -- --");}
	   delay_ms(3000); 
	   LCD_xoamanhinh();
	   LCD_guichuoi("TIME  ");
	   LCD_chonvitri(1,0);
	   LCD_guichuoi("DATE  ");
 }	  
}
void main()
{
  
	LCD_khoitao();
	LCD_guichuoi("TIME  ");
	LCD_chonvitri(1,0);
	LCD_guichuoi("DATE  ");
	  while(1)											   
	  {								  
	    hienthi();
		key();
		baothuc();
		setuphengio();
		baohengio();
	  }
}
