# Cofre Digital com Arduino (Projeto Maker Lab)

Projeto desenvolvido para a disciplina de Maker Lab (FIAP). Trata-se de um cofre digital simulado que utiliza senha, feedback visual (LCD) e sonoro (Buzzer) para acionar uma tranca (Servo Motor).

## 👥 Integrantes do Grupo
* Felipe Hideki RM98323
* Guilherme Milheiro RM550295
* Jhonatan Curci RM94188
* Enzo Vasconcelos RM550702
* Ricardo Queiroz RM94241

## 🛠 Hardware Utilizado
* Arduino Uno
* LCD 16x2 (I2C ou Paralelo)
* Teclado Matricial 4x4
* Servo Motor
* Buzzer Piezoelétrico
* Potenciômetro (para contraste do LCD)

## ⚙️ Funcionalidades
1. **Senha na EEPROM:** A senha fica salva mesmo desligando o Arduino.
2. **Tranca Automática:** Servo motor gira 90º ao acertar a senha.
3. **Bloqueio de Segurança:** Após 3 tentativas erradas, o sistema dispara um alarme e bloqueia temporariamente.

## 💻 Simulação
O projeto pode ser visualizado e testado no Wokwi através deste link:
[Cole seu link do Wokwi aqui]

## 📝 Como usar
1. Inicie a simulação.
2. A senha padrão inicial é `1234`.
3. Pressione `#` para confirmar.
4. Pressione `A` para trancar novamente.
