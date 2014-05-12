/**********************************************************************
* © 2014 YD Ynvisible, S.A.
*
* FileName:        Mini_Ziphius.ino
* Dependencies:    SoftwareSerial.h
* Processor:       ATmega328
* IDE:             Arduino 1.0.5
*
* Description:
* Driving a 2 motors drone through a smartphone via Bluetooth 4.0
* We have an App with a graphical interface to interact with the drone
**********************************************************************/

#include <SoftwareSerial.h> //Software Serial Port
#define RxD 6 //Bluetooth TX=Core RX
#define TxD 7 //Bluetooth RX=Core TX
#define FRONT 1
#define BACK 0
#define OFF 0
#define ON 1
SoftwareSerial blueToothSerial(RxD,TxD);

int motor1_pwm=9;
int motor1_dir=8;
int motor2_pwm=11;
int motor2_dir=12;
char X[3], Y[3], A, B;
int X1_num;
int i=1;
int X_num,Y_num, A_num, B_num;
int X_numf,Y_numf, A_numf, B_numf;
int motor1_way,motor2_way;
char recvChar;

void setup()
{
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  pinMode(motor1_pwm, OUTPUT); //Motor1 PWM
  pinMode(motor1_dir, OUTPUT); //Motor1
  pinMode(motor2_pwm, OUTPUT); //Motor2 PWM
  pinMode(motor2_dir, OUTPUT); //Motor3
  digitalWrite(motor1_pwm,LOW);
  digitalWrite(motor1_dir,LOW);
  digitalWrite(motor2_pwm,LOW);
  digitalWrite(motor2_dir,LOW);
  setupBlueToothConnection();
}
void loop()
{
  if(blueToothSerial.available()){          //check if the pipe is free
    recvChar = blueToothSerial.read();
    if(recvChar!='\n'){                     //recognise end of line
      if(i==2 && recvChar==','){            //treatment of data sent from the smartphone
        i++;X[1]=NULL;
      }
      if((i==4 || i==5) && recvChar==','){  //treatment of data sent from the smartphone
        i++;Y[1]=NULL;
      }
      switch(i){
        case 1: X[0]=recvChar;break;
        case 2: X[1]=recvChar;break;      
        case 3: break;      //comma
        case 4: Y[0]=recvChar;break;
        case 5: Y[1]=recvChar;break;
        case 6: break;      //comma     
        case 7: A=recvChar;break;
        case 8: B=recvChar;break; 
        default:break;     
      }  
      if(i==3 && recvChar!=','){            //treatment of data sent from the smartphone
        X[0]='9';X[1]='9';i--;
      }
      if(i==6 && recvChar!=','){            //treatment of data sent from the smartphone
        Y[0]='9';Y[1]='9';i--;
      }
      i++;
      
      if(i==9){                             //all data received?... start calculation convertion
        X_num=atoi(X);
        Y_num=atoi(Y);
        A_num=A-'0';
        B_num=B-'0';

        if(Y_num>=50){
          Y_numf=Y_num-50;
          Y_numf=Y_numf*255.0/50.0;
          Y_numf=255-Y_numf;
          motor1_way=FRONT;
          motor2_way=FRONT;
          
        }
        else{
          Y_numf=50-Y_num;
          Y_numf=Y_numf*255.0/50.0;
          motor1_way=BACK;
          motor2_way=BACK; 
      
        }
        Y_num=(int)Y_numf;
        
        if(X_num>=50){   
          motor1_way=ON;
        }
        else{
          motor2_way=ON;   
        }
        analogWrite(motor1_pwm,Y_num);        //Write the converted data sent from the smartphone to the motor driver
        digitalWrite(motor1_dir,motor1_way);
        analogWrite(motor2_pwm,255-Y_num);
        digitalWrite(motor2_dir,!motor2_way);
        i=1; 
      }
    }
  } 
}  
  
void setupBlueToothConnection(){
  blueToothSerial.begin(9600); //Set BluetoothBee BaudRate to default baud rate 9600
  delay(10); // This delay is required.
  blueToothSerial.print("AT+RENEW"); //Restore all setup value to factory setup
  delay(10);  // This delay is required.
  blueToothSerial.print("AT+ROLE0"); //make it a slave   
  delay(10);  // This delay is required.
}
