#include <SPI.h>
#include <SD.h>
#include <DHT.h>

// --- Configurações de pinos ---
#define DHTPIN        4
#define DHTTYPE       DHT11
const int pinoLDR         = A0;
const int pinoSolo        = A1;
const int bombaPin        = 7;
const int csPin           = 10;   // Chip Select do SD
const int ledPin          = 8;    // Led ligado com a leitura do sensor LDR
const int threshold       = 500;  // limiar para acender o LED

DHT dht(DHTPIN, DHTTYPE);
File dataFile;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(bombaPin, OUTPUT);

  // Inicializa SD
  if (!SD.begin(csPin)) {
    Serial.println("Erro ao inicializar SD");
    while (1);
  }
  Serial.println("SD iniciado");

  // Cria cabeçalho no CSV se não existir
  if (!SD.exists("datalog.csv")) {
    dataFile = SD.open("datalog.csv", FILE_WRITE);
    dataFile.println("timestamp_ms,ldr,umid_solo,umid_ar,temperatura");
    dataFile.close();
  }
}

void loop() {
  unsigned long t = millis();
  int leituraLDR   = analogRead(pinoLDR);
  int umidSolo     = analogRead(pinoSolo);
  float h          = dht.readHumidity();
  float temp       = dht.readTemperature();

  if(threshold < 500){
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin, LOW);
  }

  // Aciona bomba conforme solo (exemplo)
  if (umidSolo > 800) digitalWrite(bombaPin, HIGH);
  else digitalWrite(bombaPin, LOW);

  // Abre arquivo e registra dados
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(t);
    dataFile.print(',');
    dataFile.print(leituraLDR);
    dataFile.print(',');
    dataFile.print(umidSolo);
    dataFile.print(',');
    if (isnan(h) || isnan(temp)) {
      dataFile.print("ERR,ERR");
    } else {
      dataFile.print(h);
      dataFile.print(',');
      dataFile.print(temp);
    }
    dataFile.println();
    dataFile.close();
    Serial.println("Registro gravado");
  } else {
    Serial.println("Erro abrindo datalog.csv");
  }

  delay(2000);
}
