#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

ESP8266WebServer server(80);
 
const char* ssid = "Hello";
const char* password = "asasasas";
const int channelNo = 1;
const bool hidden = false;
const int max_connection = 2;

const char *page = "<!DOCTYPE html><html lang='en'><head>    <meta charset='UTF-8'>    <meta name='viewport' content='width=device-width, initial-scale=1.0'>    <title>Joystick</title>    <style>        * {            box-sizing: border-box;            margin: 0;            padding: 0;        }        html{            width: 100vw;            height: 100vh;            background-color: rgb(107, 208, 211);        }        .main{            width: 100%;        }        .container {            /* height: 100%; */            width: 100%;            display: flex;            flex-direction: column;        }        .each {            width: 100%;            height: 50vh;            display: flex;            align-items: center;            justify-content: center;        }        .canvas {            border-radius: 50%;        }    </style></head><body>   <div class='main'>       <div class='container'>            <div class='each'>                <canvas class='canvas' id='border1' width='300' height='300'>                </canvas>            </div>            <div class='each'>                <canvas class='canvas' id='border2' width='300' height='300'>                </canvas>            </div>       </div>   </div>        <script>        var totalRadius = 150;        var joyRadius = 20;        var coords1 = [150, 0];        var coords2 = [150, 0];        setInterval(() => {            fetch(`http://1.1.1.111/data?c1=${coords1[1]}&c2=${coords1[0]}&c3=${coords2[1]}&c4=${coords2[0]}`)        }, 500);        var b1 = document.getElementById('border1');        var b2 = document.getElementById('border2');                var ctx1 = b1.getContext('2d');        var ctx2 = b2.getContext('2d');        draw1();        draw2();        b1.addEventListener('touchmove', mouseDown1);        b2.addEventListener('touchmove', mouseDown2);        function drawJoy1() {            ctx1.beginPath();            ctx1.arc(coords1[1], coords1[0], 80, 0, 2*Math.PI);            ctx1.fillStyle = '#6bd0d3';            ctx1.fill();        }        function drawJoy2() {            ctx2.beginPath();            ctx2.arc(coords2[1], coords2[0], 80, 0, 2*Math.PI);            ctx2.fillStyle = '#6bd0d3';            ctx2.fill();        }        function draw1() {                        ctx1.beginPath();            ctx1.arc(totalRadius,totalRadius,totalRadius,0,2*Math.PI);            ctx1.fillStyle = '#ededed';            ctx1.fill();            drawJoy1();        }        function draw2() {                        ctx2.beginPath();            ctx2.arc(totalRadius,totalRadius,totalRadius,0,2*Math.PI);            ctx2.fillStyle = '#ededed';            ctx2.fill();            drawJoy2();        }                function getPosition(event, ctx) {            var offTop = event.touches[0].target.offsetTop;            var offLeft = event.touches[0].target.offsetLeft;                        var top = event.touches[0].clientY - offTop;            var left = event.touches[0].clientX - offLeft + 1;            if(top <= 0) top=0;            if(top > 300) top=300;            if(left <= 0) left=0;            if(left > 300) left=300;            return [top, left];        };        function mouseDown1(e) {            coords1 = getPosition(e, ctx1);            draw1();        };        function mouseDown2(e) {            coords2 = getPosition(e, ctx2);            draw2();        };    </script> </body></html>";
IPAddress local_IP(1,1,1,111);
IPAddress gateway(1,1,1,0);
IPAddress subnet(255,255,255,0);

int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;

StaticJsonDocument <100> json;
 
void setup(void){   
  delay(1000);
  Serial.begin(9600);
  WiFi.softAP(ssid, password, channelNo, hidden, max_connection);
//  Serial.println("");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }
  
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(ssid);
//  Serial.print("IP address: ");
  WiFi.softAPConfig(local_IP, gateway, subnet);
//  Serial.println(WiFi.softAPIP()); 

  // server stuffs

  server.on("/",HTTP_GET, [](){
    server.send(200, "text/html", page);
  });

  server.on("/data", handleData);

  server.begin();
  
}
 
void loop(void){
  server.handleClient();

  json["c1"] = c1;
  json["c2"] = c2;
  json["c3"] = c3;
  json["c4"] = c4;
  
  serializeJson(json, Serial);
  
}

void handleData() {                      
  if( ! server.hasArg("c1") && ! server.hasArg("c2") && ! server.hasArg("c3") && ! server.hasArg("c4")  ) {
    server.send(400, "text/plain", "400: Invalid Request");
    return;
  }

   c1 = server.arg("c1").toInt();
   c2 = server.arg("c2").toInt();
   c3 = server.arg("c3").toInt();
   c4 = server.arg("c4").toInt();
   
   server.send(200, "text/plain", "Done");
   
   return;
}
