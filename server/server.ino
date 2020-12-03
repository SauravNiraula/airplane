#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>


//int16_t ax;
//const int MPU_ADDR = 0x68;
//
//int16_t offset = 0;


ESP8266WebServer server(80);
 
const char* ssid = "Hello";
const char* password = "asasasas";
const int channelNo = 1;
const bool hidden = false;
const int max_connection = 2;

const char *page = "<!DOCTYPE html><html lang='en'><head>    <meta charset='UTF-8'>    <meta name='viewport' content='width=device-width, initial-scale=1.0'>    <title>Joystick</title>    <style>        * {            box-sizing: border-box;            margin: 0;            padding: 0;        }        html{            width: 100vw;            height: 100vh;            background-color: rgb(107, 208, 211);        }        .main{            width: 100%;        }        .container {            width: 100%;            display: flex;            flex-direction: column;        }        .each {            width: 100%;            height: 50vh;            display: flex;            align-items: center;            justify-content: center;        }        .canvas {            border-radius: 50%;        }    </style></head><body>   <div class='main'>       <div class='container'>            <div class='each'>                <canvas class='canvas' id='border1' width='250' height='250'>                </canvas>            </div>            <div class='each'>                <canvas class='canvas' id='border2' width='250' height='250'>                </canvas>            </div>       </div>   </div>        <script>        var totalRadius = 125;        var joyRadius = 20;        var coords1 = [125, 0];        var coords2 = [125, 0];        setInterval(() => {            fetch(`http://1.1.1.111/data?c1=${coords1[1]}&c2=${coords1[0]}&c3=${coords2[1]}&c4=${coords2[0]}`)        }, 100);        var b1 = document.getElementById('border1');        var b2 = document.getElementById('border2');                var ctx1 = b1.getContext('2d');        var ctx2 = b2.getContext('2d');        draw1();        draw2();        b1.addEventListener('touchmove', mouseDown1);        b2.addEventListener('touchmove', mouseDown2);        function drawJoy1() {            ctx1.beginPath();            ctx1.arc(coords1[1], coords1[0], 70, 0, 2*Math.PI);            ctx1.fillStyle = '#6bd0d3';            ctx1.fill();        }        function drawJoy2() {            ctx2.beginPath();            ctx2.arc(coords2[1], coords2[0], 70, 0, 2*Math.PI);            ctx2.fillStyle = '#6bd0d3';            ctx2.fill();        }        function draw1() {                        ctx1.beginPath();            ctx1.arc(totalRadius,totalRadius,totalRadius,0,2*Math.PI);            ctx1.fillStyle = '#ededed';            ctx1.fill();            drawJoy1();        }        function draw2() {                        ctx2.beginPath();            ctx2.arc(totalRadius,totalRadius,totalRadius,0,2*Math.PI);            ctx2.fillStyle = '#ededed';            ctx2.fill();            drawJoy2();        }                function getPosition(event, ctx, id) {            event = Object.values(event.touches);            event = event.filter(each => each.target.id == id)[0];            var offTop = event.target.offsetTop;            var offLeft = event.target.offsetLeft;                        var top = event.clientY - offTop;            var left = event.clientX - offLeft + 1;            if(top < 0) top=0;            if(top > 255) top=255;            if(left < 0) left=0;            if(left > 255) left=255;            return [top, left];        };        function mouseDown1(e) {            coords1 = getPosition(e, ctx1, 'border1');            draw1();        };        function mouseDown2(e) {            coords2 = getPosition(e, ctx2, 'border2');            draw2();        };    </script> </body></html>";
IPAddress local_IP(1,1,1,111);
IPAddress gateway(1,1,1,0);
IPAddress subnet(255,255,255,0);

const int dataLen = 4;
int16_t data[dataLen];
byte checkByte[dataLen];

void sendArray(byte[], int[], int);
 
void setup(){   
  delay(1000);
  Serial.begin(9600);
  WiFi.softAP(ssid, password, channelNo, hidden, max_connection);
  Serial.println("");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.println(WiFi.softAPIP()); 


  Wire.begin(D1, D2);
//  Wire.beginTransmission(MPU_ADDR);
//  Wire.write(0x6B);
//  Wire.write(0);
//  Wire.endTransmission(true);

  checkByte[0] = 0xFC;
  checkByte[1] = 0xFD;
  checkByte[2] = 0xFE;
  checkByte[3] = 0xFF;

  data[0] = 0;
  data[1] = 125;
  data[2] = 125;
  data[3] = 125;

  // server stuffs

  server.on("/",HTTP_GET, [](){
    server.send(200, "text/html", page);
  });

  server.on("/data", handleData);

  server.begin();
  
}
 
void loop(){
  server.handleClient();
//  mpu();
}


//void mpu() {
//  Wire.beginTransmission(MPU_ADDR);
//  Wire.write(0x3B);
//  Wire.endTransmission(true);
//  Wire.requestFrom(MPU_ADDR, 2, true);
//  if (Wire.available() == 2) {
//    ax = Wire.read()<<8 | Wire.read();
//    ax = ax/200;
//    if(millis() < 5000) {
//      offset = ax;
//    }
//    else {
//      
//   // transfer mpu6050 data i2c
//      ax = ax - offset;
//      Wire.beginTransmission(11);
//      Wire.write(0xEF);
//      Wire.write(ax>>8);
//      Wire.write(ax);
//      Wire.endTransmission(true);
//    }
//
//    
//  };
//  return;
//}


void sendArray(byte arr1[], int16_t arr2[], int dataLen) {
//  Serial.println(data[0]);
  for(int i = 0; i < dataLen; i++) {

    Wire.beginTransmission(11);
    Wire.write(arr1[i]);
    Wire.write(arr2[i]>>8);
    Wire.write(arr2[i]);    
    Wire.endTransmission(true);
    
  }
  
}

void handleData() {                      
  if( ! server.hasArg("c1") && ! server.hasArg("c2") && ! server.hasArg("c3") && ! server.hasArg("c4")  ) {
    server.send(400, "text/plain", "400: Invalid Request");
    return;
  }

   data[0] = server.arg("c1").toInt();
   data[1] = server.arg("c2").toInt();
   data[2] = server.arg("c3").toInt();
   data[3] = server.arg("c4").toInt();
   
   server.send(200, "text/plain", "Done"); 

   sendArray(checkByte, data, dataLen);
     
   return;
}
