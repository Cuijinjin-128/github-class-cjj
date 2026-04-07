#include <WiFi.h>
#include <WebServer.h>

// ===================== ESP32自建热点配置 =====================
const char* ap_ssid = "ESP32-CJJ";    // 热点名称
const char* ap_pwd  = "12345678";    // 热点密码（不少于8位）
#define TOUCH_PIN 26                 // 触摸引脚
#define LED_PIN 2                    // LED引脚

WebServer server(80);

// ===================== 系统状态全局变量 =====================
bool isDefense = false;
bool isAlarm = false;
unsigned long alarmTime = 0;

// ===================== 生成网页（修复布防按钮链接） =====================
void handleRoot() {
  String ds = isDefense ? "<span style='color:red;'>布防中</span>" : "<span style='color:green;'>撤防中</span>";
  String as = isAlarm ? "<span style='color:red;'>报警！</span>" : "<span style='color:green;'>正常</span>";

  String html = "<html><body style='text-align:center;margin-top:60px;'>";
  html += "<h2>ESP32 安防报警</h2>";
  html += "<p>状态：" + ds + "</p >";
  html += "<p>报警：" + as + "</p >";
  // 核心修复：布防按钮链接改为/defense
  html += "<p><a href='/defense'><button style='padding:10px 20px;background:red;color:white;'>布防</button></a ></p >";
  html += "<p><a href='/undefense'><button style='padding:10px 20px;background:green;color:white;'>撤防</button></a ></p >";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// ===================== 网页路由 =====================
void handleDefense() {
  isDefense = true;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleUndefense() {
  isDefense = false;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW); // 强制熄灭LED
  server.sendHeader("Location", "/");
  server.send(303);
}

// ===================== 新增：触摸数据接口（实时上报） =====================
void handleTouchData() {
  // 注意：ESP32触摸引脚不能用digitalRead，必须用touchRead()
  int touchVal = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchVal));
}

// ===================== setup() 初始化（核心修改：AP模式） =====================
void setup() {
  Serial.begin(115200);
  delay(1000); // 等待串口稳定

  // 初始化LED引脚
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // ===================== 核心修改：开启ESP32自建热点 =====================
  WiFi.softAP(ap_ssid, ap_pwd);
  IPAddress apIP = WiFi.softAPIP();
  
  Serial.println("\n=== ESP32 热点已启动 ===");
  Serial.print("热点名称: ");
  Serial.println(ap_ssid);
  Serial.print("访问地址: http://");
  Serial.println(apIP);
  Serial.println("=========================");

  // 注册路由
  server.on("/", handleRoot);
  server.on("/defense", handleDefense);
  server.on("/undefense", handleUndefense);
  server.on("/touch", handleTouchData); // 新增触摸数据接口
  server.begin();
}

// ===================== loop() 主循环（修复触摸检测逻辑） =====================
void loop() {
  server.handleClient();

  // 报警状态逻辑：LED高频闪烁
  if (isAlarm) {
    if (millis() - alarmTime > 300) {
      alarmTime = millis();
      static bool s = false;
      s = !s;
      digitalWrite(LED_PIN, s ? HIGH : LOW);
    }
    return; // 报警时跳过其他逻辑，保持闪烁
  }

  // 只有布防且未报警时，才检测触摸
  if (isDefense) {
    // 核心修复：ESP32触摸引脚必须用touchRead()，不能用digitalRead()
    int touchVal = touchRead(TOUCH_PIN);
    // 触摸阈值（可根据实际调整，比如20/30）
    if (touchVal < 20) {
      isAlarm = true;
      Serial.println("🚨 触摸触发报警！");
    }
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(50); // 降低CPU占用
}