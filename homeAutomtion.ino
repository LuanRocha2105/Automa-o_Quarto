#define out1 0 
#define out2 1
#define s1 2
#define s2 3 

int s1Read, s2Read;


void setup() {

pinMode(out1, OUTPUT);
pinMode(out2, OUTPUT);
pinMode(s1, INPUT);   //interruptor 1
pinMode(s2, INPUT);   //interruptor 2


 
Serial.begin(9600);
}

void loop() {

//s1Read = digitalRead(s1);   //interruptor 1 leitor
//s2Read = digitalRead(s2);   //interruptor 2 leitor
test();

switch (s1Read) {
  case 0: //digitalWrite(out1, LOW);
          Serial.print("lamp off\n");
          break;
  case 1: //digitalWrite(out1, HIGH);
          Serial.print("lamp on\n");
          break;  
}

switch (s2Read) {
  case 0: digitalWrite(out2, LOW);
          Serial.print("outlet off\n");
          break;
  case 1: digitalWrite(out2, HIGH);
          Serial.print("outlet on\n");
          break;  
}
delay(1000);


}
