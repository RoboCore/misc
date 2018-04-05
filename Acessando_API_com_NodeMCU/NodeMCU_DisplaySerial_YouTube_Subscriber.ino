#include <SerialDisplay.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
char ssid[] = "NOME_DA_REDE";       // sua rede wifi
char password[] = "SENHA_DA_REDE";  // sua senha
char channelId[] = "ID_DO_CANAL"; // ID do seu canal
char apiKey[] = "CHAVE_DA_API"; // chave da API
SerialDisplay displays(14, 12, 10); // (data D5, clock D6, number of modules)
WiFiClientSecure client;
String mensagemAnterior = "";

void setup() {
  for (int x = 1; x < 11; x++) {
    displays.Print(1, x);
  }
  
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  IPAddress ip = WiFi.localIP();
  
}

void loop() {
  checarInscritos();
  delay(5000); //checa inscritos de 10 em 10 segundos
}

void checarInscritos() {
  client.setTimeout(10000);
  if (!client.connect("www.googleapis.com", 443)) {  //443: YouTube API SSL Port
    return;
  }
  String chamarAPI = "GET https://www.googleapis.com/youtube/v3/channels?part=statistics&id=";
  chamarAPI += channelId;
  chamarAPI += "&key=";
  chamarAPI += apiKey;
  chamarAPI += " HTTP/1.1";
  client.println(chamarAPI);
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    return;
  }
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    return;
  }
  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    return;
  }
  String resposta = "";
  while (client.available()) {
    char c = client.read();
    resposta += c;
  }
  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  // Parse JSON object
  JsonObject& root = jsonBuffer.parseObject(resposta);
  // Test if parsing succeeds.
  if (!root.success()) {
    return;
  }
  JsonObject& items0 = root["items"][0];
  JsonObject& items0_statistics = items0["statistics"];
  //const char* items0_statistics_viewCount = items0_statistics["viewCount"]; // "2061149"
  //const char* items0_statistics_commentCount = items0_statistics["commentCount"]; // "3"
  const char* items0_statistics_subscriberCount = items0_statistics["subscriberCount"]; // "37747"
  //bool items0_statistics_hiddenSubscriberCount = items0_statistics["hiddenSubscriberCount"]; // false
  //const char* items0_statistics_videoCount = items0_statistics["videoCount"]; // "162"
  //int seguidores = atoi(items0_statistics_subscriberCount);
  String mensagem = items0_statistics_subscriberCount;
  //mensagem += "F"; //FIM
  Serial.print(mensagem);
  if (mensagem != mensagemAnterior) {
    for (int cont = 0; cont <= mensagem.length(); cont++) {
      displays.Print(mensagem[cont], cont + 1);
      delay(10);
    }
    mensagemAnterior = mensagem;
  }
  //jsonBuffer.clear();
  //items0_statistics_subscriberCount = "";
  // Disconnect
  client.stop();
}
