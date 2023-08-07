#include <ESP8266WiFi.h>
 
#define SSID_REDE     "enter your wireless network name" //This data should be changed before use.
#define SENHA_REDE    "enter the password" //This data should be changed before use.
#define INTERVALO_DADOS  15000
 
char EnderecoAPIThingSpeak[] = "api.thingspeak.com";
String ChaveEscritaThingSpeak = "enter key thingspeak"; //This data should be changed before use.
long lastConnectionTime; 
WiFiClient client;
 
void EnviaInformacoesThingspeak(String StringDados);
void ConectaWiFi(void);
float LeituraGLP(void);
 
void EnviaInformacoesThingspeak(String StringDados)
{
    if (client.connect(EnderecoAPIThingSpeak, 80))
    {         
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+ChaveEscritaThingSpeak+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(StringDados.length());
        client.print("\n\n");
        client.print(StringDados);
   
        lastConnectionTime = millis();
        Serial.println("Data sended to App ThingSpeak.");
     }   
}

void ConectaWiFi(void)
{
    client.stop();
    Serial.println("fiding local wifi...");
    Serial.println();  
    delay(1000);
    WiFi.begin(SSID_REDE, SENHA_REDE);
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("Wifi conected");  
    Serial.println("IP device: ");
    Serial.println(WiFi.localIP());
 
    delay(1000);
}

float LeituraGLP(void)
{
    int Base;
    float percentual_ppm;
 
     Base = analogRead(0);
     Serial.print("[Reader numeric] "); 
     Serial.println(Base);
      
     percentual_ppm = 100 * ((-350+(float)Base) / 850);
     Serial.print("[Estimated percentage of LPG in the local atmosphere (+- 2%)] ");
     Serial.print(percentual_ppm);
     Serial.println("%");
     Serial.println("------------------------------------------------------------------");
     
     return percentual_ppm;
}
void setup()
{  
    Serial.begin(9600);
    lastConnectionTime = 0; 
    ConectaWiFi();
    Serial.println("IOT / UECE 2023");
    Serial.println("Professor: Dr. Rafael Lopes Gomes and Dr. Marcial Porto Fernandez");
    Serial.println("Student: Eng. Esp. Marcos Camargo ");
}
 
void loop()
{
    float percentual_ppmLida;
    int percentual_ppmTruncada;
    char Asterisco[11];
     
    if (client.connected())
    {
        client.stop();
        Serial.println("O App ThingSpeak foi desconectado.");
        Serial.println();
    }
 
    percentual_ppmLida = LeituraGLP();
    percentual_ppmTruncada = (int)percentual_ppmLida;
     
    if(!client.connected() && 
      (millis() - lastConnectionTime > INTERVALO_DADOS))
    {
        sprintf(Asterisco,"field1=%d",percentual_ppmTruncada);
        EnviaInformacoesThingspeak(Asterisco);
    }
 
     delay(10000);
}
