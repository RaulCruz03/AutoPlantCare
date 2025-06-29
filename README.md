# AutoPlantCare – Sistema de Monitoramento e Cuidado Automático de Plantas #



Projeto de um sistema embarcado com Arduino Mega 2560 para monitoramento ambiental e cuidados automatizados com plantas, incluindo irrigação e controle de luz artificial.

🚀 Objetivo



Desenvolver um sistema capaz de:

Monitorar continuamente umidade do solo, temperatura, umidade do ar e luminosidade;

Atuar automaticamente para manter as plantas em boas condições;

Acionar dispositivos como bomba de água, servo motor ou LEDs de acordo com as condições ambientais;

Exibir dados em tempo real (LCD) ou armazená-los para análise (módulo SD opcional).

🧩 Componentes Principais



🔍 Sensores



Umidade do solo: YL-28 ou sensor capacitivo

Temperatura e umidade do ar: DHT11 ou DHT22

Luminosidade: LDR (fotoresistor)

⚙️ Atuadores



Bomba de água ou servo motor: Para irrigação automática

LEDs ou fita de LED: Iluminação artificial

💡 Outros



Microcontrolador: Arduino Mega 2560

Display LCD 16x2 (opcional): Visualização em tempo real

Módulo SD (opcional): Armazenamento de dados para comparação

📈 Funcionamento Geral



Monitoramento Contínuo



Leitura da umidade do solo

Monitoramento de temperatura e umidade do ar (DHT11/22)

Detecção da luminosidade ambiente via LDR

Ações Automatizadas



Acionamento da bomba ou servo quando o solo estiver seco

Ativação dos LEDs em caso de baixa luminosidade

Exibição de dados em LCD 16x2 (se disponível)

Registro dos dados no cartão SD (modo de comparação, opcional)

