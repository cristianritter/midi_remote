#include <Arduino.h>
#include <pinout.h>
#include <pinmode.h>
#include <ArduinoUniqueID.h>
#include <EEPROM.h>
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, Midi);  //porta midi selecionada

uint32_t last_keys = 0;
uint32_t current_keys = 0;
unsigned long key_times[22];
const int lamps_list[] = { lamp0, lamp1, lamp2, lamp3, lamp4, lamp5, lamp6, lamp7, lamp8, lamp9,
                 lamp10, lamp11, lamp12, lamp13, lamp14, lamp15, lamp16, lamp17,
                 lamp18, lamp19, lamp20, lamp21 };
unsigned long time_le_teclas = 0;
unsigned long last_key_event = 0;

void evento_teclado(uint32_t _teclas_ativadas);
void testa_lamps();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Midi.begin();   //serial2
  //Serial2.begin(31250);
  //Serial1.begin(9600);
  definir_pinout();
  testa_lamps();
  }

void enviaMidi(int bt, bool estado);

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
  delay(150);
  for (int i=0; i<=21; i++){
    desliga_lamp(lamps_list[i]);
  }
  delay(150);
  for (int i=0; i<=21; i++){
    liga_lamp(lamps_list[i]);
    delay(30);
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

void le_teclas(){
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
  if (last_keys != teclas_ativadas){  //chama se alguma tecla mudou
    evento_teclado(teclas_ativadas);
    last_keys = teclas_ativadas;
  }
}

void verifica_evento(char msg[400]){
    char * pch;
    Serial.println(msg);
    if (msg[0]=='R' && msg[1]=='E' && msg[2]=='C'){
      pch = strtok(msg, "[], ");
      pch = strtok(NULL, "[], ");
      int posicao = atoi(pch);
      Serial.print("posicao:");
      Serial.println(posicao);
      int i=0;
      while (pch != NULL)
      {
            pch = strtok(NULL, "[], ");
            Serial.println(pch);
            EEPROM.write(posicao*100+i, atoi(pch));
            i++;
      }

    }
}

void enviaMidi(int bt, bool estado){
    int i=0;
    byte sysEx[14];
    int start_point;
    if (estado == true)  
      start_point = 0;
    else 
      start_point = 14;
    for (i=0; i<14; i++){       
        int valor = EEPROM.read(bt*100+i+start_point);
        Serial.print(valor,HEX);
        Serial.print("-");
        sysEx[i] = valor;
      }
    Serial.println(") ");
    Midi.sendSysEx(i, sysEx, false);
}

void evento_teclado(uint32_t _teclas_ativadas){
  if ( (last_key_event+300) < millis() ){
    last_key_event = millis();
  }
  else{
    return;
  }
  uint32_t teclas_que_mudaram;
  teclas_que_mudaram = (_teclas_ativadas ^ last_keys);
  for (int i=0; i<=21; i++){
    if (teclas_que_mudaram & ((uint32_t)1 << i)){    //se a tecla mudou desde o ultimo evento
        if(_teclas_ativadas & ((uint32_t)1 << i)){   //se a tecla esta ativada agora
          if (!(current_keys & ((uint32_t)1 << i))){ //se a funcao nao ta ativada
            bitSet(current_keys, i);
            Serial.print("Tecla ativada: ");
            Serial.print(i);
            Serial.print("    (");
            liga_lamp(lamps_list[i]);
            enviaMidi(i, true);
            key_times[i] = millis();
            return;
          }
          else if (current_keys & ((uint32_t)1 << i)) {
              Serial.print("Tecla desativada: ");
              Serial.print(i);
              Serial.print("    (");
              bitClear(current_keys, i); 
              desliga_lamp(lamps_list[i]);
              enviaMidi(i, false);
              return;
          }
        }
        if (current_keys & ((uint32_t)1 << i)){
            if (millis() > key_times[i] + 500){
                Serial.print("Tecla desativada: ");
                Serial.print(i);
                Serial.print("    (");
                bitClear(current_keys, i);
                desliga_lamp(lamps_list[i]);
                enviaMidi(i, false);
            }
        }
    }
  }
}

void loop() {
  if ( (time_le_teclas+100) < millis() ){
    time_le_teclas = millis();
    le_teclas();
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/

void serialEvent(){
  char msg[400];
  int idx = 0;
  while (Serial.available()) {
    delay(1);
    msg[idx] = (char)Serial.read();
    idx++;
  }
  msg[idx]=0;
  verifica_evento(msg);
  Serial.print(msg);
}

void serialEvent1(){
}

void serialEvent2(){
}
