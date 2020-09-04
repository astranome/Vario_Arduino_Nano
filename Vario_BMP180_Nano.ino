#include <Wire.h> 
#include <Adafruit_BMP085.h>
#include <LiquidCrystal_I2C.h>
uint8_t bukva1[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100
};
uint8_t bukva2[8] = {
  B11100,
  B10100,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000
};
LiquidCrystal_I2C lcd(0x27, 16, 2); // Для экрана 16х2 (двухстрочный)

Adafruit_BMP085 dps;   
long vario=0,vario2=0;
long  Pressure = 0;
float Temperature = 0, Altitude = 0;
const int n=20;
const byte buzzPin=3;
//int delta=0;
//const byte ledPin = 13;
unsigned long time1=0,time2=0;
int ibuf;
int t1=0,t2=0;
unsigned long tim[20],al[20];
long QFE=0; 
void setup(void) { 
//  Serial.begin(115200); 
  Wire.begin(); 
  delay(1000);
  
  if (!dps.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
 while (1) {}
  } 

 lcd.init();  
  lcd.backlight();

lcd.noCursor();
lcd.createChar(0,bukva1);
lcd.createChar(1,bukva2);

QFE=dps.readPressure(); // Set ongroung pressure and now Altitude=0 
}          


void loop() { 
  int i;
  float  dg;
 
  if (((millis() - time1)/1000.0) >= 5.0) {     
    // dps.calcTrueTemperature();
     Temperature = dps.readTemperature();
     time1 = millis();   
  }

  t1++;
  t2++;
//  t3++;
//  t4++;
  if (t1 >= 10) {t1=0;noTone(buzzPin);}
  if (t2 >= 6) {t2=0;noTone(buzzPin);}
 
  //if (vario < 0) noTone(buzzPin);
  if (vario > -5 && vario < -2) tone(buzzPin, 350); //Sink :-(
  if (vario <= -5 ) tone(buzzPin, 250);             // Falling ;-((
  //if (vario <= -5 ) tone(buzzPin, 300);
  if (vario < 5 && vario > 1 && t1 == 5) { tone(buzzPin, 1444); } //Climbing !! 
  if (vario >= 5  && t2 ==3 ) { tone(buzzPin, 1888);}     // Termic !!!!
      
  Altitude = (dps.readAltitude(QFE)*100); 
 
//Altitude+=10;
ibuf++;
if (ibuf > n-1) ibuf=0;
al[ibuf]=Altitude;
tim[ibuf]=millis(); //ibuf*delta;
/*
// test
for (i=0;i<n;i++){
al[i]=i;
tim[i]=i;
ibuf=0;
}
*/

long  xs,ys,xys,xxs;

xs=0;
ys=0;
xys=0;
xxs=0;

for ( i=0;i < n;i++){
xs+=tim[i]-tim[0];
ys+=al[i]-al[0];
xys+=(tim[i]-tim[0])*(al[i]-al[0]);
xxs+=(tim[i]-tim[0])*(tim[i]-tim[0]);
}

xs=xs;
ys=ys;
xys=xys;
xxs=xxs;


//float  tmp=0.0;
dg=(xs*xs/n - xxs);

//tmp= (xs*ys - xys*n)/dg*10; 
/*
if (abs(dg) > 0.0001) 
   vario = (xs*ys - xys*n)/dg*10; 
else
   vario =99.9;
*/

vario=(xs*ys/n-xys)/dg*100;//(long)tmp;
//vario=10;
vario2=(vario+0.9*vario2);
  if (((millis() - time2)/1000.0) >= 1.0) {
    
  
  lcd.clear();
 
  lcd.setCursor(0,0);
  if (vario >= 0) 
     lcd.write(0);
  else
     lcd.print("\-");
  lcd.setCursor(2,0);
  lcd.print( abs(vario*0.1),1 );
  lcd.setCursor(8,0);
  
  if (vario2 >= 0) 
     lcd.write(0);
  else
     lcd.print("\-");  
  lcd.setCursor(10,0);
  lcd.print(abs(vario2)*0.01,1);
  lcd.print("m/s");
 
  lcd.setCursor(0,1);
  lcd.print( "A=");
  lcd.print( int(Altitude*0.01));
  lcd.print("m");
  lcd.setCursor(8,1);
  lcd.print("T=");
  
  lcd.print(Temperature); // *0.1,1);
  lcd.write(1);
  lcd.print("C");
  time2=millis();
  }
/*
  Serial.print(" U Alt(cm): "); 
  Serial.print(Altitude); 
  //Serial.print("  P(hPa): "); 
  //Serial.print(Pressure*0.01); 

  Serial.print(" V: ");
  Serial.print(vario*0.1);
   Serial.print(" V2: ");
  Serial.print(vario2*0.1); 
  Serial.print(" T: ");   
  Serial.println(Temperature*0.1); 
*/
  delay(100); 
}

