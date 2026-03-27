const int ledPinG = 26;
const int ledPinR = 32;

const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);

  ledcAttach(ledPinG, freq, resolution);
  ledcAttach(ledPinR, freq, resolution);
}

void loop() {
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    ledcWrite(ledPinG, dutyCycle);
    ledcWrite(ledPinR, 255 - dutyCycle);
    delay(10);
  }

  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(ledPinG, dutyCycle);
    ledcWrite(ledPinR, 255 - dutyCycle);
    delay(10);
  }
  
  Serial.println("Dual flash cycle completed");
}
