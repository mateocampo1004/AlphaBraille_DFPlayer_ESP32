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
    {{1, 0, 0, 0, 0, 0}, 'A'}, {{1, 1, 0, 0, 0, 0}, 'B'}, {{1, 0, 0, 1, 0, 0}, 'C'},
    {{1, 0, 0, 1, 1, 0}, 'D'}, {{1, 0, 0, 0, 1, 0}, 'E'}, {{1, 1, 0, 1, 0, 0}, 'F'},
    {{1, 1, 0, 1, 1, 0}, 'G'}, {{1, 1, 0, 0, 1, 0}, 'H'}, {{0, 1, 0, 1, 0, 0}, 'I'},
    {{0, 1, 0, 1, 1, 0}, 'J'}, {{1, 0, 1, 0, 0, 0}, 'K'}, {{1, 1, 1, 0, 0, 0}, 'L'},
    {{1, 0, 1, 1, 0, 0}, 'M'}, {{1, 0, 1, 1, 1, 0}, 'N'}, {{1, 0, 1, 0, 1, 0}, 'O'},
    {{1, 1, 1, 1, 0, 0}, 'P'}, {{1, 1, 1, 1, 1, 0}, 'Q'}, {{1, 1, 1, 0, 1, 0}, 'R'},
    {{0, 1, 1, 1, 0, 0}, 'S'}, {{0, 1, 1, 1, 1, 0}, 'T'}, {{1, 0, 1, 0, 0, 1}, 'U'},
    {{1, 1, 1, 0, 0, 1}, 'V'}, {{0, 1, 0, 1, 1, 1}, 'W'}, {{1, 0, 1, 1, 0, 1}, 'X'},
    {{1, 0, 1, 1, 1, 1}, 'Y'}, {{1, 0, 1, 0, 1, 1}, 'Z'}, {{1, 1, 0, 1, 1, 1}, 'Ñ'},
    {{0, 0, 1, 0, 0, 0}, '.'}, {{0, 1, 0, 0, 0, 0}, ','}, {{0, 1, 1, 0, 0, 0}, ';'},
    {{0, 1, 0, 0, 1, 0}, ':'}, {{0, 0, 1, 0, 0, 1}, '-'}, {{0, 1, 0, 0, 0, 1}, '¿'},
    {{0, 1, 1, 0, 1, 0}, '¡'}, {{0, 1, 1, 0, 0, 1}, '"'}, {{1, 1, 0, 0, 0, 1}, '('},
    {{0, 0, 1, 1, 1, 0}, ')'}, {{1, 0, 1, 0, 1, 1}, 'á'}, {{1, 1, 1, 0, 1, 1}, 'é'},
    {{0, 1, 1, 1, 0, 1}, 'í'}, {{0, 0, 1, 1, 0, 0}, 'ó'}, {{0, 0, 1, 1, 0, 1}, 'ú'},
    {{0, 1, 1, 1, 1, 1}, 'ü'}
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
    pinMode(LED_4, OUTPUT);
    pinMode(LED_5, OUTPUT);
    pinMode(LED_6, OUTPUT);

    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_4, LOW);
    digitalWrite(LED_5, LOW);
    digitalWrite(LED_6, LOW);

    Serial.begin(9600);
    mySoftwareSerial.begin(9600);
    myDFPlayer.begin(mySoftwareSerial);

    myDFPlayer.volume(20); // Configurar el volumen
}

void loop() {
    // Leer el estado de los botones
    readInput();
    
    // Mostrar el estado de los botones y controlar los LEDs
    controlLEDs();
    
    // Si el botón CHECK está presionado, procesar la entrada y reproducir el carácter correspondiente
    if (digitalRead(CHECK) == LOW) {
        delay(500);
        findAndPlayCharacter();
    }
}

void readInput() {
    input[0] = !digitalRead(BUTTON_1); // Leer si el botón está presionado (LOW significa presionado)
    input[1] = !digitalRead(BUTTON_2);
    input[2] = !digitalRead(BUTTON_3);
    input[3] = !digitalRead(BUTTON_4);
    input[4] = !digitalRead(BUTTON_5);
    input[5] = !digitalRead(BUTTON_6);
}

void controlLEDs() {
    // Mostrar el estado de cada botón y encender/apagar LEDs
    digitalWrite(LED_1, input[0]);
    digitalWrite(LED_2, input[1]);
    digitalWrite(LED_4, input[2]);
    digitalWrite(LED_5, input[3]);
    digitalWrite(LED_6, input[4]);
}

void findAndPlayCharacter() {
    for (int i = 0; i < 42; i++) {
        if (inputMatches(i)) {
            Serial.print("Character detected: ");
            Serial.println(alphabet[i].character);
            myDFPlayer.play(i+1); // Reproducir sonido (puedes agregar lógica para sonidos específicos)
            delay(1000); // Delay entre reproducciones
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
