const int ledPin_R = 27;
const int ledPin_G = 26;
const int ledPin_Y = 32;

enum SosState {
  S_SHORT,
  PAUSE_S_O,
  O_LONG,
  PAUSE_O_S,
  PAUSE_WORD
};

SosState currentState = S_SHORT;
unsigned long previousMillis = 0;
int blinkCount = 0;

const int SHORT_ON  = 200;
const int SHORT_OFF = 200;
const int LONG_ON   = 600;
const int LONG_OFF  = 200;
const int CHAR_PAUSE = 500;
const int WORD_PAUSE = 2000;

void setup() {
  pinMode(ledPin_R, OUTPUT);
  pinMode(ledPin_G, OUTPUT);
  pinMode(ledPin_Y, OUTPUT);

  digitalWrite(ledPin_R, LOW);
  digitalWrite(ledPin_G, LOW);
  digitalWrite(ledPin_Y, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentState) {
    case S_SHORT:
      if (blinkCount < 3) {
        if (digitalRead(ledPin_R) == LOW) {
          if (currentMillis - previousMillis >= SHORT_OFF) {
            digitalWrite(ledPin_R, HIGH);
            digitalWrite(ledPin_G, HIGH);
            digitalWrite(ledPin_Y, HIGH);
            previousMillis = currentMillis;
          }
        } else {
          if (currentMillis - previousMillis >= SHORT_ON) {
            digitalWrite(ledPin_R, LOW);
            digitalWrite(ledPin_G, LOW);
            digitalWrite(ledPin_Y, LOW);
            previousMillis = currentMillis;
            blinkCount++; 
          }
        }
      } else {
        currentState = PAUSE_S_O;
        previousMillis = currentMillis;
        blinkCount = 0;
      }
      break;

    case PAUSE_S_O:
      if (currentMillis - previousMillis >= CHAR_PAUSE) {
        currentState = O_LONG;
        previousMillis = currentMillis;
      }
      break;

   case O_LONG:
        if (digitalRead(ledPin_R) == LOW) {
          if (currentMillis - previousMillis >= LONG_OFF) {
            digitalWrite(ledPin_R, HIGH);
            digitalWrite(ledPin_G, HIGH);
            digitalWrite(ledPin_Y, HIGH);
            previousMillis = currentMillis;
          }
        } else {
          if (currentMillis - previousMillis >= LONG_ON) {
            digitalWrite(ledPin_R, LOW);
            digitalWrite(ledPin_G, LOW);
            digitalWrite(ledPin_Y, LOW);
            previousMillis = currentMillis;
            blinkCount++;
          }
        }
      } else {
        currentState = PAUSE_O_S;
        previousMillis = currentMillis;
        blinkCount = 0;
      }
      break;

    case PAUSE_O_S:
      if (currentMillis - previousMillis >= CHAR_PAUSE) {
        currentState = S_SHORT;
        previousMillis = currentMillis;
      }
      break;

    case PAUSE_WORD:
      if (currentMillis - previousMillis >= WORD_PAUSE) {
        currentState = S_SHORT;
        previousMillis = currentMillis;
      }
      break;
  }
}