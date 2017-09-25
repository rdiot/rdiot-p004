/* Air Quality Sensor Measurement RGB Matrix Output [P004] : http://rdiot.tistory.com/327 [RDIoT Demo] */

// NOTE THIS CAN ONLY BE USED ON A MEGA! NOT ENOUGH RAM ON UNO!
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <MQ135.h>

#include <Wire.h> //온도센서
#include <SI7021.h> //온도센서
int pin = A5; // 공기질센서
int cds = A6; // CDS

// LED Matrix 
#define OE   9
#define LAT 10
#define CLK 11
#define A   A0
#define B   A1
#define C   A2
#define D   A3
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

MQ135 gasSensor = MQ135(pin);

//#define RZERO 541.25 // 76.63
#define RZERO 76.63

double rzero;
double result;

int i=0;

/* 온도센서 SI7021 */
SI7021 sensor;

void setup() {

  matrix.begin(); // LED Matrix 초기화
  sensor.begin(); // Temperature SI7021   

  // X,Y좌표에 1개 포인트 그리기 - 흰색 (7,7,7)
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));
  delay(500);

  /* 사각형 채우기 */
  // X,Y좌표부터 64x32만큼 사각형 채워서 그리기 - Yellow
  matrix.fillRect(0, 0, 64, 32, matrix.Color333(0, 7, 0));

  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.println("LED MATRIX!");
  
  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(7,4,0)); 
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0)); 
  matrix.print('6');
  matrix.setTextColor(matrix.Color333(0,7,0));  
  matrix.print('4');
  matrix.setCursor(34, 24);  
  matrix.setTextColor(matrix.Color333(0,7,7)); 
  matrix.print("*");
  matrix.setTextColor(matrix.Color333(0,4,7)); 
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7)); 
  matrix.print("B");
  matrix.setTextColor(matrix.Color333(7,0,4)); 
  matrix.println("*");

  delay(3000); 
}


void loop() {
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  if(analogRead(cds) < 750)
  {
  // temperature is an integer in hundredths
  float temperature = sensor.getCelsiusHundredths();
  temperature = temperature / 100.0;
  
  matrix.setTextColor(matrix.Color333(4,0,7)); 
  matrix.setCursor(1, 1);    
  matrix.println("T:"+(String)temperature+"'C ");

  int humidity = sensor.getHumidityPercent();
  matrix.setTextColor(matrix.Color333(7,0,4));
  matrix.setCursor(1, 9);  
  matrix.println("H:"+(String)humidity+"% ");

  if (i==0) {
   rzero = gasSensor.getRZero(); // float
  }
  if (i>0) {  
   result = gasSensor.getRZero();
   rzero = (rzero + result)/2;
  }

  float co2_ppm = gasSensor.getPPM();

  matrix.setCursor(1, 17);    // start at top left, with 8 pixel of spacing
  matrix.println("A:"+(String)co2_ppm+"ppm");

  i++;

  if(co2_ppm > 20)
  {
    matrix.drawRect(0, 0, 64, 32, matrix.Color333(0, 4, 7));
  }
  else
  {
    matrix.drawRect(0, 0, 64, 32, matrix.Color333(0, 0, 7));
  }

  sensor.setHeater(true);
  delay(2000);
  sensor.setHeater(false);

  //cool down
  delay(2000);
  }
  else
  {
      delay(10000);
  }
}
