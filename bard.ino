// Iv=5 LCD=(11,10,9,8,7,6) Rele=12 Botao=4 #4,5,6,7,8,9,10,11
// #1,2,3,12,13
unsigned long milise=0; 
#include <IRremote_library.h> //#include <IRremote>
#include <LiquidCrystal.h>
LiquidCrystal lcd(11,10,9,8,7,6);
const int IV = 5;
int temp,tempan,cont=0,seg=0,minut=0,hor=0,parado=0,minuta=0,hora=0,b=3000,timer=0,definido=5000,beep=50,desligar=0; 
bool Status,Status2,alarme,beeps;
IRrecv Rec(IV);
decode_results results;
void setup()
{
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("                ");
  Serial.begin(9600);
  Rec.enableIRIn();
  pinMode(12,1);
  pinMode(4,0);
  pinMode(13,1);
  pinMode(3,1);
}

void loop() {
//  digitalWrite (13,1); teste, deixar ligado LCD
  if (timer>0){
    timer--;
    if (beep>0){
      if (beep==50){
        if (beeps) {digitalWrite(3,0);Serial.println('a');beeps=0;}else {digitalWrite(3,1);Serial.println('b');beeps=1;}
      }
      beep--;  
    }else beep=50;
  }
  b++;
  Serial.println(timer);
  synchora();
  lcd.setCursor(0,0);
  tempan=temp;
  temp=analogRead(A0);
  if (tempan-temp>5 || tempan-temp<-5) temp=map(temp,0,1023,0,510); //60=30 41=19
//
  lcd.setCursor(0,0);lcd.print("                ");
  synchora();
 // Serial.print("Temp=");Serial.println(temp);
  if (Rec.decode(&results)) {
    Serial.println("infraVermelho");
    if (results.value==0x800F840C && !Status){
      if (b>100){
        digitalWrite(12,1);
        Serial.println("Liga");
        Status=1;b=0;desligar=90;
      }  
    }
    else if (results.value==0x800F840C && Status){
      if (b>100){
        digitalWrite(12,0);
        Serial.println("desLiga");
        Status=0; b=0;desligar=0;
      }
    }
    else if (results.value==0x800F040C && !Status){
      if (b>100){
        digitalWrite(12,1);
        Serial.println("Liga");
        Status=1;b=0;desligar=90;
      } 
    }
    else if (results.value==0x800F040C && Status){
      if (b>100){
        digitalWrite(12,0);
        Serial.println("desLiga");
        Status=0; b=0;desligar=0;
      }
    }
    else if (results.value==0x800F840D || results.value==0x800F040D){
      if (Status2 && b>100){
        digitalWrite(13,0);
        Serial.println("desLigaLCD");
        Status2=0; b=0;
      }
      else if (!Status2 && b>100){
        digitalWrite(13,1);
        Serial.println("LigaLCD");
        Status2=1; b=0;
      }
    }
    else if (results.value==0x800F0424 || results.value==0x800F8424){
      Rec.resume();
      while (parado<200){
        lcd.setCursor(0,0);lcd.print("Editando Hora");
        synchora();
        if (Rec.decode(&results)){
          if (results.value==0x800F0480 || results.value==0x800F8480){
           parado=0;
             hor++;  
          }
          else if (results.value==0x800F0481 || results.value==0x800F8481){
              parado=0;
              minut++;  
          }
          else if (results.value==0x800F840D || results.value==0x800F040D){
            if (Status2 && b>100){
            digitalWrite(13,0);
            Serial.println("desLigaLCD");
            Status2=0; b=0;
            }
            else if (!Status2 && b>100){
              digitalWrite(13,1);
              Serial.println("LigaLCD");
              Status2=1; b=0;
            }
          }
          Rec.resume();
        }
        Serial.println(parado);
        parado++; 
      }
    }
    else if (results.value==0x800F0480 || results.value==0x800F8480){
      Rec.resume();
      while (parado<200){
        lcd.setCursor(0,0);lcd.print("Editando Alarme");
        syncalarme();
        if (Rec.decode(&results)){
          if (results.value==0x800F0480 || results.value==0x800F8480){
           parado=0;
             hora++;
             if (hora>24)hora=0 ;
          }
          else if (results.value==0x800F0481 || results.value==0x800F8481){
              parado=0;
              minuta++; 
              if(minuta>60)minuta=0;
          }
          else if (results.value==0x800F0424 || results.value==0x800F8424){
            definido=definido+100;
            if (definido>10000) definido=0;
            parado=0;
          }
          else if (results.value==0x800F840D || results.value==0x800F040D){
            if (Status2 && b>100){
            digitalWrite(13,0);
            Serial.println("desLigaLCD");
            Status2=0; b=0;
            }
            else if (!Status2 && b>100){
              digitalWrite(13,1);
              Serial.println("LigaLCD");
              Status2=1; b=0;
            }
          }
          Rec.resume();
        }
        Serial.println(parado);
        parado++; 
      }
    }
    else if (results.value==0x800F840E || results.value==0x800F040E){
      if (b>100){
        if (!alarme) {
          alarme=1; b=0;
          Serial.println("Alarme1");
        } else {
          alarme=0; b=0;
          Serial.println("alarme0");
          }
      }
    }
    else {Serial.println("Err: Tecla nao reconhecida");b=2000;}
    parado=0;
    Rec.resume();
    b++;
    }
  if (digitalRead(4)==0){
    Serial.println("Botão:");
    if (!Status){
      digitalWrite(12,1);
      Serial.println("Liga");
      timer=0;desligar=90;
      digitalWrite(3,0);
      Status=1; 
    }
    else if (Status){
      digitalWrite(12,0);
      Serial.println("desLiga");
      timer=0;desligar=0;
      digitalWrite(3,0);
      Status=0; 
    }
    while(digitalRead(4)==0){
      synchora();
      }    
  }
  if (hora==hor && minut==minuta && alarme) {
    timer=definido;
    alarme=0;
  }
//  delay(100);
  if (desligar==0 && Status) {
    digitalWrite(12,0);
    Status=0;
  }
}
void synchora(){
  lcd.setCursor(0,1);lcd.print(hor);lcd.print(":");lcd.print(minut);lcd.print(':');lcd.print(seg);lcd.print(" ");lcd.print(temp);lcd.print("oC");if (alarme) {lcd.print(" a");} else {lcd.print("   ");}
  if ((millis()-milise)>=1000){
    milise=milise+1000;
    seg++;    
  }
  if (seg>=60){
    if(desligar>0)desligar--;
    seg=0;
    minut++;
  }
  if (minut>=60){
    minut=0;
    hor++;
  }
  if (hor>=24) hor=0;
}
void syncalarme(){
  lcd.setCursor(0,1);lcd.print(hora);lcd.print(":");lcd.print(minuta);lcd.print(" | ");lcd.print(definido);lcd.print("   ");
    if ((millis()-milise)>1000){
    milise=milise+1000;
    seg++;    
  }
  if (seg>=60){
    seg=0;
    minut++;
  }
  if (minut>=60){
    minut=0;
    hor++;
  }
  if (hor>=24) hor=0;
}
