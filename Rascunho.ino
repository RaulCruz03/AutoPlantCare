#include <DHT.h>

// Configuração do DHT11
#define DHTPIN 4      // Pino digital onde o DHT11 está conectado
#define DHTTYPE DHT11   // Tipo do sensor DHT

DHT dht(DHTPIN, DHTTYPE);  // Cria o objeto do sensor

const int pinoLDR      = A2;   // Pino analógico do LDR
const int ledPin       = 8;    // Pino digital conectado ao LED
const int threshold    = 500;  // Limiar para decidir se acende o LED (ajuste conforme necessário)
const int pinoSolo     = A1;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sensores...");
  pinMode(ledPin,OUTPUT);

  dht.begin();  // Inicializa o sensor DHT11
}

void loop() {

  int umidSolo = analogRead(pinoSolo);
  Serial.print("Umidade do Solo (Valor Bruto): "); // Adicionado para clareza
  Serial.println(umidSolo);

  // --- NOVO CÓDIGO PARA ESTADO DO SOLO ---
  // Estes valores são exemplos. Você precisará calibrá-los para o seu sensor específico!
  // Geralmente, para sensores capacitivos:
  // Ar/Seco: Valores mais altos (ex: > 700-800)
  // Água/Encharcado: Valores mais baixos (ex: < 300-400)
  // Úmido: Valores intermediários
  if (umidSolo > 700) { // Exemplo para solo seco
    Serial.println("Estado do Solo: Seco");
  } else if (umidSolo > 350) { // Exemplo para solo úmido
    Serial.println("Estado do Solo: Úmido");
  } else { // Valores abaixo de 350 indicam solo encharcado
    Serial.println("Estado do Solo: Encharcado");
  }
  // --- FIM DO NOVO CÓDIGO ---

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

  delay(100); // Pequeno delay, pode ser removido se não necessário aqui

  if (isnan(h) || isnan(t)) {
    Serial.println("Erro ao ler do DHT11! Verifique conexões.");
  } else {
    Serial.print("Umidade do Ar: "); // Adicionado "do Ar" para clareza
    Serial.print(h);
    Serial.print(" %\t");

    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" °C");
  }

  // Aguarda 2 segundos antes da próxima leitura
  delay(1500);
}