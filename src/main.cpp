#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
// #include <DHT.h>


// #define DHTTYPE DHT11 // Define o tipo de sensor DHT11
// #define DHTPIN 4 
// DHT dht(DHTPIN, DHTTYPE); // Cria um objeto DHT
#define LED_BUILTIN 2
#define LEDvermelho 23
#define LEDverde 19
#define LEDamarelo 26

const char* ssid = "DiEletrons_Reuniao";
const char* password = "49433012";
const char* mqtt_server = "broker.hivemq.com"; // MQTT Broker server é um servidor que recebe todas as mensagens dos clientes e então encaminha as mensagens apropriadas para os destinatários corretos.

WiFiClient espClient; // Cria um cliente WiFi para se conectar à rede
PubSubClient client(espClient); // Cria um cliente MQTT para se conectar ao servidor MQTT
unsigned long lastMsg = 0; // Variável para armazenar o tempo da última mensagem enviada
#define MSG_BUFFER_SIZE (50) // Define o tamanho do buffer de mensagens
char msg[MSG_BUFFER_SIZE]; // Cria um buffer de mensagens


char LED;
char Sensor;

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Mudar o estado do LED com base na mensagem recebida

  if (char(payload[0]) == '1'){
    digitalWrite(LEDverde , !digitalRead(LEDverde)); 
    if(digitalRead(LEDamarelo) == LOW){
    snprintf (msg, MSG_BUFFER_SIZE, "LED Verde desligado");
    }else{
    snprintf (msg, MSG_BUFFER_SIZE, "LED Verde ligado");
    }
    snprintf (msg, MSG_BUFFER_SIZE, "LED Verde desligado");

    client.publish("Mensagens", msg); // Publica a mensagem no tópico outTopic
  } if ((char)payload[0] == '2'){
    digitalWrite(LEDvermelho , !digitalRead(LEDvermelho)); 
    if(digitalRead(LEDamarelo) == LOW){
    snprintf (msg, MSG_BUFFER_SIZE, "LED Vermelho desligado");
    }else{
    snprintf (msg, MSG_BUFFER_SIZE, "LED Vermelho ligado");
    }

    client.publish("Mensagens", msg); // Publica a mensagem no tópico outTopic
  }if (char(payload[0]) == '3'){
    digitalWrite(LEDamarelo, !digitalRead(LEDamarelo)); // ligar o LED acendendo-o com um LOW
    if(digitalRead(LEDamarelo) == LOW){
    snprintf (msg, MSG_BUFFER_SIZE, "LED Amarelo desligado");
    }else{
    snprintf (msg, MSG_BUFFER_SIZE, "LED Amarelo ligado");
    }
    client.publish("Mensagens", msg); // Publica a mensagem no tópico outTopic
  }
}

void reconnect(){
  // Loop até que estejamos reconectados
  while (!client.connected()){
    Serial.print("Tentando se conectar ao servidor MQTT...");
    // Tentar se conectar
    String clientId = "Dieletrons-";
    // clientId += String(random(0xffff), HEX);

 if(client.connect(clientId.c_str())){
      Serial.println("Conectado");
      digitalWrite(LED_BUILTIN, HIGH);
      client.subscribe("LED");
    }else{
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      // Esperar 5 segundos antes de tentar novamente
      delay(100);
    }
  }
}


void setup(){
    // Sensor = 15;
    pinMode(LED_BUILTIN, OUTPUT); // Define o pino do LED como saída
    pinMode(LEDvermelho, OUTPUT);
    pinMode(LEDamarelo, OUTPUT);
    pinMode(LEDverde, OUTPUT);
    // pinMode(Sensor, INPUT);

    Serial.begin(921600);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback); // Define a função de retorno de chamada para o cliente MQTT

if (!client.connected()) {
    reconnect();
  }
  client.loop();

}

void loop(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Alterna o estado do LED

    // float h = dht.readHumidity(); // Ler a umidade
    // float t = dht.readTemperature(); // Ler a temperatura em Celsius (o valor padrão)
    int sns = HIGH; // Lê o estado do sensor
    float h = random(1, 11);
    float t = random(1, 30);

    if (sns == HIGH){
      // Serial.println("Movimento detectado");
      snprintf (msg, MSG_BUFFER_SIZE, "Movimento detectado");
      client.publish("Sensor_de_movimento", msg);
    }else{
      // Serial.println("Nenhum movimento detectado");
      snprintf (msg, MSG_BUFFER_SIZE, "Nenhum movimento detectado");
      client.publish("Sensor_de_movimento", msg);
    }

    // Serial.print("Umidade: "); 
    // Serial.print(h);   // Imprime o valor da umidade
    // Serial.print(F("%\t")); // Imprime o valor da umidade
    sprintf(msg, "%f", h); // Converte o valor da umidade em string
    client.publish("UNIDADE", msg); // Publica o valor da umidade no tópico outTopic

    // Serial.print("Temperatura: ");
    // Serial.print(t); // Imprime o valor da temperatura
    // Serial.print(F("°C\t")); // Imprime o valor da temperatura
    sprintf(msg, "%f", t); // Converte o valor da temperatura em string
    client.publish("TEMPERATURA", msg); // Publica o valor da temperatura no tópico outTopic

    delay(2000); // Aguarda 2 segundos

    if (!client.connected()){
      reconnect();
    }
    client.loop();
}