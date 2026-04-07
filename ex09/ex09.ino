#include <WiFi.h>
#include <WebServer.h>
#define LED_BUILTIN 2  // 根据你的开发板调整 

const char* ap_ssid = "ESP32-CJJ";
const char* ap_pass = "12345678";

#define TOUCH_PIN 27       // 定义触摸引脚 T0 (GPIO 27)

WebServer server(80);


String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实时传感器仪表盘</title>
  <style>
    body { font-family: 'Segoe UI', sans-serif; text-align: center; margin-top: 50px; background-color: #f0f2f5; }
    .card { background: white; border-radius: 15px; padding: 40px; box-shadow: 0 4px 15px rgba(0,0,0,0.1); display: inline-block; width: 300px; }
    h1 { color: #333; margin-bottom: 10px; }
    #sensorValue { font-size: 80px; color: #007bff; font-weight: bold; margin: 20px 0; }
    .unit { font-size: 20px; color: #666; }
    .status { margin-top: 20px; color: #888; font-size: 14px; }
  </style>
</head>
<body>
  <div class="card">
    <h1>触摸传感器数值</h1>
    <div id="sensorValue">--</div>
    <div class="unit">电容读数</div>
    <div class="status" id="statusText">正在连接...</div>
  </div>

  <script>
    async function updateSensorData() {
      try {
        const response = await fetch('/data');
        const value = await response.text();
        document.getElementById('sensorValue').innerText = value;
        document.getElementById('statusText').innerText = "更新成功";
      } catch (error) {
        document.getElementById('statusText').innerText = "连接错误";
      }
    }
    // 每100ms请求一次数据
    setInterval(updateSensorData, 100);
    updateSensorData();
  </script>
</body>
</html>
)rawliteral";
  return html;
}

// ================= 处理函数 =================
void handleRoot() {
  server.send(200, "text/html", makePage());
}

void handleData() {
  int touchValue = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchValue));
}

void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found");
}

// ================= 初始化 =================
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED亮，表示启动

  // 启动 AP 模式
  WiFi.softAP(ap_ssid, ap_pass);
  Serial.println("AP 模式已启动");
  Serial.print("IP 地址: ");
  Serial.println(WiFi.softAPIP());

  // 路由配置
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP 服务器已启动");
}

// ================= 主循环 =================
void loop() {
  server.handleClient();
  // 此处不需要 Serial.print 打印数值，因为网页每0.1秒就会请求一次
  // 频繁打印会严重拖慢串口监视器
}