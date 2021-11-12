// Importing necessary libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// Setting network credentials
const char *ssid = "ReAb";
const char *password = "colgatedeesta";

// Creating a AsyncWebServer object
AsyncWebServer server(8080);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>SERVER ESP32</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="styles.css">
</head>
<body>
  <h2>ESP32 WEB SERVER</h2>

  %PLACEHOLDER%

<script src="main.js"></script>
</body>
</html>
)rawliteral";

const char css[] PROGMEM = R"rawliteral(
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
)rawliteral";

const char js_scripts[] PROGMEM = R"rawliteral(
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}

// WebSocket Settings
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', (event) => {
    initWebSocket();
  });

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = (event) => {
      console.log('Connection opened');
    };
    websocket.onclose   = (event) => {
      console.log('Connection closed:', event);
      // setTimeout(initWebSocket, 2000);
    };
    websocket.onmessage = (event) => {
      var state;
      if (event.data == "checked"){
        state = event.data;
      }
      else{
        state = "";
      }
      document.getElementById('32').innerHTML = state;
    }; 
    websocket.onerror =  (event) => {
      console.log('WebSocket Error: ', event);
   
    };
  }

)rawliteral";

// Allow replace elements into html code
String processor(const String &var)
{
  if (var == "PLACEHOLDER")
  {
    String buttons = "";
    buttons += "<h4>Output - GPIO 32</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"32\" " + outputState(32) + "><span class=\"slider\"></span></label>";

    return buttons;
  }
  return String();
}

String outputState(int output)
{
  if (digitalRead(output))
  {
    return "checked";
  }
  else
  {
    return "";
  }
}

// webSocket Configuration
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    if (strcmp((char *)data, "<message received>") == 0)
    {
        // do somethings
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  switch (type){
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);  
 
}
// -------------------------

void setup()
{
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi");
  }

  // Print ESP Local IP Address
  Serial.println("Host:");
  Serial.println(WiFi.localIP());

    // Start socket
  initWebSocket();

  // Add some routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/javascript;charset=UTF-8 ", js_scripts); });
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/css;charset=UTF-8 ", css); });

  // Send a GET request to <HOST_IP>/update?output=<output_value>&state=<state_value>
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String output_value;
              String state_value;
              // GET input1 value on <HOST_IP>/update?output=<output_value>&state=<state_value>
              if (request->hasParam("output") && request->hasParam("state"))
              {
                output_value = request->getParam("output")->value();
                state_value = request->getParam("state")->value();
                digitalWrite(output_value.toInt(), state_value.toInt());
                // send message by broadcast
                ws.textAll(state_value.toInt() == 1 ? String("checked") : String(""));
              }
              else
              {
                output_value = "No message sent";
                state_value = "No message sent";
              }
              Serial.print("GPIO: ");
              Serial.print(output_value);
              Serial.print(" - Set to: ");
              Serial.println(state_value);
              request->send(200, "text/plain", "OK");
            });


  // Start server
  server.begin();
}

void loop(){
   ws.cleanupClients();
}
 
