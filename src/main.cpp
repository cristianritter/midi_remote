#include <Arduino.h>
#include <pinout.h>
#include <pinmode.h>

uint32_t vetor_teclado;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  definir_pinout();
  }

void liga_lamp(int lamp){
  digitalWrite(lamp, HIGH);
}

void desliga_lamp(int lamp){
  digitalWrite(lamp, LOW);
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
  set_out_in_in(Row3, Row0, Row1);
  for (int i=0; i<=7; i++){
    int status = digitalRead(colunas_lista[i]);
    if (status == LOW) {
      bitSet(status_teclas[2],i);
    }
  }
  //finalizacao
  if (status_teclas[0] & 0b01000000)
    bitSet(teclas_ativadas, 0);
  if (status_teclas[0] & 0b00100000)
    bitSet(teclas_ativadas, 1);
  if (status_teclas[0] & 0b00010000)
    bitSet(teclas_ativadas, 2);
  if (status_teclas[0] & 0b00001000)
    bitSet(teclas_ativadas, 3);
  if (status_teclas[0] & 0b00000100)
    bitSet(teclas_ativadas, 4);
  if (status_teclas[0] & 0b00000010)
    bitSet(teclas_ativadas, 5);
  if (status_teclas[0] & 0b00000001)
    bitSet(teclas_ativadas, 6);
  if (status_teclas[1] & 0b01000000)
    bitSet(teclas_ativadas, 7);
  if (status_teclas[1] & 0b00100000)
    bitSet(teclas_ativadas, 8);
  if (status_teclas[1] & 0b00010000)
    bitSet(teclas_ativadas, 9);
  if (status_teclas[1] & 0b00001000)
    bitSet(teclas_ativadas, 10);
  if (status_teclas[1] & 0b00000100)
    bitSet(teclas_ativadas, 11);
  if (status_teclas[1] & 0b00000010)
    bitSet(teclas_ativadas, 12);
  if (status_teclas[1] & 0b00000001)
    bitSet(teclas_ativadas, 13);
  if (status_teclas[2] & 0b01000000)
    bitSet(teclas_ativadas, 14);
  if (status_teclas[2] & 0b00100000)
    bitSet(teclas_ativadas, 15);
  if (status_teclas[2] & 0b00010000)
    bitSet(teclas_ativadas, 16);
  if (status_teclas[2] & 0b00001000)
    bitSet(teclas_ativadas, 17);
  if (status_teclas[2] & 0b00000100)
    bitSet(teclas_ativadas, 18);
  if (status_teclas[2] & 0b00000010)
    bitSet(teclas_ativadas, 19);
  if (status_teclas[0] & 0b10000000)
    bitSet(teclas_ativadas, 20);
  if (status_teclas[1] & 0b10000000)
    bitSet(teclas_ativadas, 21);
    return teclas_ativadas;
}

void loop() {
  vetor_teclado = le_teclas();
  Serial.println(vetor_teclado, BIN);
  delay(50);

  //for (int i=0; i<20; i++){
    //Serial.print(teclado[i]);
    //Serial.print(' ');
  //}
  //Serial.print('\n');
  // put your main code here, to run repeatedly:
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent1(){
  Serial.print("SerialEvent1");
//statements
}

void serialEvent2(){
  Serial.print("SerialEvent1");
//statements
}
