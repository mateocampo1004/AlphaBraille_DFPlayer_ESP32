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
int alphabet[26][7] = {
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
    {0, 0, 1, 1, 1, 0, ')'}, {1, 0, 1, 0, 1, 1, ''}, {1, 1, 1, 0, 1, 1, 'á'},
    {0, 1, 1, 1, 0, 1, 'é'}, {0, 0, 1, 1, 0, 0, 'í'}, {0, 0, 1, 1, 0, 1, 'ó'},
    {0, 1, 1, 1, 1, 1, 'ú'}, {1, 1, 0, 0, 1, 1, 'ü'},

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
    if (inputMatches(0)) {
        Serial.println("[Out] Caracter correspondiente:\t'A';");
        myDFPlayer.play(1); // Reproduce archivo 001.mp3 para 'A'
    }
    else if (inputMatches(1)) {
        Serial.println("[Out] Caracter correspondiente:\t'B';");
        myDFPlayer.play(2); // Reproduce archivo 002.mp3 para 'B'
    }
    else if (inputMatches(2)) {
        Serial.println("[Out] Caracter correspondiente:\t'C';");
        myDFPlayer.play(3); // Reproduce archivo 003.mp3 para 'C'
    }
    else if (inputMatches(3)) {
        Serial.println("[Out] Caracter correspondiente:\t'D';");
        myDFPlayer.play(4); // Reproduce archivo 004.mp3 para 'D'
    }
    else if (inputMatches(4)) {
        Serial.println("[Out] Caracter correspondiente:\t'E';");
        myDFPlayer.play(5); // Reproduce archivo 005.mp3 para 'E'
    }
    else if (inputMatches(5)) {
        Serial.println("[Out] Caracter correspondiente:\t'F';");
        myDFPlayer.play(6); // Reproduce archivo 006.mp3 para 'F'
    }
    else if (inputMatches(6)) {
        Serial.println("[Out] Caracter correspondiente:\t'G';");
        myDFPlayer.play(7); // Reproduce archivo 007.mp3 para 'G'
    }
    else if (inputMatches(7)) {
        Serial.println("[Out] Caracter correspondiente:\t'H';");
        myDFPlayer.play(8); // Reproduce archivo 008.mp3 para 'H'
    }
    else if (inputMatches(8)) {
        Serial.println("[Out] Caracter correspondiente:\t'I';");
        myDFPlayer.play(9); // Reproduce archivo 009.mp3 para 'I'
    }
    else if (inputMatches(9)) {
        Serial.println("[Out] Caracter correspondiente:\t'J';");
        myDFPlayer.play(10); // Reproduce archivo 010.mp3 para 'J'
    }
    else if (inputMatches(10)) {
        Serial.println("[Out] Caracter correspondiente:\t'K';");
        myDFPlayer.play(11); // Reproduce archivo 011.mp3 para 'K'
    }
    else if (inputMatches(11)) {
        Serial.println("[Out] Caracter correspondiente:\t'L';");
        myDFPlayer.play(12); // Reproduce archivo 012.mp3 para 'L'
    }
    else if (inputMatches(12)) {
        Serial.println("[Out] Caracter correspondiente:\t'M';");
        myDFPlayer.play(13); // Reproduce archivo 013.mp3 para 'M'
    }
    else if (inputMatches(13)) {
        Serial.println("[Out] Caracter correspondiente:\t'N';");
        myDFPlayer.play(14); // Reproduce archivo 014.mp3 para 'N'
    }
    else if (inputMatches(14)) {
        Serial.println("[Out] Caracter correspondiente:\t'O';");
        myDFPlayer.play(15); // Reproduce archivo 015.mp3 para 'O'
    }
    else if (inputMatches(15)) {
        Serial.println("[Out] Caracter correspondiente:\t'P';");
        myDFPlayer.play(16); // Reproduce archivo 016.mp3 para 'P'
    }
    else if (inputMatches(16)) {
        Serial.println("[Out] Caracter correspondiente:\t'Q';");
        myDFPlayer.play(17); // Reproduce archivo 017.mp3 para 'Q'
    }
    else if (inputMatches(17)) {
        Serial.println("[Out] Caracter correspondiente:\t'R';");
        myDFPlayer.play(18); // Reproduce archivo 018.mp3 para 'R'
    }
    else if (inputMatches(18)) {
        Serial.println("[Out] Caracter correspondiente:\t'S';");
        myDFPlayer.play(19); // Reproduce archivo 019.mp3 para 'S'
    }
    else if (inputMatches(19)) {
        Serial.println("[Out] Caracter correspondiente:\t'T';");
        myDFPlayer.play(20); // Reproduce archivo 020.mp3 para 'T'
    }
    else if (inputMatches(20)) {
        Serial.println("[Out] Caracter correspondiente:\t'U';");
        myDFPlayer.play(21); // Reproduce archivo 021.mp3 para 'U'
    }
    else if (inputMatches(21)) {
        Serial.println("[Out] Caracter correspondiente:\t'V';");
        myDFPlayer.play(22); // Reproduce archivo 022.mp3 para 'V'
    }
    else if (inputMatches(22)) {
        Serial.println("[Out] Caracter correspondiente:\t'W';");
        myDFPlayer.play(23); // Reproduce archivo 023.mp3 para 'W'
    }
    else if (inputMatches(23)) {
        Serial.println("[Out] Caracter correspondiente:\t'X';");
        myDFPlayer.play(24); // Reproduce archivo 024.mp3 para 'X'
    }
    else if (inputMatches(24)) {
        Serial.println("[Out] Caracter correspondiente:\t'Y';");
        myDFPlayer.play(25); // Reproduce archivo 025.mp3 para 'Y'
    }
    else if (inputMatches(25)) {
        Serial.println("[Out] Caracter correspondiente:\t'Z';");
        myDFPlayer.play(26); // Reproduce archivo 026.mp3 para 'Z'
    }
    else if (inputMatches(26)) {
        Serial.println("[Out] Caracter correspondiente:\t'Ñ';");
        myDFPlayer.play(26); // Reproduce archivo 026.mp3 para 'Z'
    }
    else { 
        Serial.println("[!] Mensaje de error: Caracter no encontrado.");
        myDFPlayer.play(27); // Reproduce archivo de error, asumiendo que el archivo de error es 027.mp3
    }
}

bool inputMatches(int index) {
    for (int j = 0; j < BRAILLE_BTNS; j++) {
        if (alphabet[index][j] != input[j]) {
            return false;
        }
    }
    return true;
}
