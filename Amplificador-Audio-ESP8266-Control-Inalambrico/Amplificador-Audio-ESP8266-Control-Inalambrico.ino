//Proyecto: Amplificador con control wifi utilizando el modulo ESP8266
//Estudiante: Adrian Mayora 16-00131

//--------------------------------------LIBRERIAS A UTILIZAR----------------------------------------------------------------------
#include <ESP8266WiFi.h>;                                 // Nos ayuda a conectarnos a la red wifi
#include <PubSubClient.h>;                                // Nos ayuda a conectarnos al broker MQTT

#include "encendido.h"                                    // Datos del audio
#include "apagado.h"                                      // Datos del audio
#include "calentamiento.h"                                    // Datos del audio
#include "AudioFileSourcePROGMEM.h"                       // Nos ayudan a ejecutar el audio de encendido
#include "AudioGeneratorWAV.h"                            //
#include "AudioOutputI2SNoDAC.h"                          //

//--------------------------------------DATOS DEL WIFI----------------------------------------------------------------------

const char* ssid = "A9";                                    // Ingreso el nombre de la red wifi ala que me deseo conectar                                    
const char* password = "12345678";                          // Ingreso la contraseña de la red wifi ala que me deseo conectar  


//--------------------------------------DATOS DEL BROKER---------------------------------------------------------------------

const char* mqtt_server = "node02.myqtthub.com";            // ingreso la direccion del broker al que me deseo conectar
const char* Id = "ESP8266";                                 // ingreso la Id del broker al que me deseo conectar
const char* User = "strix0087";                             // ingreso el nombre de usuario con el que me registre en el broker
const char* CodePass = "draimy123";                         // ingreso la contraseña con la que que me registre en el broker


//------------------------OTRAS VARIABLES VARIABLES-----------------------------

AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2SNoDAC *out;
                          
WiFiClient espClient;
PubSubClient client(espClient);
String _topic;
String _payload;

String strPulsador;                                                         // Indica si el amplificador esta apagado o encendido
String strvumetro;                                                          // Indica si el amplificador esta apagado o encendido
int newButtonState= 0;                                                      // Indica el estado actual del botón de encendido y apagado del amplificador
int oldButtonState= 0;                                                      // Indica el estado anterior del botón de encendido y apagado delamplificador
int ampliState= 1;                                                          // Indica el encedido y apagado del amplificador
int fanState= 1; 
unsigned long previousMillis2 = 0;                                                         // Indica el encedido y apagado del amplificador


int Vo;                                                                     // Es la tension leida en la entrada A0
float R1 = 100;                                                             // Indica el valor la resistencia  en serie a la resistencia NTC
float logR2, R2, TEMPERATURA;                                               // R2 es el valor la resistencia NTC y temperatura la variable a medir
float c1 = 2.478490884e-03, c2 = 1.623993107e-04, c3 = 13.07015263e-07;     // Constantes de steinhart-hart
unsigned long previousMillis = 0;                                           // Esta variable reincia el tiempo de la lectura de la temperatura a 0
String strventilador;                                                       // Indica si el ventilador esta apagado o encendido
                                                                            
 

//----------------------------CONEXIÓN AL WIFI---------------------------------------------------------
//Esta Funcion se encarga de conectarnos a la red wifi

void setup_wifi() {
  delay(10);                                        // Espero 10ms                                    
  Serial.println();                                 // Comenzamos a coectarnos al wifi
  Serial.print("\nConectando a ");                  // Imprimo conectado a
  Serial.println(ssid);                             // imprimo en nombre de la red wifi
  WiFi.begin(ssid, password);                       // comienzo a conectarme al broker

  while (WiFi.status() != WL_CONNECTED) {           // Mientras no este conectado al broker 
   delay(10);
   audio();                                         // Preparo la reproducción del audio
   PushButton();                                    // Ver si se precionó el boton de encendido
  }
  Serial.println("");                               // Si ya realice la conexión 
  Serial.println("\nConectado al WiFi");            // Imprimir conectado al wifi
  Serial.println("\n Direccion IP : ");             // Imprimo la direccion Ip del dispositivo conectado
  Serial.println(WiFi.localIP());                   // 
}


//------------------------ENCENDIDO Y APAGADO DEL AMPLIFICADOR---------------------------------------------
//Esta Funcion se encarga de detectar si se ha presionado el botón de encendido y apagado del amplificador
//y avisar al cliente si se encuentra apagado o encedido

    void PushButton() {
    newButtonState=digitalRead(D5);                                      // Guardo el valor del pin D5
    if(newButtonState != oldButtonState){                                // Si el valor es distinto al valor anterior
    oldButtonState = newButtonState;                                     // Guardo el nuevo 
    if (newButtonState == 0) {                                           // Si el valor nuevo es igual a 0
    ampliState = !ampliState;                                            // Negar el estado (ON/OFF) del amplificador
    if (ampliState == 0) {                                               // Si el el estado es 0
    strPulsador = "encendido";                                           // Indico que el amplificador esta encendido
    file = new AudioFileSourcePROGMEM( encendido, sizeof(encendido) );   // Ejecuto el audio de encendido 
    out = new AudioOutputI2SNoDAC();                                     // 
    wav = new AudioGeneratorWAV();                                       //     
    wav->begin(file, out);                                               //
    digitalWrite(D7, ampliState);                                        // Encendemos o apagamos el amplificaodor 
} 
    else {
    strPulsador = "apagado";                                             // Sino indico que el amplificador esta apagado
    file = new AudioFileSourcePROGMEM( apagado, sizeof(apagado) );     // ejecuto el audio de encendido   
    out = new AudioOutputI2SNoDAC();                                       //   
    wav = new AudioGeneratorWAV();                                         //
    wav->begin(file, out);                                                 //
    audio();    
}
     
    client.publish("pulsador",String(strPulsador).c_str());              // Aviso al cliente si el amplificdor esta encendido
    Serial.println("\nEnviando: ");                                      // Imprimo que se envio el mensaje al cliente
    Serial.println(strPulsador);
    digitalWrite(D4, HIGH);                                              // Apagamos el ventilador
    delay(1000);  
  }
  }    
}

//---------------------------------------------------CONECTAR AL BROKER-------------------------------------------------------------------------------
// Se ejecuta al perder la conexión con el broker y se encarga de reconectarnos al mismo. 

void reconnect() {
  while (!client.connected()) {                                         // Esta funcón se ejecuta cada vez que no estamos conectados al servidor MQTT
    Serial.println("\nIntentando conectar al servidor MQTT... ");       // Mientras no se ejecute,imprimir  Intentando conectar al servidor MQTT...        
    if (client.connect(Id,User,CodePass)) {
    } else {
      audio();                                                          // Sino preparar el audio de encendido 
      PushButton();                                                     // Ver si se ha precionado el botón de encendido
      delay(10);                                                                 
    }
      Serial.println("\nconnectado");                                   // Si logramos conectarnos idicar que nos conectamos al servidor
      client.subscribe("Amplificador");                                 // Subcribirnos a al topico Amplificador
      client.subscribe("Vumetro");                                      // Subcribirnos a al topico Vumetro   
  }
}

//------------------------RECEPCION Y ENVIO DE DATOS-----------------------------
// Se ejecuta al enviar o recibir un mensaje del cliente y le indicará al ESP8266
// las intrucciones a seguir

  void callback(char* topic, byte* payload, unsigned int length) {
  String conc_payload_;
  for (int i = 0; i < length; i++) {
    conc_payload_  += (char)payload[i];
  }
  _topic = topic;                                                          // Permite utilizar topico dentro del bucle loop
  _payload = conc_payload_;                                                // Perimite utilizar el payload dentro del bucle loop


    if (_topic == "Amplificador") {                                        // Si el topico es Amplificador 
    if (_payload == "encender") {                                          // Y el payload es encender
    digitalWrite(D7, LOW);                                                 // Encender amplificador
    ampliState=LOW;                                                        // Indico que se cambio el estado del amplificador
    delay(1000);                                                           // Espero 1 segundo  
    file = new AudioFileSourcePROGMEM( encendido, sizeof(encendido) );     // ejecuto el audio de encendido   
    out = new AudioOutputI2SNoDAC();                                       //   
    wav = new AudioGeneratorWAV();                                         //
    wav->begin(file, out);                                                 //
  }
    
    if (_payload == "apagar") {                                            // Si el payload es apagar  
    file = new AudioFileSourcePROGMEM( apagado, sizeof(apagado) );     // ejecuto el audio de encendido   
    out = new AudioOutputI2SNoDAC();                                       //   
    wav = new AudioGeneratorWAV();                                         //
    wav->begin(file, out);                                                 //
    delay(1000);  
   
    ampliState=HIGH;                                                       // Indico que se cambio el estado del amplificador
    
  }

    if (ampliState == 0) {                                                 // Si el amplificador esta encendido
    strPulsador = "encendido";                                             // variable srtpulsador igual a encendido
  } 
    else {
    strPulsador = "apagado";                                               // Sino variable srtpulsador igual apagado
  }
    client.publish("pulsador",String(strPulsador).c_str());                // Aviso al cliente si el amplificdor esta encendido
    Serial.println("\nEnviando: ");                                        // Imprimo que se envio el mensaje alcliente
    Serial.println(strPulsador);
    
    Serial.print("\nmensaje recibido de [");                               // Indico que se recibio un mensjae
    Serial.print(_topic);
    Serial.println("] ");
    Serial.println(_payload);
  }


    if (_topic == "Vumetro") {                                       // Si el topico es Vumetro
    if (_payload == "encender") {                                    // Y el payload es encender
    digitalWrite(D6, HIGH);                                          // Encender Vumetro
    strvumetro = "encendido";                                        // Indico que se cambió el estado del amplificador
    }
    if (_payload == "apagar") {                                      // Si el payload es apagar
    digitalWrite(D6, LOW);                                           // Encender Vumetro
     strvumetro = "apagado";                                         // Sino variable srtpulsador igual no presuionado
    }

    client.publish("vumetro/on/off",String(strvumetro).c_str());     // Aviso al cliente si el vúmetro esta encendido
    Serial.println("\nEnviando: ");                                  // Imprimo que se envio el mensaje alcliente
    Serial.println(strPulsador);
 
    Serial.print("\nmensaje recibido de [");                         // Indicar que se recibio un mensjae
    Serial.print(_topic);
    Serial.println("] ");
    Serial.println(_payload);
    }    
}

//------------------------------------------SETUP---------------------------------------------------------------
//Se ejecuta al inicio del progarama y se encarga de configurar los pines del ESP8266 conectarnos a la red wifi,
//conectarnos al broker y comenzar el envio y recepción de datos

void setup() {
  pinMode(D4, OUTPUT);                                                     // Configuramos D2  como salida digital
  digitalWrite(D4, HIGH);                                                  // Apagamos el ventilador
  pinMode(D2, OUTPUT);                                                     // Configuramos D2  como salida digital
  digitalWrite(D7, LOW);                                                   // Apagamos el ventilador
  pinMode(D7, OUTPUT);                                                     // Configuramos D7  como salida digital
  digitalWrite(D7, HIGH);                                                  // Apagamos el amplificador
  pinMode(D6, OUTPUT);                                                     // Configuramos D6 como salida digital
  digitalWrite(D6, LOW);                                                   // Apagamos el vúmetro
  pinMode(D5, INPUT);                                                      // Configuramos el pin D5 como entrada
  digitalWrite(BUILTIN_LED, HIGH);                                         // Apagamos el led que viene por defecto en el ESP8266
  Serial.begin(115200);                                                    // Configuramos el puerto serie  115200 baudios
  delay(1000);

  audioLogger = &Serial;                                                   // Indico las variables para ejecutar el audio 
  file = new AudioFileSourcePROGMEM( encendido, sizeof(encendido) );       //
  file = new AudioFileSourcePROGMEM( apagado, sizeof(apagado) );           //
  file = new AudioFileSourcePROGMEM( calentamiento, sizeof(calentamiento) );           //
  out = new AudioOutputI2SNoDAC();                                         //
  wav = new AudioGeneratorWAV();                                           //
  
  setup_wifi();                                                            // Llamamos a la funsion setup_wifi para conectarnos a la red wifi
  client.setServer(mqtt_server, 1883);                                     // Nos conectamos al servidor MQTT
  client.setCallback(callback);                                            // Inicia el callback para recibir y enviar un mensaje 
}


//-----------------------------------------------audio----------------------------------------------------------
// Esta función debe ejecutarse justo antes de ejecutar un audio y se encarga de parar la ejecucion del audio
// una vez finalizado su reproducción 

void audio()
{
   if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
  } else { 
    audioLogger = &Serial;
    if (ampliState == 1) {                                                 // Si el amplificador esta encendido
    digitalWrite(D7, HIGH);                                                // Apagar amplificador
  } 
  }
}


//-----------------------------------------------LOOP----------------------------------------------------------

void loop() {
  
  if (!client.connected()) {
    reconnect();                                                   // Si el cliente no esta conectado ejecutar la funcion reconect
  }
  client.loop();                                                   // Si esta conectado ejecutar bucle 
  audio();  
  PushButton();                                                    // Ver si se a precionado el bóton de encendido
  digitalWrite(D4, HIGH);                                                  // Apagamos el ventilador

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 4000) {                     // Envia la temperatura solo si han pasado 4ms
  previousMillis = currentMillis;                                   // Indico que ya pasaron 10 segundos
  Vo = analogRead(A0);                                              // Lectura de A0
  R2 = R1 * (1023.0 / (float)Vo - 1.0);                             // Conversion de tension a resistencia
  logR2 = log(R2);                                                  // Logaritmo de R2 necesario para ecuacion
  TEMPERATURA = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));     // Igualo la ecuacion steinhart-hart a la variable temperatura
  TEMPERATURA = TEMPERATURA - 275.15;                               // Convieto de Kelvin a Centigrados    
  if (TEMPERATURA >= 45) {                                          // Si la temperatura es mayor o igual a 45°
  digitalWrite(D2, HIGH);                                           // Enciendo el ventilador    
  strventilador = "encendido";                                      // Indico que se cambió el estado del ventilador 
   if (fanState == HIGH) {                                                 // Si el amplificador esta encendido
   file = new AudioFileSourcePROGMEM( calentamiento, sizeof(calentamiento) );    // ejecuto el audio de encendido   
   out = new AudioOutputI2SNoDAC();                                   //   
   wav = new AudioGeneratorWAV();                                         //
   wav->begin(file, out);   
   fanState=LOW;
  }                           
  }
  else {
  digitalWrite(D2, LOW);                                            // Apago el ventilador   
  strventilador = "apagado";                                        // Indico que se cambió el estado del ventilador 
  fanState=HIGH;
  }
  client.publish("ventilador",String(strventilador).c_str());       // Aviso al cliente que hay un sobrecalentamiento
  Serial.println("\nEnviando: ");                                   // Imprimo que se envio el mensaje al cliente
  Serial.println(strventilador);  
  
  client.publish("\ntemperatura",String(TEMPERATURA).c_str());      // Envio el valor de temperatura al cliente
  Serial.print(F("\nenviando temperatura a "));                     // Indico que el valor de temperatura se envio
  Serial.print(TEMPERATURA);
  Serial.println("...");
  delay(10);                                                        // Esperrar 10ms
  }
}
