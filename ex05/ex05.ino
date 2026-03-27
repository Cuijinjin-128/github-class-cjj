#define TOUCH_PIN 27
#define LED_PIN   2

const int freq = 5000;
const int resolution = 8;

const int touchThreshold = 600;
const unsigned long debounceDelay = 80;
bool lastTouched = false;
unsigned long lastDebounceTime = 0;

int currentSpeed = 1;
const int speedSteps[3][2] = {
  {5, 5},
  {10, 5},
  {20, 3}
};


int dutyCycle = 0;
bool isIncreasing = true;

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, freq, resolution);
}

void loop() {
  int touchValue = touchRead(TOUCH_PIN);
  bool isTouched = (touchValue < touchThreshold);

  // 防抖逻辑
  if (millis() - lastDebounceTime < debounceDelay) {
    isTouched = lastTouched;
  }

  if (!lastTouched && isTouched) {
    currentSpeed = (currentSpeed % 3) + 1;
    lastDebounceTime = millis();
    Serial.print("切换到速度档位：");
    Serial.println(currentSpeed);
  }
  lastTouched = isTouched;

  int step = speedSteps[currentSpeed - 1][0];
  int delayMs = speedSteps[currentSpeed - 1][1];

  if (isIncreasing) {
    dutyCycle += step;
    if (dutyCycle >= 255) {
      dutyCycle = 255;
      isIncreasing = false;
    }
  } else {
    dutyCycle -= step;
    if (dutyCycle <= 0) {
      dutyCycle = 0;
      isIncreasing = true;
    }
  }

  ledcWrite(LED_PIN, dutyCycle);
  delay(delayMs);
}
