//Definicion de pines
const long rout = 0x00 + 0x13 + 0xA2 + 0x00 + 0x40 + 0xB6 + 0x6B + 0x14;//direction of xbee router
long rec;
byte trash[50];
int i;
char data;
String dato = "0";

const int temp = A1;

void setup() {
  Serial3.begin(115200);
  Serial.begin(115200);
}

//Define los ultimos 3 bytes de la direccion MAC destino

void router() {
  Serial3.write(byte(0xB6));
  Serial3.write(byte(0x6B));
  Serial3.write(byte(0x14));
}

//Descompone el numero en sus factores y retorna la longitud igual a 4

String msg(String valor) {
  //int temperatura = analogRead(temp);
  //Serial3.write(byte(temperatura));
  Serial3.write(byte('7'));
  //Serial3.write(byte('O'));
  //Serial3.write(byte('K'));
  return "7";
  //return temperatura;
}

long checksum(long receiver, String text) {
  long cs = 0x10 + 0x00 + receiver + 0xFF + 0xFE + byte('O') + byte('K');
  return cs;
}

//Envia el dato de 4 bytes

//@param to represents the receiver of the message
//@param message[] is a string containing the message to be sent
void chat(int to) {
  int frameLen = 1 + 14;//length of the API frame is calculated
  long len = ("0x%p\n", frameLen);//frame length is converted to type long
  Serial3.write(byte(0x7E));//delimiter
  Serial3.write(byte(0x00));//length LSB
  Serial3.write(byte(len));//length MSB
  Serial3.write(byte(0x10));//frameType (transmit request)
  Serial3.write(byte(0x00));//frameId
  Serial3.write(byte(0x00));//high destination address
  Serial3.write(byte(0x13));//high destination address
  Serial3.write(byte(0xA2));//high destination address
  Serial3.write(byte(0x00));//high destination address
  Serial3.write(byte(0x40));//high destination address
  if (to == 1) { //selects receiver from method param to
    router();
    rec = rout;//places destination address in variable ’rec’ type long
  }
  Serial3.write(byte(0xFF));//const
  Serial3.write(byte(0xFE));//const
  Serial3.write(byte(0x00));//const (options)
  Serial3.write(byte(0x00));//const (options)
  String mVal;
  mVal = msg(dato);//calls method to write message to serial and sets message length in variable ’mVal’ type long
  long cs = checksum(rec, mVal); //sets checksum value to variable ’cs’ type long
  Serial3.write(byte(0xFF - (cs & 0xFF)));//checksum is fully calculated and written to serial
}


void serialEvent3()
{
  delay(10);//small delay to let Serial buffer(s) load
  String st;
  int aux1 = 0;
  if (Serial3.available() > 17) {//require a minimum frame length of 17 (message with one character at least)
    data = Serial3.read();//data read from Serial is stored in variable ’data’ type char
    if (data == 0x7E) {//assures that incoming data is an API frame from another xbee

      data = Serial3.read();
      data = Serial3.read();

      for (int i = 0; i < data-1; ++i) { //cycles through the next 12bytes
        trash[i] = Serial3.read();//places bytes in variable ’trash’ type byte
        aux1 = i;
      }
     // i = -1;
      //i = Serial3.read();//first character of message
      byte checkOut = Serial3.read();

      //Serial.print("Este es foraneo ");
      //Serial.println(checkOut);

      byte checkForaneo = convertir_mensaje(trash, aux1);

      if (int(checkOut) == int(checkForaneo)) {
        Serial.println("OK");
        delay(500);
        chat(1);
      }
      i = Serial3.read();//continues reading bytes
    }
  }
  for (int i = 12; i <= aux1; i++) {
    Serial.print((char)trash[i]);
  }
  Serial.println();

  st = Serial3.readStringUntil('\n');
}
byte convertir_mensaje(byte text [], int aux1) {
  byte mensaje = 0;
  char aux;
  for (int i = 12; i <= aux1; i++) {
    aux = (char)trash[i];
    mensaje += aux;
  }
  //Serial.print("Este es local ");
  //Serial.println(mensaje);

  return mensaje;
}

void loop()
{
  int valor = analogRead(temp);
  //Serial.println(byte(valor/4));
   // if(Serial3.available()){
     // Serial.print("yes");
      //serialEvent3();
     //delay(3000);
    //}
  //  delay(10);//small delay to let Serial buffer load
  const char msg[] = "21\n";//message to be sent, \n is needed so receiver may fully capture the message
  chat(1); //calls chat method, sent to router
  delay(1000);

}
