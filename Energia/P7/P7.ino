
double tempsensor = 0, temp=0, t3=0;
int t1=0, t2=0, tflag=0 ;
int photosensor = 0, pflag=0, p1=0, p2=0, photo=0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial1.print("P7 \n");

}

void loop() {
  // put your main code here, to run repeatedly: 
  if(Serial1.available())
  {
    //temperature sensor
    //sample if the value is not same to previous value
    t1 = analogRead(A0);
    if(t1!=t2)
    {
      t2 = t1;
      tempsensor = (t1*3.3/4096 -0.5)*100;  
    }
    

   //photocell sensor
    //sample if the value is not same to previous value
    p1 = analogRead(A1);
    if(p1!=p2)
    {
      p2 = p1;
      photosensor = p1; 
    }



    

    if(tempsensor!=0 && photosensor!=0)
    {
        Serial1.print(tempsensor);
        Serial1.print(", ");
        Serial1.print(photosensor);
        Serial1.print("\n");
        
        tempsensor=0;
        photosensor=0;
    }
   delay(100);


  }
}
