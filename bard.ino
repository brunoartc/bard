
unsigned long milise=0;
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Time.h>
LiquidCrystal lcd(11,10,9,8,7,6);
int temp,tempan,cont=0,seg=0,parado=0,minuta=0,hora=0,b=3000,timer=0,definido=120,beep=50,desligar=0,timerr=90,hor=0,minut=0,hor1=0,minut1=0; 
char biit;
bool Status,Status2,alarme,beeps,mudo=1;
IRrecv Rec(5);
decode_results results;
void setup(){
  setTime(0,0,0,1,1,2000); //Define clock | Define o tempo
  lcd.begin(16,2); // LCD Dimensions | Dimensoes do LCD
  lcd.setCursor(0,0);lcd.print("                "); //Clear LCD Screen | Limpa tela LCD
  Serial.begin(9600); //Starts USB/Bluethoot comunicatio | Comeca coneccao UBS/Bluethoot
  Rec.enableIRIn(); //Starts IR reciver | Comeca o receptor de IV
  pinMode(12,1); // Relay|Rele
  pinMode(4,0); //Button |Botao
  pinMode(13,1); // LCD Bright | Brilho LCD
  pinMode(3,1); // Buzzer
    digitalWrite(3,1);
    delay(50);
    digitalWrite(3,0);
    delay(50);
    digitalWrite(3,1);
    delay(50);
    digitalWrite(3,0);
    delay(50);
    digitalWrite(3,1);
    delay(100);
    digitalWrite(3,0);
}
void MostraTempo(){
  lcd.setCursor(0,1);lcd.print(hour());lcd.print(":");lcd.print(minute());lcd.print(':');lcd.print(second());lcd.print(" ");
  lcd.print(temp);lcd.print("oC");
  if (alarme) {lcd.print(" a");} else if (desligar>0){lcd.print(desligar);} else {lcd.print("    ");}
}
void loop() {
  //digitalWrite (13,1);/* teste, deixar ligado LCD */
  MostraTempo();
  while (timer>0){
    timer--;
    if (digitalRead(4)==0) timer=0;
    
    digitalWrite(3,1);Serial.println('a');
    digitalWrite(12,0);
    Serial.println("DesLiga");
    delay(300);
    
    digitalWrite(3,0);Serial.println('b');
    Serial.println(timer);
    digitalWrite(12,1);
    Serial.println("Liga");
    desligar=timerr;
    delay(200);
    Status=1; 
    alarme=0;
  }

  
  b++;
  lcd.setCursor(0,0);
  //Serial.println(second());
  //Serial.println(minute());
  //Serial.println(timer);
  tempan=temp;temp=analogRead(A0);
  if (tempan-temp>5 || tempan-temp<-5 || b>100) temp=map(temp,0,1023,0,510); //60=30 41=19



  if (Serial.available()){
    Serial.println("Bluetooth:");
    biit = Serial.read();
    while (biit=='a'){
      if (Serial.available()){
        hor=Serial.read();
        while (hor>-1){
          if (Serial.available()){
          hor1=Serial.read();
            while (hor1>-1){
              if (Serial.available()){
                minut=Serial.read();
                while (minut>-1){
                  if (Serial.available()){
                    minut1=Serial.read();
                    hor1=((hor-48)*10)+hor1-48;
                    minut1=((minut-48)*10)+minut1-48;     
                    hora=hor1;minuta=minut1;
                    Serial.println("sync:");Serial.print(hor1);Serial.print(":");Serial.print(minut1);
                    biit='e';
                    minut=-1;
                    hor=-1;
                    hor1=-1;
                    minut1=-1;
                    alarme=1;
                    break; 
                }
              }
            }
          }
        }
      }        
    }
  } 
    while (biit=='h'){
      if (Serial.available()){
        hor=Serial.read();
        while (hor>-1){
          if (Serial.available()){
          hor1=Serial.read();
            while (hor1>-1){
              if (Serial.available()){
                minut=Serial.read();
                while (minut>-1){
                  if (Serial.available()){
                    minut1=Serial.read();
                    hor1=((hor-48)*10)+hor1-48;
                    minut1=((minut-48)*10)+minut1-48;
                    
                    setTime(hor1,minut1,0,1,1,2000);
                    Serial.println("sync:");Serial.print(hor1);Serial.print(":");Serial.print(minut1);
                    biit='e';
                    minut=-1;
                    hor=-1;
                    hor1=-1;
                    minut1=-1;
                    break; 
                }
              }
            }
          }
        }
      }        
    }
  } 
    if (biit=='l'){
      digitalWrite(12,1);
      Serial.println("Liga");
      timer=0;desligar=timerr;
      digitalWrite(3,0);
      Status=1; 
    }
    
    else if (biit=='d'){
      digitalWrite(12,0);
      Serial.println("desLiga");
      timer=0;desligar=0;
      digitalWrite(3,0);
      Status=0; 
    }
    else if (biit=='@'){
      if (Status2){
        digitalWrite(13,0);
        Serial.println("desLigaLCD");
        Status2=0; b=0;
      }
      else if (!Status2){
        digitalWrite(13,1);
        Serial.println("LigaLCD");
        Status2=1; b=0;
      }
    }  
    else if (biit=='M'){
      if (mudo){
        mudo=0; b=0;
      }
      else if (!mudo){
        mudo=1; b=0;
      }
    }
    if(mudo!=1){digitalWrite(3,1);
    delay(50);
    digitalWrite(3,0);}
  } 

  
  if (Rec.decode(&results) && b>100) {  //integrar mudo ao controle
    Serial.println("infraVermelho:");
    if (results.value==0x800F840C && !Status){
      if (b>100){
        digitalWrite(12,1);
        Serial.println("Liga");
        Status=1;b=0;desligar=timerr;
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
        Status=1;b=0;desligar=timerr;
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
        lcd.setCursor(0,0);lcd.print("Editando Hora ");lcd.print(timerr);
        MostraTempo();
        if (Rec.decode(&results)){
          if (results.value==0x800F0480 || results.value==0x800F8480){
           parado=0;
             hor++;  
             setTime(hor,minut,0,1,1,2000);
          }
          else if (results.value==0x800F0481 || results.value==0x800F8481){
              parado=0;
              minut++; 
              setTime(hor,minut,0,1,1,2000); 
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
          else if(results.value==0xFFFFFFFF || results.value==0xFFFFFFFF){ //0xFFFFFFFF=bota para aumentar 'timer'
              parado=0;
              timer++;  
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
    if(mudo!=1){digitalWrite(3,1);
    delay(50);
    digitalWrite(3,0);}
    }
  if (digitalRead(4)==0){
    Serial.println("Botão:");
    if (!Status){
      digitalWrite(12,1);
      Serial.println("Liga");
      timer=0;desligar=timerr;
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
      MostraTempo();
      }    
  }
  if (hora==hour() && minute()==minuta && alarme) {
    timer=definido;
    alarme=0;
  }
//  delay(100);
  if (desligar==0 && Status) {
    digitalWrite(12,0);
    Status=0;
  }
}
void syncalarme(){
  lcd.setCursor(0,1);lcd.print(hora);lcd.print(":");lcd.print(minuta);lcd.print(" | ");lcd.print(definido);lcd.print("   ");
}
