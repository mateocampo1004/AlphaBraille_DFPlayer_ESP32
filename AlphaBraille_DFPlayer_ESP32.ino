#include "DFPlayerMini_Fast.h"
#include <SoftwareSerial.h>

#define CHECK 12          // Pin del botón de verificación (GPIO 12)
#define BUTTON_1 13       // Primer botón Braille (GPIO 13)
#define BUTTON_2 14       // Segundo botón Braille (GPIO 14)
#define BUTTON_3 15       // Tercer botón Braille (GPIO 15)
#define BUTTON_4 5        // Cuarto botón Braille (GPIO 5)
#define BUTTON_5 18       // Quinto botón Braille (GPIO 18)
#define BUTTON_6 19       // Sexto botón Braille (GPIO 19)

#define LED_1 25          // LED del botón 1
#define LED_2 23          // LED del botón 2
#define LED_3 22          // LED del botón 3
#define LED_4 26          // LED del botón 4
#define LED_5 27          // LED del botón 5
#define LED_6 32          // LED del botón 6

#define BRAILLE_BTNS 6

// Estructura para representar los caracteres Braille
struct BrailleChar {
    int dots[6]; // Representación en Braille con 6 bits
    char character; // Caracter asociado
};

// Inicialización correcta del array de caracteres Braille (42 elementos)
BrailleChar alphabet[] = { 
    {{0, 0, 0, 1, 0, 0}, 'A'}, {{0, 0, 0, 1, 1, 0}, 'B'}, {{1, 0, 0, 1, 0, 0}, 'C'},
    {{1, 1, 0, 1, 0, 0}, 'D'}, {{0, 1, 0, 1, 0, 0}, 'E'}, {{1, 0, 0, 1, 1, 0}, 'F'},
    {{1, 1, 0, 1, 1, 0}, 'G'}, {{0, 1, 0, 1, 1, 0}, 'H'}, {{1, 0, 0, 0, 1, 0}, 'I'},
    {{1, 1, 0, 0, 1, 0}, 'J'}, {{0, 0, 0, 1, 0, 1}, 'K'}, {{0, 0, 0, 1, 1, 1}, 'L'},
    {{1, 0, 0, 1, 0, 1}, 'M'}, {{1, 1, 0, 1, 0, 1}, 'N'}, {{0, 1, 0, 1, 0, 1}, 'O'},
    {{1, 0, 0, 1, 1, 1}, 'P'}, {{1, 1, 0, 1, 1, 1}, 'Q'}, {{0, 1, 0, 1, 1, 1}, 'R'},
    {{1, 0, 0, 0, 1, 1}, 'S'}, {{1, 1, 0, 0, 1, 1}, 'T'}, {{0, 0, 1, 1, 0, 1}, 'U'},
    {{0, 0, 1, 1, 1, 1}, 'V'}, {{1, 1, 1, 0, 1, 0}, 'W'}, {{1, 0, 1, 1, 0, 1}, 'X'},
    {{1, 1, 1, 1, 0, 1}, 'Y'}, {{0, 1, 1, 1, 0, 1}, 'Z'}, {{1, 1, 1, 1, 1, 0}, 'Ñ'},
    {{0, 0, 0, 0, 0, 1}, '.'}, {{0, 0, 0, 0, 1, 0}, ','}, {{0, 0, 0, 0, 1, 1}, ';'},
    {{0, 1, 0, 0, 1, 0}, ':'}, {{0, 0, 1, 0, 0, 1}, '-'}, {{0, 0, 1, 0, 1, 0}, '¿'},
    {{0, 1, 0, 0, 1, 1}, '¡'}, {{0, 0, 1, 0, 1, 1}, '"'}, {{0, 0, 1, 1, 1, 0}, '('},
    {{1, 1, 0, 0, 0, 1}, ')'}, {{0, 1, 1, 1, 1, 1}, 'á'}, {{1, 0, 1, 0, 1, 1}, 'é'},
    {{1, 0, 0, 0, 0, 1}, 'í'}, {{1, 0, 1, 0, 0, 1}, 'ó'}, {{1, 1, 1, 0, 1, 1}, 'ú'},
    {{0, 1, 1, 1, 1, 0}, 'ü'}
};

int input[BRAILLE_BTNS] = {0, 0, 0, 0, 0, 0};

SoftwareSerial mySoftwareSerial(16, 17); // RX, TX
DFPlayerMini_Fast myDFPlayer;

void setup() {
    pinMode(CHECK, INPUT_PULLUP);
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLUP);
    pinMode(BUTTON_4, INPUT_PULLUP);
    pinMode(BUTTON_5, INPUT_PULLUP);
    pinMode(BUTTON_6, INPUT_PULLUP);

    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT); // LED del botón 3
    pinMode(LED_4, OUTPUT);
    pinMode(LED_5, OUTPUT);
    pinMode(LED_6, OUTPUT);

    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
    digitalWrite(LED_5, LOW);
    digitalWrite(LED_6, LOW);

    Serial.begin(9600);
    mySoftwareSerial.begin(9600);
    myDFPlayer.begin(mySoftwareSerial);

    myDFPlayer.volume(20); // Configurar el volumen
}

void loop() {
    readInput();
    controlLEDs();

    if (digitalRead(CHECK) == LOW) {
        delay(500);
        findAndPlayCharacter();
    }
}

void readInput() {
    input[0] = !digitalRead(BUTTON_1);
    input[1] = !digitalRead(BUTTON_2);
    input[2] = !digitalRead(BUTTON_3);
    input[3] = !digitalRead(BUTTON_4);
    input[4] = !digitalRead(BUTTON_5);
    input[5] = !digitalRead(BUTTON_6);
}

void controlLEDs() {
    digitalWrite(LED_1, input[0]);
    digitalWrite(LED_2, input[1]);
    digitalWrite(LED_3, input[2]); // Control del LED asociado al botón 3
    digitalWrite(LED_4, input[3]);
    digitalWrite(LED_5, input[4]);
    digitalWrite(LED_6, input[5]);
}

void findAndPlayCharacter() {
    for (int i = 0; i < 43; i++) {
        if (inputMatches(i)) {
            Serial.print("Character detected: ");
            Serial.println(alphabet[i].character);
            myDFPlayer.play(i+1);
            delay(1000);
            break;
        }
    }
}

bool inputMatches(int index) {
    for (int i = 0; i < 6; i++) {
        if (alphabet[index].dots[i] != input[i]) {
            return false;
        }
    }
    return true;
}