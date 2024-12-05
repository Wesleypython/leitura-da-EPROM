// Definições de pinos
#define DATA 2    // Saída de Dados Serial
#define DCLK 4    // Clock Serial Bidirecional
#define OE 8      // Enable de Saída
#define nCS 9     // Chip Select (ativo baixo)
// Essas pinagens comentadas são para testes, programar...  
// #define TDI 11    // Entrada JTAG (não usado para leitura serial)
// #define TDO 1     // Saída JTAG (não usado para leitura serial)
// #define TMS 19    // Modo JTAG (não usado para leitura serial)
// #define TCK 3     // Relógio JTAG (não usado para leitura serial)


// Constante de atraso
const int T = 20; // Atraso em microssegundos

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Aguarda a conexão do monitor serial (para placas como Leonardo/Micro)
  }

  Serial.println("EEPROM leitura inicializada");

  //Configuração de pinos
  pinMode(DATA, INPUT);    // Dados como entrada
  pinMode(DLCK, OUTPUT);   // Clock como saída
  pinMode(OE, OUTPUT);     // Habilitação de saída
  pinMode(nCS, OUTPUT);    // Seleção de Chip

  // Inicialização de estados
  digitalWrite(DCLK, LOW); // Clock em estado baixo
  digitalWrite(OE, HIGH);  // Habilitação ativa
  digitalWrite(nCS, HIGH); // Chip desativado
  Serial.println(" Leitura de EEPROM . Inicia com 'r' para começar a ler.");
}

void loop() {
  // Escuta comandos do monitor serial
  if (Serial.available() > 0) // se a serial estiver disponível {
    char command = Serial.read();

    if (command == 'r')
    {
      Serial.println("Iniciando a leitura da EPROM...");
      readAllEEPROM(0xFFFF); // Lê todos os endereços
      Serial.println("EEPROM Read Complete.");
    } else {
      Serial.println("Un"know"n Command. Type 'r' to read EEPROM.");
      // Senão = Comando desconhecido. Digite 'r' para ler a EEPROM
    }
  }
}

// Lê todos os bytes da EEPROM
void readAllEEPROM(unsigned int maxAddress) {
  for (unsigned int address = 0; address <= maxAddress; address++) 
  {
    byte data = readData(address); //byte data recebe o valor do seu endereço/ CHAMANDO A função readData(address);
    Serial.print("Address 0x");
    Serial.print(address, HEX);
    Serial.print(": ");
    Serial.println(data, BIN); // Exibe o dado em binário
    delay(10);
  }
}

// Define o endereço e lê um byte da EEPROM  ( lê o bite após )


byte readData(unsigned int address) // byte recebe a leitura
{
  digitalWrite(nCS, LOW); // Habilita o chip
  serWrite((0x03 << 16) | address, 24); //pega a função: void serWrite; Envia comando de leitura, 0x03 << 16 desloca para a esquerda, os bits do valor 0x03 (em binário, 00000000 00000011)
  // fica assim 00000000 00000011 00000000 de um número de 24 bits, os bits do valor 0x03 (em binário, 00000000 00000011).
  byte data = serRead(); // Lê o dado achando o endereço ele já chama a função.   
  digitalWrite(nCS, HIGH); // Desabilita o chip
  return data;
}

// Envia dados serialmente para a EEPROM
void serWrite(unsigned long data, byte bits) {
  while (bits > 0) 
  {
    bits--;
    digitalWrite(DATA, (data & 0x800000) ? HIGH : LOW); // Bit mais significativo
    data <<= 1;
    delayMicroseconds(T / 4);
    digitalWrite(DCLK, HIGH);
    delayMicroseconds(T / 2);
    digitalWrite(DCLK, LOW);
    delayMicroseconds(T / 4);
  }
}

// Lê um byte serialmente da EEPROM
byte serRead() // byte recebe a leitura
 {
  byte data = 0;
  for (byte i = 0; i < 8; i++) {
    digitalWrite(DCLK, HIGH);
    delayMicroseconds(T / 4);
    data = (data << 1) | digitalRead(DATA);
    digitalWrite(DCLK, LOW);
    delayMicroseconds(T / 4);
  }
  return data;
}
