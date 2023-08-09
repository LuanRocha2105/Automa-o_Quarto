#include <IRremote.hpp>


#define botaoLamp 23
#define botaoVent 25
#define botaoDesliga 27
#define botaoExaust 29
#define botaoVelMin 33
#define botaoVelMed 35
#define botaoVelMax 37
#define capFioVerde 9
#define capFioPreto 8 
#define exaust 7
#define vent 6
#define lamp 5  
//#define ledR 33   
//#define ledG 35   
//#define ledB 37 

const byte recep = 31;  
int  cod, block, fila, y = 0;
unsigned long int t, tempoLeitura;
unsigned int tempoDeParada = 60000;  // tempo de parada das pás do ventilador/exaustor quando desligado

void setup()
{
   Serial.begin(115200);
   Serial.println("IR Automação Iniciando...");
   IrReceiver.begin(recep, DISABLE_LED_FEEDBACK);

   delay(100);
   
   
   pinMode(botaoLamp, INPUT_PULLUP);
   pinMode(botaoVent, INPUT_PULLUP);
   pinMode(botaoDesliga, INPUT_PULLUP);
   pinMode(botaoExaust, INPUT_PULLUP);
   pinMode(botaoVelMin, INPUT_PULLUP);
   pinMode(botaoVelMed, INPUT_PULLUP);
   pinMode(botaoVelMax, INPUT_PULLUP);
   
   pinMode(capFioVerde, OUTPUT);
   pinMode(capFioPreto, OUTPUT);
   pinMode(exaust, OUTPUT);
   pinMode(vent, OUTPUT);
   pinMode(lamp, OUTPUT);
 //pinMode(ledR, OUTPUT);
 //pinMode(ledG, OUTPUT);
 //pinMode(ledB, OUTPUT);
   
   digitalWrite(capFioVerde, LOW);
   digitalWrite(capFioPreto, HIGH);
   digitalWrite(exaust, HIGH);
   digitalWrite(vent, HIGH);
   digitalWrite(lamp, HIGH);
 //digitalWrite(ledR, HIGH);
 //digitalWrite(ledG, HIGH);
 //digitalWrite(ledB, HIGH);


   delay(2000);
   Serial.println("Tudo pronto!");
}

void loop()
{  

//-------------------------------------------------------------- comandos usando os botões da parede ----------------------------------------------------------------//

   if(digitalRead(botaoLamp) == LOW){

     if((millis() - tempoLeitura) >= 500){
      
      alternar(lamp);
      tempoLeitura = millis();
      
     }
    }

    if(digitalRead(botaoVent) == LOW && digitalRead(botaoExaust) == HIGH){

     exaustor();
    
   }

   if(digitalRead(botaoExaust) == LOW && digitalRead(botaoVent) == HIGH){

     ventilador();
    
   }

   if(digitalRead(botaoDesliga) == LOW){

     desligapas();
    
   }

   if(digitalRead(botaoVelMin) == LOW){

    digitalWrite(capFioPreto, LOW);   // velocidade minima do ventilador/exaustor
    digitalWrite(capFioVerde, HIGH);
           
   }

   if(digitalRead(botaoVelMed) == LOW){

    digitalWrite(capFioPreto, HIGH);   // velocidade media do ventilador/exaustor
    digitalWrite(capFioVerde, LOW);
           
   }

   if(digitalRead(botaoVelMax) == LOW){

    digitalWrite(capFioPreto, LOW);   // velocidade maxima do ventilador/exaustor
    digitalWrite(capFioVerde, LOW);
           
   }

  
//---------------------------------------------------------------- comandos usando o controle ir --------------------------------------------------------------------//
  
   if (IrReceiver.decode())
   {
      cod = IrReceiver.decodedIRData.command;
      Serial.println(cod, DEC);
      delay(100);

      switch (cod){
        case 26:
           alternar(lamp);   // liga/desliga lampada
           break;          
        case 31: 
         /*
           alternar(ledR);   // liga/desliga led vermelho
           break;           
        case 30:
           alternar(ledG);   // liga/desliga led verde
           break;          
        case 9: 
           alternar(ledB);   // liga/desliga led azul
           break;   
        case 4:
           digitalWrite(ledR, LOW);   //deixa o led branco
           digitalWrite(ledG, LOW);
           digitalWrite(ledB, LOW);
           break;          
        case 10:
           digitalWrite(ledR, HIGH);   //desliga o led
           digitalWrite(ledG, HIGH);
           digitalWrite(ledB, HIGH);
           break;
         */            
        case 136:
           ventilador();   // liga o ventilador 
           break;    
        case 157:                
           exaustor();   // liga o exaustor
           break;           
        case 134:
           desligapas();   // desliga o ventilador/exaustor
           break;
        case 145:
           digitalWrite(capFioPreto, LOW);   // velocidade minima do ventilador/exaustor
           digitalWrite(capFioVerde, HIGH);
           break;           
        case 16:
           digitalWrite(capFioPreto, HIGH);   // velocidade média do ventilador/exaustor
           digitalWrite(capFioVerde, LOW);
           break;          
        case 139:
           digitalWrite(capFioPreto, LOW);   // velocidade máxima do ventilador/exaustor
           digitalWrite(capFioVerde, LOW);
           break;
           
      }

   }
   IrReceiver.resume();
   delay(10);

//---------------------------------------------------------- intertravamento ventilador/exaustor -------------------------------------------------------------------//
   
   switch(block){
    case 1: 
      if (fila != 0 &&(millis() - t) >= tempoDeParada){
     
         block =0;
         y = 0;
        
      }
      break;

    case 2: 
      if (fila != 0 &&(millis() - t) >= tempoDeParada){
        
        block = 0;
        y = 0;
        
      }
      break;
    case 3:
      if ((millis() - t) >= tempoDeParada){
    
         block = 0;
         y = 0;
    
      }
      break;         
    
   }

    if((fila != 0) && (block == 0)){

     switch (fila){
      case 1: 
        ventilador();
        break;
      case 2: 
        exaustor();
        break;
     }
    }


} //loop


//------------------------------------------------------------------------- FUNÇÕES --------------------------------------------------------------------------------//

void alternar(int x){   //função para alternar sinal de saida

 digitalWrite(x, !digitalRead(x));
 
}


void ventilador(){   //função para o modo ventilador

   digitalWrite(exaust, HIGH);
   if(block == 0) {
    
     digitalWrite(vent, LOW);   
     block = 1; 
     fila = 0; 
     
   }
   else{
    
     fila = 1;
      if (y == 0){
        
        t = millis();
        y = 1;
        
     }
   }
      
}


void exaustor(){  //função para o modo exaustor

    digitalWrite(vent, HIGH);  
    if(block == 0) {

      digitalWrite(exaust, LOW);
      block = 2;
      fila = 0;
     
    }
    else{
      
      fila = 2;
      if (y == 0){
        
        t = millis();
        y = 1;
        
      } 
    }
}


void desligapas(){  // desliga o ventilador/exaustor

  digitalWrite(vent, HIGH);   
  digitalWrite(exaust, HIGH);
  fila = 0;
  if (y == 0 && (block == 1 || block ==2)){
        
        t = millis();
        y = 1;
        
      }  
  block = 3;       
}
