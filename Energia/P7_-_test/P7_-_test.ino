
double tempsensor = 0, temp=0, t3=0;
int t1=0, t2=0, tflag=0 ;
int photosensor = 0, pflag=0, p1=0, p2=0, photo=0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial1.print("P7 - classify \n");
  pinMode(RED_LED, OUTPUT);     
  pinMode(GREEN_LED, OUTPUT);     
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

        //classification starts
        if(photosensor>2946)
        {
           digitalWrite(GREEN_LED, LOW);
           digitalWrite(RED_LED, HIGH);
        }
        else
        {
          if(tempsensor<=10.26)
          {
            if(photosensor<=1060)//non-screen
            {
              digitalWrite(RED_LED, LOW);
              digitalWrite(GREEN_LED, HIGH);
            }
            else//screen
            { 
              digitalWrite(GREEN_LED, LOW);
              digitalWrite(RED_LED, HIGH);             
            }
          }
          else
          {
            if(photosensor<=2933)//non-screen
            {
              digitalWrite(RED_LED, LOW);
              digitalWrite(GREEN_LED, HIGH);
            }
            else
            { 
               if(tempsensor>24.28)//screen
               {
                  digitalWrite(GREEN_LED, LOW);
                  digitalWrite(RED_LED, HIGH); 
               }    
               else if(tempsensor > 23.15)//non-screen
               {
                  digitalWrite(RED_LED, LOW);
                  digitalWrite(GREEN_LED, HIGH);                     
               }
               else
               {
                  if(photosensor<=2938)//non-screen
                  {
                    digitalWrite(RED_LED, LOW);
                    digitalWrite(GREEN_LED, HIGH);
                  }
                  else//screen
                  {
                    digitalWrite(GREEN_LED, LOW);
                    digitalWrite(RED_LED, HIGH);                     
                  }
               }
            }
          }
        }
        
        tempsensor=0;
        photosensor=0;
    }
   delay(100);


  }
}
