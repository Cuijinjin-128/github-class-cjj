#define TOUCH_PIN 27    // 触摸引脚改为 27
#define LED_PIN   2     // 板载LED

// 触摸阈值（可根据串口数值微调）
const int touchThreshold = 600;
// 防抖延时
const unsigned long debounceDelay = 80;

bool ledState = false;
int lastTouchValue = 0;
bool lastTouched = false;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int touchValue = touchRead(TOUCH_PIN);
  Serial.print("Touch: ");
  Serial.println(touchValue);

  bool isTouched = (touchValue < touchThreshold);

  // 防抖
  if (millis() - lastDebounceTime < debounceDelay) {
    return;
  }

  // 上升沿检测：刚按下瞬间
  if (!lastTouched && isTouched) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    lastDebounceTime = millis();
  }

  lastTouched = isTouched;
  delay(20);
}