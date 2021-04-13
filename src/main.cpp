#include <Arduino.h>
#include <pinout.h>
#include <pinmode.h>
#include <ArduinoUniqueID.h>

uint32_t last_keys = 0;
uint32_t current_keys = 0;
unsigned long key_times[22];
const int lamps_list[] = { lamp0, lamp1, lamp2, lamp3, lamp4, lamp5, lamp6, lamp7, lamp8, lamp9,
                 lamp10, lamp11, lamp12, lamp13, lamp14, lamp15, lamp16, lamp17,
                 lamp18, lamp19, lamp20, lamp21 };


void evento_teclado(uint32_t _teclas_ativadas);
void testa_lamps();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial1.begin(9600);
  //Serial.begin(9600); 
  definir_pinout();
  testa_lamps();
  }

void liga_lamp(int lamp){
  digitalWrite(lamp, LOW);
}

void desliga_lamp(int lamp){
  digitalWrite(lamp, HIGH);
}

void testa_lamps(){
  for (int i=0; i<=21; i++){
    liga_lamp(lamps_list[i]);
  }
  delay(500);
  for (int i=0; i<=21; i++){
    desliga_lamp(lamps_list[i]);
  }
  delay(500);
  for (int i=0; i<=21; i++){
    liga_lamp(lamps_list[i]);
    delay(200);
    desliga_lamp(lamps_list[i]);
  }
}

void set_out_in_in(int out, int in1, int in2){
  pinMode(out, OUTPUT);
  delay(1);
  digitalWrite(out, 0);
  pinMode(in1, INPUT_PULLUP);
  pinMode(in2, INPUT_PULLUP);
}

uint32_t le_teclas(){
  uint8_t status_teclas[3];
  uint32_t teclas_ativadas = 0;
  int colunas_lista[] = {Col0, Col1, Col2, Col3, Col4, Col5, Col6, Col7};
  //firstline
  status_teclas[0]=0;
  set_out_in_in(Row0, Row1, Row2);
  for (int i=0; i<=7; i++){
    int status = digitalRead(colunas_lista[i]);
    if (status == LOW) {
      bitSet(status_teclas[0],i);
      }
  }
  //secondline
  status_teclas[1]=0;
  set_out_in_in(Row1, Row0, Row2);
  for (int i=0; i<=7; i++){
    int status = digitalRead(colunas_lista[i]);
    if (status == LOW) {
      bitSet(status_teclas[1],i);
    }
  }
  //thirdline
  status_teclas[2]=0;
  set_out_in_in(Row2, Row0, Row1);
  for (int i=0; i<=7; i++){
    int status = digitalRead(colunas_lista[i]);
    if (status == LOW) {
      bitSet(status_teclas[2],i);
    }
  }
  //finalizacao
  if (status_teclas[0] & 0b00000001)
    bitSet(teclas_ativadas, 6);
  if (status_teclas[0] & 0b00000010)
    bitSet(teclas_ativadas, 5);
  if (status_teclas[0] & 0b00000100)
    bitSet(teclas_ativadas, 4);
  if (status_teclas[0] & 0b00001000)
    bitSet(teclas_ativadas, 3);
  if (status_teclas[0] & 0b00010000)
    bitSet(teclas_ativadas, 2);
  if (status_teclas[0] & 0b00100000)
    bitSet(teclas_ativadas, 1);
  if (status_teclas[0] & 0b01000000)
    bitSet(teclas_ativadas, 0);
  if (status_teclas[0] & 0b10000000)
    bitSet(teclas_ativadas, 20);
  if (status_teclas[1] & 0b00000001)
    bitSet(teclas_ativadas, 13);
  if (status_teclas[1] & 0b00000010)
    bitSet(teclas_ativadas, 12);
  if (status_teclas[1] & 0b00000100)
    bitSet(teclas_ativadas, 11);
  if (status_teclas[1] & 0b00001000)
    bitSet(teclas_ativadas, 10);
  if (status_teclas[1] & 0b00010000)
    bitSet(teclas_ativadas, 9);
  if (status_teclas[1] & 0b00100000)
    bitSet(teclas_ativadas, 8);
  if (status_teclas[1] & 0b01000000)
    bitSet(teclas_ativadas, 7);
  if (status_teclas[1] & 0b10000000)
    bitSet(teclas_ativadas, 21);
  if (status_teclas[2] & 0b00000010)
    bitSet(teclas_ativadas, 19);
  if (status_teclas[2] & 0b00000100)
    bitSet(teclas_ativadas, 18);
  if (status_teclas[2] & 0b00001000)
    bitSet(teclas_ativadas, 17);
  if (status_teclas[2] & 0b00010000)
    bitSet(teclas_ativadas, 16);
  if (status_teclas[2] & 0b00100000)
    bitSet(teclas_ativadas, 15);
  if (status_teclas[2] & 0b01000000)
    bitSet(teclas_ativadas, 14);
  if (last_keys != teclas_ativadas){
    evento_teclado(teclas_ativadas);
    last_keys = teclas_ativadas;
  }
    return teclas_ativadas;
}

void evento_teclado(uint32_t _teclas_ativadas){
  uint32_t teclas_que_mudaram = (_teclas_ativadas ^ last_keys);
  for (int i=0; i<=21; i++){
    if (teclas_que_mudaram & ((uint32_t)1 << i)){
        Serial.println(_teclas_ativadas, BIN);
        Serial.println(((uint32_t)1 << i), BIN);
        if(_teclas_ativadas & ((uint32_t)1 << i)){
          Serial.print("Tecla pressionada: ");
          Serial.println(i);
          if (!(current_keys & ((uint32_t)1 << i))){
            bitSet(current_keys, i);
            liga_lamp(lamps_list[i]);
            key_times[i] = millis();
          }
          else {
            bitClear(current_keys, i); 
            desliga_lamp(lamps_list[i]);
          }
        }
        else{
          Serial.print("Tecla solta: ");
          Serial.println(i);
          if (millis() > key_times[i] + 500){
            bitClear(current_keys, i);
            desliga_lamp(lamps_list[i]);
            
          }
        }
    }
  }
 // Serial.println(current_keys, BIN);
  //UniqueIDdump(Serial);
}

void loop() {
  le_teclas();
  delay(50);

  // put your main code here, to run repeatedly:
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/

void serialEvent1(){
  //Serial.println("Incoming SerialEvent1, ecoando para Serial");
  //Serial.println()
//statements
}

void serialEvent2(){
  //Serial.print("SerialEvent1");
//statements
}
