#include "DFPlayerMini_Fast.h"
#include <SoftwareSerial.h>

#define CHECK 12          // Pin del botón de verificación (GPIO 12)
#define BUTTON_1 13       // Primer botón Braille (GPIO 13)
#define BUTTON_2 14       // Segundo botón Braille (GPIO 14)
#define BUTTON_3 15       // Tercer botón Braille (GPIO 15)
#define BUTTON_4 5        // Cuarto botón Braille (GPIO 5)
#define BUTTON_5 18       // Quinto botón Braille (GPIO 18)
#define BUTTON_6 19       // Sexto botón Braille (GPIO 19)

#define BRAILLE_BTNS 6

// Abecedario en Braille
int alphabet[42][7] = {
    {1, 0, 0, 0, 0, 0, 'A'}, {1, 1, 0, 0, 0, 0, 'B'}, {1, 0, 0, 1, 0, 0, 'C'},
    {1, 0, 0, 1, 1, 0, 'D'}, {1, 0, 0, 0, 1, 0, 'E'}, {1, 1, 0, 1, 0, 0, 'F'},
    {1, 1, 0, 1, 1, 0, 'G'}, {1, 1, 0, 0, 1, 0, 'H'}, {0, 1, 0, 1, 0, 0, 'I'},
    {0, 1, 0, 1, 1, 0, 'J'}, {1, 0, 1, 0, 0, 0, 'K'}, {1, 1, 1, 0, 0, 0, 'L'},
    {1, 0, 1, 1, 0, 0, 'M'}, {1, 0, 1, 1, 1, 0, 'N'}, {1, 0, 1, 0, 1, 0, 'O'},
    {1, 1, 1, 1, 0, 0, 'P'}, {1, 1, 1, 1, 1, 0, 'Q'}, {1, 1, 1, 0, 1, 0, 'R'},
    {0, 1, 1, 1, 0, 0, 'S'}, {0, 1, 1, 1, 1, 0, 'T'}, {1, 0, 1, 0, 0, 1, 'U'},
    {1, 1, 1, 0, 0, 1, 'V'}, {0, 1, 0, 1, 1, 1, 'W'}, {1, 0, 1, 1, 0, 1, 'X'},
    {1, 0, 1, 1, 1, 1, 'Y'}, {1, 0, 1, 0, 1, 1, 'Z'}, {1, 1, 0, 1, 1, 1 , 'Ñ'},
    {0, 0, 1, 0, 0, 0, '.'}, {0, 1, 0, 0, 0, 0, ','}, {0, 1, 1, 0, 0, 0, ';'},
    {0, 1, 0, 0, 1, 0, ':'}, {0, 0, 1, 0, 0, 1, '-'}, {0, 1, 0, 0, 0, 1, '¿?'},
    {0, 1, 1, 0, 1, 0, '¡!'}, {0, 1, 1, 0, 0, 1, '""'},{1, 1, 0, 0, 0, 1, '('},
    {0, 0, 1, 1, 1, 0, ')'}, {1, 0, 1, 0, 1, 1, 'á'}, {1, 1, 1, 0, 1, 1, 'é'},
    {0, 1, 1, 1, 0, 1, 'í'}, {0, 0, 1, 1, 0, 0, 'ó'}, {0, 0, 1, 1, 0, 1, 'ú'},
    {0, 1, 1, 1, 1, 1, 'ü'}
};

int input[BRAILLE_BTNS] = {0, 0, 0, 0, 0, 0};

SoftwareSerial mySoftwareSerial(16, 17); // RX, TX
DFPlayerMini_Fast myDFPlayer;

void setup() {
    pinMode(CHECK, INPUT_PULLUP); // Configuración del pin CHECK con INPUT_PULLUP
    pinMode(BUTTON_1, INPUT_PULLUP); // Configuración del primer botón Braille
    pinMode(BUTTON_2, INPUT_PULLUP); // Configuración del segundo botón Braille
    pinMode(BUTTON_3, INPUT_PULLUP); // Configuración del tercer botón Braille
    pinMode(BUTTON_4, INPUT_PULLUP); // Configuración del cuarto botón Braille
    pinMode(BUTTON_5, INPUT_PULLUP); // Configuración del quinto botón Braille
    pinMode(BUTTON_6, INPUT_PULLUP); // Configuración del sexto botón Braille

    Serial.begin(9600);
    mySoftwareSerial.begin(9600);
    myDFPlayer.begin(mySoftwareSerial);

    myDFPlayer.volume(20); // Configurar el volumen
}

void loop() {
    if (digitalRead(CHECK) == LOW) {
        delay(500);
        readInput();
        findAndPlayCharacter();
    }
}

void readInput() {
    Serial.print("[In] Entrada del teclado:\t\t");
    input[0] = !digitalRead(BUTTON_1);
    input[1] = !digitalRead(BUTTON_2);
    input[2] = !digitalRead(BUTTON_3);
    input[3] = !digitalRead(BUTTON_4);
    input[4] = !digitalRead(BUTTON_5);
    input[5] = !digitalRead(BUTTON_6);

    for (int i = 0; i < BRAILLE_BTNS; i++) {
        Serial.print(input[i]);
    }
    Serial.println(';');
}

void findAndPlayCharacter() {
    for (int i = 0; i < 42; i++) {
        if (inputMatches(i)) {
            Serial.print("[Out] Caracter correspondiente:\t");
            Serial.print((char)alphabet[i][6]);
            Serial.println(";");
            myDFPlayer.play(i + 1); // Reproduce archivo correspondiente
            return;
        }
    }
    Serial.println("[!] Mensaje de error: Caracter no encontrado.");
    myDFPlayer.play(43); // Reproduce archivo de error, asumiendo que el archivo de error es 043.mp3
}

bool inputMatches(int index) {
    for (int j = 0; j < BRAILLE_BTNS; j++) {
        if (alphabet[index][j] != input[j]) {
            return false;
        }
    }
    return true;
}
