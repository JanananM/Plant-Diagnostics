#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

DHT dht(D1,DHT11);

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
String style = "";
double data; 
float hum, temp;

const char webpage[] PROGMEM = R"=====(

<html> 

<title>IOT Plant Sensor</title> 


<body>
<div class = "iot">
  <h1>IoT Plant Sensor</h1>
</div>

<table>
  <tr>
      <th>Sunlight Level:<span id="Sunlight">0</span></th>
      <th>Temperature (Degree Celsius):<span id="Temperature">0</span></th>
      <th>Humidity:<span id="Humidity">0</span></th> 
  </tr>
</table>
</body>

<style type"text/css" media="screen" />
  body{
    background-color: lightblue;
}

h1{
    color:brown;
    text-align:center;
}

.iot{
  background-color: lightgreen;
  padding: 20px;
  text-align:center;
  font-family:helvetica;
  border-radius: 25px;
}

table{
  width:100%;
  height:100%;
}

th{
    padding: 100px;
    text-align: center;
    font-family: helvetica;
}

th:hover{
  background-color: brown;
  color:white;
}
</style>

<script>

setInterval(function(){
  getData();
  getDatatemp();
  getDatahum();
},500);

function getData(){
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function(){
    if(this.readyState == 4 && this.status == 200){
      document.getElementById("Sunlight").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET","SunlightSensor",true);
  xhttp.send();
}

function getDatatemp(){
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function(){
    if(this.readyState == 4 && this.status == 200){
      document.getElementById("Temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET","Temp",true);
  xhttp.send();
}

function getDatahum(){
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function(){
    if(this.readyState == 4 && this.status == 200){
      document.getElementById("Humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET","Hum",true);
  xhttp.send();
}
</script>


</html>

)=====";

void handleRoot(){
  String s = webpage;
  server.send(200,"text/html",s);
}

void handleSS(){
  String ss = String(data);
  server.send(200,"text/plane",ss);
}

void handletemp(){
  String tempstring = String(temp);
  server.send(200,"text/plane",tempstring);
}

void handlehum(){
  String humstring = String(hum);
  server.send(200,"text/plane",humstring);
}

void setup(void){

  pinMode(A0, INPUT);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");

  dht.begin();
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  server.on("/",handleRoot);
  server.on("/SunlightSensor",handleSS);
  server.on("/Temp",handletemp);
  server.on("/Hum",handlehum);

  
  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  data = analogRead(A0);
  delay(1000);

/*read from sensors*/
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  delay(1000);
  
  server.handleClient();
}