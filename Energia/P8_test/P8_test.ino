#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include <math.h>
LIS3DH accsensor( I2C_MODE, 0x18 );

double max_X=-10.0 , std_X=0.0, std_Z=0.0, min_X=0.0, min_Z =0.0;
double sumX=0.0, avgX=0.0, sum1=0.0;
//double sumZ=0.0, avgZ=0.0, sum2=0.0;

double x[10]={0.0,}, y[10]={0.0,}, z[10]={0.0,};
int i=0, j=0;
void setup() {
  Wire.setModule(0);
  
  pinMode(RED_LED, OUTPUT);   //fast_walk  
  pinMode(GREEN_LED, OUTPUT); //sitting   
  pinMode(BLUE_LED, OUTPUT);  //slow_walk
  
  Serial1.begin(9600);
  Serial1.print("P8_test  \n");
  Wire.begin();  
  accsensor.begin();
}

void loop() {
  // put your main code here, to run repeatedly: 
  if(Serial1.available())
  {
    //Get all parameters
    if(i<10)
    {
      x[i]=accsensor.readFloatAccelX();
      y[i]=accsensor.readFloatAccelY();
      z[i]=accsensor.readFloatAccelZ();
      Serial1.print(i);
      Serial1.print("    ");
      Serial1.print(x[i], 4);
      Serial1.print(", ");
      Serial1.print(y[i], 4);
      Serial1.print(", ");
      Serial1.print(z[i], 4);
      Serial1.print("\n");

      
    }    
    if(i==9)  // if i==10
    {

      //1. calculate std_X
      for (j = 0; j < 10; j++)
      {
          sumX = sumX + x[j];
      }
      avgX = sumX / 10.0;
      for (j = 0; j < 10; j++)
      {
          sum1 = sum1 + pow((x[j] - avgX), 2);
      }
      std_X = sqrt(sum1 / 10.0);
      Serial1.print(" -- std_X : ");
      Serial1.print(std_X, 4);

      // 2. calculate and min_X
      //max_X = x[0];   
      min_X = x[0]; 
      for (j = 1; j < 10; j++)
      {
        if(x[j]<min_X)
        {
          min_X = x[j];      
        }
      }
      //Serial1.print(" -- max_X : ");
      //Serial1.print(max_X, 4);
      Serial1.print(" -- min_X : ");
      Serial1.print(min_X, 4);
      
      // 3. calculate min_Z
      min_Z = z[0];     
      for (j = 1; j < 10; j++)
      {
        if(z[j]<min_Z)
        {
          min_Z = z[j];      
        }
      }
      Serial1.print(" -- min_Z : ");
      Serial1.print(min_Z, 4);
      Serial1.print("\n");





      // classification
      if(std_X<= 0.010363)//sitting
      {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);        
        digitalWrite(BLUE_LED, LOW);
      }
      else
      {
        if(min_Z<=0.4844)
        {
          if(std_X<=0.135625)//slow_walking
          {
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);        
            digitalWrite(BLUE_LED, HIGH);
          }   
          else//fast_walking
          {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);        
            digitalWrite(BLUE_LED, LOW);            
          }
        }  
        else
        {
          if(min_X > -0.9838)//slow_walking
          {
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, LOW);        
            digitalWrite(BLUE_LED, HIGH);
          }      
          else
          {
            if(std_X<=0.221639)//slow_walking
            {
              digitalWrite(RED_LED, LOW);
              digitalWrite(GREEN_LED, LOW);        
              digitalWrite(BLUE_LED, HIGH);
            }   
            else//fast_walking
            {
              digitalWrite(RED_LED, HIGH);
              digitalWrite(GREEN_LED, LOW);        
              digitalWrite(BLUE_LED, LOW);            
            }
          }
        }
      }
      

      sum1=0.0;
      sumX=0.0;
      avgX=0.0;
      std_X=0.0;
      //max_X=-10.0;
      //min_X=0.0;
      //min_Z=0.0;      
    }

    delay(100);
    i++;
    if(i==10)
      i=0;
  }
}
