 //incluindo as bibliotecas do primeiro teste.
#include <FirebaseArduino.h> //Biblioteca de comunicação entre a ide e o firebase
#include <ESP8266WiFi.h> //Biblioteca expecifica para as placas micro-controladoras ESP8266 
#include <Ticker.h> //Biblioteca responsavel por publicar os dados coletados no nodeMCU para o firebase

//Definição dos dados do firebase.
#define FIREBASE_HOST "teste-69708.firebaseio.com"    //Locar onde recebe os dados da aplicação.
#define FIREBASE_AUTH "OIVJ8Xv2SVvc6XeVQaLVjiqVF89jJf2NfUnEwV8X"    //Atenticando no servidor do firebase.

//Definição dos dados da rede local
#define WIFI_SSID "FATEC LABs" //nome da rede wifi.
#define WIFI_PASSWORD "ceetepspg"  //senha de rede wifi.

// Definiçao para o tempo de interevalo do envio dos dados
#define PUBLISH_INTERVAL 1000*60*5 

//sensor umidade
#define pino_sinal_analogico A0 
int valor_analogico;
 
//sensor de chuva
#define pino_sinal_digital D0
int valor_digital;

//Utilizando a biblioteca <Ticker.h>
Ticker ticker;
bool publishNewState = true; //Valor True para poder começar a publicação dos dados, caso o contrario o valor o programa não publica os dados

//funcao de publicacao
void publish(){
  publishNewState = true;
}

void setup() {
// leitura pino A0 - umidade solo
pinMode(pino_sinal_analogico, INPUT);
// leitura pino D0 - chuva
pinMode(pino_sinal_digital, INPUT);
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //enviando o nome da rede ea senha  para conectar.
  Serial.print("connecting"); //mensagem caso a conecção de certo.
  while (WiFi.status() != WL_CONNECTED) { //caso a de certo..
    Serial.print(".");  //mostra um . para sinalizar que esta carregando
    delay(5000); //5 segundos para cada ponto
  }
  Serial.println();
  Serial.print("connected: ");  //mostra que conectou
  Serial.println(WiFi.localIP()); //mostrando o ip do esp8266 na rede
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //enviando os dados do firebase pela rede ja conectada 
  // Registra o ticker para publicar de tempos em tempos
  ticker.attach_ms(PUBLISH_INTERVAL, publish); //Enviando os dados do coletados para o firebase guarda
}

void loop() {
  //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
  //Le o valor do pino A0 do sensor
  valor_digital = analogRead(pino_sinal_digital);
  // Apenas publique quando passar o tempo determinado
  if(publishNewState){
    Serial.println("Publish new State");
    // Obtem os dados do sensor DHT 
    //float humidity = ;
    if(!isnan(valor_analogico)){
      // Manda sinal umidade para o FB
      StaticJsonBuffer<200> jsonBuffer; // Instacio a ultilização da classe jsonBuffer; 
      JsonObject& root = jsonBuffer.createObject(); // criando um objeto do memso tipo da classe para ser salvo no firebase
      //neste momento uma key(chave) foi criadano firebase, nela vai conter um arquivo no formato json.
      root["umidade"] = valor_analogico; // root umidade sera o primeiro valor
      root["chuva"] = valor_digital;  // root chuva sera o segundo valor
      // append a new value to /logDHT
      String name = Firebase.push("sensor", root); // aqui os dados do root são enviados
      Serial.println("Valor umidade:"); 
      Serial.println(valor_analogico); // exibindo o valor na serial
      Serial.println("Valor chuva:");
      Serial.println(valor_digital); // exibindo o valor na serial
    }else{
      Serial.println("Error Publishing"); //caso ocorra alguns erro para publicar o dado sera apresentada uma mensagem na saida serial
    }
  }
  delay(5000); // de quanto em quanto tempo sera feita a leitura do dados e o envio do mesmo
  // ----- Fim
}
