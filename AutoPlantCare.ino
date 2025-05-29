#include <DHT.h>

// Configuração do DHT11
#define DHTPIN 4       // Pino digital onde o DHT11 está conectado
#define DHTTYPE DHT11  // Tipo do sensor DHT

DHT dht(DHTPIN, DHTTYPE);  // Cria o objeto do sensor

const int pinoLDR = A0;     // Pino analógico do LDR
const int ledPin = 8;       // Pino digital conectado ao LED
const int threshold = 500;  // Limiar para decidir se acende o LED (ajuste conforme necessário)
void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sensores...");
  pinMode(ledPin,OUTPUT);

  dht.begin();  // Inicializa o sensor DHT11
}

void loop() {
  // Leitura da luminosidade
  int leitura = analogRead(pinoLDR);
  Serial.print("Luminosidade: ");
  Serial.println(leitura);

  // Leitura do DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(leitura < threshold){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(100);

  if (isnan(h) || isnan(t)) {
    Serial.println("Erro ao ler do DHT11! Verifique conexões.");
  } else {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %\t");

    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" °C");
  }

  // Aguarda 2 segundos antes da próxima leitura
  delay(1500);
}

