#include "SparkFunLIS3DH.h"
#include "Wire.h"
LIS3DH accsensor( I2C_MODE, 0x18 );
void setup() {
  // put your setup code here, to run once:

  Wire.setModule(0);
  //pinMode(19,INPUT_PULLUP);
  //pinMode(38,INPUT_PULLUP);
  
  Serial1.begin(9600);
  Serial1.print("P8 \n");
  Wire.begin();  
  accsensor.begin();
}

void loop() {
  // put your main code here, to run repeatedly: 
  if(Serial1.available())
  {
    //Get all parameters
   /* Serial1.print("\nAccelerometer:\n");
    Serial1.print(" X1 = ");
    Serial1.println(accsensor.readFloatAccelX(), 4);
    Serial1.print(" Y1 = ");
    Serial1.println(accsensor.readFloatAccelY(), 4);
    Serial1.print(" Z1 = ");
    Serial1.println(accsensor.readFloatAccelZ(), 4);
  
    Serial1.print("\nSensorOne Bus Errors Reported:\n");
    Serial1.print(" All '1's = ");
    Serial1.println(accsensor.allOnesCounter);
    Serial1.print(" Non-success = ");
    Serial1.println(accsensor.nonSuccessCounter);
*/
    Serial1.print(accsensor.readFloatAccelX(), 4);
    Serial1.print(", ");
    Serial1.print(accsensor.readFloatAccelY(), 4);
    Serial1.print(", ");
    Serial1.print(accsensor.readFloatAccelZ(), 4);
    Serial1.print("\n");
    delay(100);

  }
}
