#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// --- Pinos de Conexão ---
#define DHTPIN 4
#define DHTTYPE DHT11

// --- INICIALIZAÇÃO DOS OBJETOS ---
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C, Colunas, Linhas
DHT dht(4, DHTTYPE); // Pino 4, Tipo DHT11

// --- Pârametros que VOCÊ PRECISA AJUSTAR ---
const float VOLUME_VASO_ML = 2000.0; // Volume total do seu vaso em mililitros (ex: 2000 para um vaso de 2L)
const float VAZAO_BOMBA_ML_POR_SEGUNDO = 50.0; // Vazão da bomba (mL/s). MEÇA ISSO!

const int pinoLDR = A2;
const int pinoSolo = A1;
const int ledPin = 8;     // LED de luz artificial
const int pinoBomba = 30;   // Pino do relé da bomba

// --- Constantes de Lógica e Tempo ---
const int limiarSoloSeco = 750;       // Valor do sensor que indica solo seco. AJUSTE CONFORME SEU SENSOR!
const int limiarLuz = 500;            // Limiar do LDR para considerar "luz baixa"
const float TEMP_CRITICA = 30.0;      // Temperatura acima da qual a irrigação é maior
const unsigned long INTERVALO_MINIMO_IRRIGACAO = 24 * 3600 * 1000UL; // 24 horas em milissegundos
const unsigned long PERIODO_DIA_MS = 12 * 3600 * 1000UL;  // 12 horas de "dia"
const unsigned long PERIODO_NOITE_MS = 12 * 3600 * 1000UL; // 12 horas de "noite"

// --- Variáveis Globais de Controle de Estado ---
unsigned long tempoDaUltimaIrrigacao = 0;
bool podeRegar = true; // Flag para controlar se a primeira irrigação pode ocorrer
unsigned long tempoAnteriorLCD = 0;
int telaAtual = 1;
unsigned long inicioCicloLuz = 0; // Armazena o início do ciclo de luz atual
bool ePeriodoDeDia = true;     // Controla se está no período de "dia" ou "noite"

// Cria o caractere customizado para o símbolo de grau (°)
byte grau[8] = {0b01100, 0b10010, 0b10010, 0b01100, 0b00000, 0b00000, 0b00000, 0b00000};

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando AutoPlantCare (sem RTC)...");

  // Configura os pinos
  pinMode(ledPin, OUTPUT);
  pinMode(pinoBomba, OUTPUT);
  digitalWrite(pinoBomba, HIGH); // Garante que a bomba comece desligada

  // Inicializa o Display LCD
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, grau);
  lcd.print("AutoPlantCare");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  // Inicializa o sensor DHT11
  dht.begin();
  delay(2000);
  lcd.clear();
  
  // Inicia o timer da irrigação "negativo" para permitir a primeira rega se necessário
  tempoDaUltimaIrrigacao = -INTERVALO_MINIMO_IRRIGACAO;

  // Inicia o primeiro ciclo de luz como "dia"
  inicioCicloLuz = millis();
}

void loop() {
  // --- LEITURAS E DADOS BÁSICOS ---
  float umidadeAr = dht.readHumidity();
  float temperatura = dht.readTemperature();
  int umidadeSolo = analogRead(pinoSolo);
  int luminosidade = analogRead(pinoLDR);

  // --- LÓGICA DE IRRIGAÇÃO (baseada em intervalo) ---
  // Verifica se já se passaram 24h desde a última rega
  if (millis() - tempoDaUltimaIrrigacao >= INTERVALO_MINIMO_IRRIGACAO) {
    podeRegar = true; // Libera a verificação para regar novamente
  }
  
  // Se o solo estiver seco E a verificação estiver liberada...
  if (umidadeSolo > limiarSoloSeco && podeRegar) {
    Serial.println("Solo seco e intervalo permitido. Verificando irrigacao...");
    
    // Calcula a quantidade de água necessária
    float percentualAgua = (temperatura >= TEMP_CRITICA) ? 0.10 : 0.05; // 10% ou 5%
    float volumeAguaNecessario = VOLUME_VASO_ML * percentualAgua;
    long tempoDeBombaLigada = (volumeAguaNecessario / VAZAO_BOMBA_ML_POR_SEGUNDO) * 1000.0;

    Serial.print("Irrigando com ");
    Serial.print(volumeAguaNecessario);
    Serial.println(" ml.");
    
    // Executa a irrigação
    digitalWrite(pinoBomba, LOW);
    delay(tempoDeBombaLigada);
    digitalWrite(pinoBomba, HIGH);
    
    Serial.println("Irrigacao concluida. Aguardando 24h para proxima.");

    // Reseta o timer e bloqueia novas regas pelas próximas 24h
    tempoDaUltimaIrrigacao = millis();
    podeRegar = false;
  }

  // --- LÓGICA DE CICLO DE LUZ (Dia/Noite simulado) ---
  if (ePeriodoDeDia && (millis() - inicioCicloLuz >= PERIODO_DIA_MS)) {
    // Terminou o dia, começa a noite
    ePeriodoDeDia = false;
    inicioCicloLuz = millis(); // Reseta o timer para o início da noite
    Serial.println("Iniciando periodo de noite (luz artificial desligada).");
  } else if (!ePeriodoDeDia && (millis() - inicioCicloLuz >= PERIODO_NOITE_MS)) {
    // Terminou a noite, começa o dia
    ePeriodoDeDia = true;
    inicioCicloLuz = millis(); // Reseta o timer para o início do dia
    Serial.println("Iniciando periodo de dia (luz artificial disponivel).");
  }

  // Controle da luz artificial baseado no ciclo
  if (ePeriodoDeDia) {
    // Durante o dia, a luz complementa a falta de luz natural
    if (luminosidade < limiarLuz) {
        digitalWrite(ledPin, HIGH);
    } else {
        digitalWrite(ledPin, LOW);
    }
  } else {
    // Durante a noite, a luz fica sempre desligada para a planta descansar
    digitalWrite(ledPin, LOW);
  }


  // --- ATUALIZAÇÃO DO DISPLAY LCD (a cada 4 segundos) ---
  if (millis() - tempoAnteriorLCD > 4000) {
    tempoAnteriorLCD = millis();
    lcd.clear();
    
    String estadoSolo = (umidadeSolo > limiarSoloSeco) ? "Seco!" : (umidadeSolo > 400) ? "Umido" : "Encharcado";
    
    if (telaAtual == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Temp: " + String(temperatura, 1));
      lcd.write(byte(0));
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Umid.Ar: " + String(umidadeAr, 0) + "%");
      telaAtual = 2;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Solo: " + estadoSolo);
      lcd.setCursor(0, 1);
      String statusLuz = (digitalRead(ledPin) == HIGH) ? "Ligada" : "Apagada";
      String statusCiclo = ePeriodoDeDia ? "(Dia)" : "(Noite)";
      lcd.print("Luz: " + statusLuz + " " + statusCiclo);
      telaAtual = 1;
    }
  }
}
