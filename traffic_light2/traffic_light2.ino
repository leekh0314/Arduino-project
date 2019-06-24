


#include <Servo.h>

Servo myservo;
/******************红绿灯******************/
unsigned int  G1_LIGHT = 1;//绿灯
unsigned int  Y1_LIGHT = 2;//黄灯
unsigned int  R1_LIGHT = 3;//红灯

unsigned int  G2_LIGHT = 4;//绿灯
unsigned int  Y2_LIGHT = 5;//黄灯
unsigned int  R2_LIGHT = 6;//红灯

unsigned int  G3_LIGHT = 8;//绿灯
unsigned int  Y3_LIGHT = 9;//黄灯
unsigned int  R3_LIGHT = 10;//红灯

unsigned int  G4_LIGHT = 11;//绿灯
//unsigned int  Y4_LIGHT = 8;//黄灯人行道红绿灯
unsigned int  R4_LIGHT = 12;//红灯

/******************延时时间******************/
unsigned int  G_Time = 20;//绿灯倒计时“20”秒
unsigned int  Y_Time = 5;
unsigned int  R_Time = 10;

//unsigned int  count = 30;//计数变量初始化为 0


/******************函数声明******************/
void RYG_light ();


void setup() {
  
  Serial.begin(9600);

  pinMode(R1_LIGHT,OUTPUT);//输出模式
  pinMode(Y1_LIGHT,OUTPUT);
  pinMode(G1_LIGHT,OUTPUT); 
  
  pinMode(R2_LIGHT,OUTPUT);//输出模式
  pinMode(Y2_LIGHT,OUTPUT);
  pinMode(G2_LIGHT,OUTPUT); 

  pinMode(R3_LIGHT,OUTPUT);//输出模式
  pinMode(Y3_LIGHT,OUTPUT);
  pinMode(G3_LIGHT,OUTPUT); 

  pinMode(R4_LIGHT,OUTPUT);//输出模式人行道12
  //pinMode(Y4_LIGHT,OUTPUT);
  pinMode(G4_LIGHT,OUTPUT); //11
  
  for(int i=0;i > 0; i++)
  {
    digitalWrite(i,LOW);
  }
  
   myservo.attach(9);//Servo PIN

}

void loop() {


    RYG_light ();


}

void RYG_light ()
  {

//    R_random = random(0, 1);//随机数
//    Y_random = random(0, 1);
//    G_random = random(0, 1);
//    

//     digitalWrite((random(1,3)),HIGH);//1
//     digitalWrite((random(0, 1)),HIGH);//1
//    
     for(int count = 30 ;count > -15 && count <= 30;count--)//car time : 0-30      human time :-15 - 0
    {

          if(20 < count && count <= 30)
          {
          
          digitalWrite(G1_LIGHT,HIGH);//
          digitalWrite(Y1_LIGHT,LOW);
          digitalWrite(R1_LIGHT,LOW);

          digitalWrite(G2_LIGHT,HIGH);
          digitalWrite(Y2_LIGHT,LOW);//
          digitalWrite(R2_LIGHT,LOW);

          digitalWrite(G3_LIGHT,HIGH);
          digitalWrite(Y3_LIGHT,LOW);
          digitalWrite(R3_LIGHT,LOW);//
      
          Serial.print("Green-YELLOW-RED- LIGHT");
          Serial.println(count);
      
          delay(500);//
      }
      
       if(15 < count && count <= 20)
          {
                  
          digitalWrite(G2_LIGHT,HIGH);
          digitalWrite(Y2_LIGHT,LOW);//
          digitalWrite(R2_LIGHT,LOW);

          digitalWrite(G3_LIGHT,HIGH);
          digitalWrite(Y3_LIGHT,LOW);
          digitalWrite(R3_LIGHT,LOW);//
          
          digitalWrite(G1_LIGHT,LOW);
          digitalWrite(Y1_LIGHT,HIGH);//********11111111
          digitalWrite(R1_LIGHT,LOW);
          
          delay(500);//
          
          digitalWrite(G1_LIGHT,LOW);
          digitalWrite(Y1_LIGHT,LOW);//*********1111111111
          digitalWrite(R1_LIGHT,LOW);
      
          Serial.print("Green-YELLOW-RED- LIGHT");
          Serial.println(count);
      
          //delay(500);//
      }
      
       if(10 < count && count <= 15)
          {
          
          digitalWrite(G1_LIGHT,LOW);
          digitalWrite(Y1_LIGHT,LOW);
          digitalWrite(R1_LIGHT,HIGH); //  
                 

          digitalWrite(G3_LIGHT,HIGH);
          digitalWrite(Y3_LIGHT,LOW);
          digitalWrite(R3_LIGHT,LOW);//
          
           digitalWrite(G2_LIGHT,LOW);
          digitalWrite(Y2_LIGHT,HIGH);//*******2222222
          digitalWrite(R2_LIGHT,LOW);
          
          delay(500);//
          
          digitalWrite(G2_LIGHT,LOW);
          digitalWrite(Y2_LIGHT,LOW);//******22222222
          digitalWrite(R2_LIGHT,LOW);
      
          Serial.print("Green-YELLOW-RED- LIGHT");
          Serial.println(count);
      
          //delay(500);//
      }
      
      if(5 < count && count <= 10)
          {
          
          digitalWrite(G1_LIGHT,LOW);
          digitalWrite(Y1_LIGHT,LOW);
          digitalWrite(R1_LIGHT,HIGH); // 
         
          digitalWrite(G2_LIGHT,LOW);
          digitalWrite(Y2_LIGHT,LOW);//
          digitalWrite(R2_LIGHT,HIGH); 
                 

          digitalWrite(G3_LIGHT,LOW);
          digitalWrite(Y3_LIGHT,HIGH);//33333333333333
          digitalWrite(R3_LIGHT,LOW);
          
                
          delay(500);//
          
          digitalWrite(G3_LIGHT,LOW);
          digitalWrite(Y3_LIGHT,LOW);//333333333
          digitalWrite(R3_LIGHT,LOW);
      
          Serial.print("Green-YELLOW-RED- LIGHT");
          Serial.println(count);
      
          //delay(500);//
      }
      
      if(count <= 5)
          {
          
          digitalWrite(G1_LIGHT,LOW);
          digitalWrite(Y1_LIGHT,LOW);
          digitalWrite(R1_LIGHT,HIGH); // 
         
          digitalWrite(G2_LIGHT,LOW);
          digitalWrite(Y2_LIGHT,LOW);//
          digitalWrite(R2_LIGHT,HIGH); 
                 

          digitalWrite(G3_LIGHT,LOW);
          digitalWrite(Y3_LIGHT,LOW);//
          digitalWrite(R3_LIGHT,HIGH);                
             
      
          delay(500);//
      }
        if(digitalRead(R3_LIGHT) == HIGH && digitalRead(R2_LIGHT) == HIGH && digitalRead(R1_LIGHT) == HIGH)
        {
          Serial.println("HUMAN STREET :Green light!\n");
          
          digitalWrite(G4_LIGHT,HIGH);
          digitalWrite(R4_LIGHT,LOW);
          
          myservo.write(0); //Servo  0
          
          
        
        
        }
        
        else
        {
          
          Serial.println("HUMAN STREET :Red light!\n");

          digitalWrite(G4_LIGHT,LOW);
          digitalWrite(R4_LIGHT,HIGH);
          
          myservo.write(90); //Servo  90

        }
        
      
      
          delay(500);//
    }
  
  
  }
    
    
    


    
