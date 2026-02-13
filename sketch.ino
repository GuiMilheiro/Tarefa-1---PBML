#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <EEPROM.h>

//Felipe Hideki RM98323
//Guilherme Milheiro RM550295
//Jhonatan Curci RM94188
//Enzo Vasconcelos RM550702
//Ricardo Queiroz RM94241

/* --- MAPA DE CONEXÕES (HARDWARE) ---
   LCD RS: Pino 12
   LCD E:  Pino 11
   LCD D4: Pino 10
   LCD D5: Pino 9
   LCD D6: Pino 8
   LCD D7: Pino 7
   LCD R/W e V0: GND
   
   Keypad Linhas (1-4): Pinos 5, 4, 3, 2
   Keypad Colunas (1-4): Pinos A0, A1, A2, A3
   
   Servo Motor: Pino 6
   Buzzer: Pino 13
*/

// --- CONFIGURAÇÃO DO LCD ---
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

// --- CONFIGURAÇÃO DO TECLADO 4x4 ---
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {A0, A1, A2, A3}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- CONFIGURAÇÃO DOS ATUADORES ---
Servo myServo;
const int PIN_SERVO = 6;
const int PIN_BUZZER = 13;

// --- VARIÁVEIS GLOBAIS ---
String currentPassword = ""; // Senha sendo digitada
String savedPassword = "";   // Senha gravada na memória
int errorCount = 0;          // Contador de erros
bool isLocked = true;        // Estado do cofre

void setup() {
  // Inicializa LCD
  lcd.begin(16, 2);
  
  // Inicializa Servo e Buzzer
  myServo.attach(PIN_SERVO);
  pinMode(PIN_BUZZER, OUTPUT);

  // --- LÓGICA DA MEMÓRIA EEPROM ---
  // Se a memória estiver virgem (valor 255), grava a senha padrão "1234"
  if (EEPROM.read(0) == 255) {
    savePasswordToEEPROM("1234");
  }
  // Lê a senha salva na memória
  savedPassword = readPasswordFromEEPROM();

  // Estado inicial: Trancado
  lockSafe();
  
  // Mensagem de boas-vindas
  lcd.clear();
  lcd.print("Cofre SYSTEM");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
  displayPrompt();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    beep(50); // Barulhinho de tecla pressionada

    if (key == '#') {
      // '#' confirma a senha
      verifyPassword();
    } 
    else if (key == '*') {
      // '*' apaga o que foi digitado
      currentPassword = "";
      displayPrompt();
    }
    else if (key == 'A') {
      // 'A' tranca o cofre manualmente (se estiver aberto)
      if (!isLocked) {
        lockSafe();
      }
    }
    else {
      // Acumula os números digitados (máximo 4)
      if (currentPassword.length() < 4) {
        currentPassword += key;
        updateDisplay();
      }
    }
  }
}

// --- FUNÇÕES AUXILIARES ---

void verifyPassword() {
  lcd.clear();
  
  if (currentPassword == savedPassword) {
    lcd.print("SENHA CORRETA!");
    tone(PIN_BUZZER, 1000, 200); // Som de sucesso
    delay(200);
    tone(PIN_BUZZER, 1500, 200);
    unlockSafe();
    errorCount = 0; 
  } else {
    lcd.print("SENHA INCORRETA");
    errorCount++;
    tone(PIN_BUZZER, 200, 1000); // Som grave de erro
    delay(1500);
    
    // Se errar 3 vezes, dispara o alarme
    if (errorCount >= 3) {
      triggerAlarm();
    } else {
      displayPrompt();
    }
  }
  currentPassword = ""; 
}

void unlockSafe() {
  myServo.write(90); // Gira o servo para abrir (90 graus)
  isLocked = false;
  lcd.setCursor(0, 1);
  lcd.print("Aberto");
  delay(2000);
}

void lockSafe() {
  myServo.write(0); // Gira o servo para fechar (0 graus)
  isLocked = true;
  currentPassword = "";
  lcd.clear();
  lcd.print("Bloqueado!");
  delay(1000);
  displayPrompt();
}

void triggerAlarm() {
  lcd.clear();
  lcd.print("ALARME ATIVADO!");
  lcd.setCursor(0,1);
  lcd.print("BLOQUEIO TOTAL");
  
  // Toca sirene por 5 segundos
  for (int i = 0; i < 10; i++) {
    tone(PIN_BUZZER, 1000);
    delay(200);
    tone(PIN_BUZZER, 500);
    delay(200);
  }
  noTone(PIN_BUZZER);
  
  errorCount = 0; // Reseta o contador
  displayPrompt();
}

void displayPrompt() {
  lcd.clear();
  lcd.print("Digite a Senha:");
  lcd.setCursor(0, 1);
}

void updateDisplay() {
  lcd.setCursor(0, 1);
  // Imprime asteriscos no lugar dos números
  for (int i = 0; i < currentPassword.length(); i++) {
    lcd.print("*");
  }
}

void beep(int duration) {
  tone(PIN_BUZZER, 2000, duration);
}

// --- FUNÇÕES DE MEMÓRIA (EEPROM) ---

void savePasswordToEEPROM(String pwd) {
  for (int i = 0; i < 4; i++) {
    EEPROM.write(i, pwd[i]);
  }
}

String readPasswordFromEEPROM() {
  String pwd = "";
  for (int i = 0; i < 4; i++) {
    pwd += (char)EEPROM.read(i);
  }
  return pwd;
}