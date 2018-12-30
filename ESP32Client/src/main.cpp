#include <Arduino.h>

                                                // 定时器
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "LEO_ASUS";
const char* password =  "bhu8vgy7nji9";
int LED_BUILTIN = 2;
// 默认Wi-Fi 未连接
wl_status_t WiFiStatus = WL_DISCONNECTED;

// 500ms定时任务
void taskLED(void * parameter )
{
    int i = 0;
    pinMode(LED_BUILTIN, OUTPUT);
    delay(500);
    while(1)
    {
        Serial.printf("taskLED: %d\r\n", i++);
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(500);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        delay(500);  
    }
}

// 500ms定时任务
void taskWiFi(void * parameter )
{
    int i;
    delay(500);
    while(1)
    {
    
        if (WiFi.isConnected() == false)
        {

            while (WiFi.status() != WL_CONNECTED) 
            {
                delay(1000);
                Serial.println("Connecting to WiFi..");
            }
        }
    }
}

void setup() 
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    delay(200);
    Serial.println("ESP32 Client Start……");
    xTaskCreate(taskLED, "taskLED", 2000, NULL, 1, NULL);
    xTaskCreate(taskWiFi, "taskWiFi", 8000, NULL, 2, NULL);

}

void loop() 
{
    // /
    // if (WiFi.isConnected() == false)
    // {
    //     while (WiFi.status() != WL_CONNECTED) 
    //     {
    //         delay(1000);
    //         Serial.println("Connecting to WiFi..");
    //     }
    // }


}