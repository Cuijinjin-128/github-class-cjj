// 定义 LED 引脚
const int ledPin    = 2;
const int ledPin_R  = 27;
const int ledPin_G  = 26;
const int ledPin_Y  = 32;

// 1Hz = 亮 500ms + 灭 500ms
const unsigned long interval = 500;

unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(ledPin_R, OUTPUT);
  pinMode(ledPin_G, OUTPUT);
  pinMode(ledPin_Y, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    ledState = !ledState;

    digitalWrite(ledPin,    ledState);
    digitalWrite(ledPin_R,  ledState);
    digitalWrite(ledPin_G,  ledState);
    digitalWrite(ledPin_Y,  ledState);

    Serial.println(ledState ? "LED ON" : "LED OFF");
  }
}

