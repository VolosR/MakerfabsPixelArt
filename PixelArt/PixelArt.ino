#include "LGFX_ESP32S3_RGB_MakerfabsParallelTFTwithTouch40.h"
#include <EEPROM.h>

#define background 0x230D
#define EEPROM_SIZE 396

LGFX lcd;
static LGFX_Sprite sprite(&lcd); 


int n=18;
int w=22; 
int fL=14;

unsigned short tiles[18][18];
unsigned short  col[14]={TFT_BLACK,TFT_WHITE,TFT_SILVER,TFT_RED,TFT_GREEN,TFT_DARKGREEN,TFT_ORANGE,TFT_BLUE,TFT_SKYBLUE,TFT_YELLOW,TFT_PURPLE,TFT_BROWN,TFT_MAGENTA,TFT_PINK};
int selColor=3;

int butonsW=100;
int buttons[3]={14,128,242};
String labels[3]={"NEW","LOAD","SAVE"};

void newDrawing()
{
  for(int i=0;i<n;i++)
  for(int j=0;j<n;j++){
  if(i%2==0) if(j%2==0) tiles[j][i]=1; else tiles[j][i]=2;
  if(i%2!=0) if(j%2==0) tiles[j][i]=2; else tiles[j][i]=1;}
  draw();
}

void load()
{
 int c=0;
  for(int i=0;i<n;i++)
  for(int j=0;j<n;j++){
    tiles[j][i]=EEPROM.read(c);
    c++;
  }
 draw();
}

void save()
{
  int c=0;
  for(int i=0;i<n;i++)
  for(int j=0;j<n;j++){
    EEPROM.write(c, tiles[j][i]);
    
    c++;
  }
  EEPROM.commit();
}

void setup(void)
{
  EEPROM.begin(EEPROM_SIZE);
  lcd.init();
  lcd.fillScreen(background);
  lcd.setTextDatum(5);
  lcd.setTextColor(TFT_PURPLE,TFT_SKYBLUE);
  lcd.drawRect(fL-4,fL-4,404,404,TFT_ORANGE);
  lcd.drawRect(fL-3,fL-3,402,402,TFT_ORANGE);
  lcd.drawRect(fL-5,fL-5,406,406,TFT_ORANGE);
  lcd.drawRect(fL-6,fL-6,408,408,TFT_ORANGE);
  lcd.setBrightness(8);
  
  for(int i=0;i<14;i++)
  lcd.fillRoundRect(426,i*34+4,40,30,4,col[i]);

  
  for(int i=0;i<3;i++){
  lcd.fillRect(buttons[i],424,butonsW,42,TFT_SKYBLUE);
  lcd.drawString(labels[i],buttons[i]+(butonsW/2),446,4);
  }

  sprite.createSprite(396, 396);

  newDrawing();
 

}

void draw()
{
  for(int i=0;i<n;i++)
  for(int j=0;j<n;j++)
  sprite.fillRect(j*w,i*w,w,w,col[tiles[j][i]]);

  for(int i=0;i<n;i++){
  sprite.drawLine(0,i*w,396,i*w,0x738E);
  sprite.drawLine(i*w,0,i*w,396,0x738E);
  }


  sprite.pushSprite(fL,fL);
}

void loop(void)
{

   int32_t x, y;
   if (lcd.getTouch(&x, &y)) { 
   if(x<396+fL && y<396+fL) {
   tiles[(x-fL)/w][(y-fL)/w]=selColor;
   draw();}

   if(x>425){
   lcd.drawRoundRect(426-2,selColor*34+2,44,34,4,0x230D);  
   selColor=y/34;
   lcd.drawRoundRect(426-2,selColor*34+2,44,34,4,TFT_WHITE);
   }
   if(x>buttons[0] && x<buttons[0]+100 && y>442) newDrawing();
   if(x>buttons[1] && x<buttons[1]+100 && y>442) load();
   if(x>buttons[2] && x<buttons[2]+100 && y>442) save();
  } 



}
