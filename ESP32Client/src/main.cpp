#include <Arduino.h>

                                                // 定时器
#include <WiFi.h>
#include <HTTPClient.h>

// const char* ssid = "LEO_ASUS";
// const char* password =  "bhu8vgy7nji9";
const char* ssid = "LEO's iPhoneX";
const char* password =  "8765432a";
int LED_BUILTIN = 2;
// 默认Wi-Fi 未连接
wl_status_t WiFiStatus = WL_DISCONNECTED;

#define LED_CLOSE   digitalWrite(LED_BUILTIN, LOW)
#define LED_OPEN   digitalWrite(LED_BUILTIN, HIGH)

char *WiFiStatusToString(int status)
{
    switch(status)
    {
        case 0:
            return (char*)"WL_IDLE_STATUS"; 
        case 1:
            return (char*)"WL_NO_SSID_AVAIL"; 
        case 2:
            return (char*)"WL_SCAN_COMPLETED"; 
        case 3:
            return (char*)"WL_CONNECTED"; 
        case 4:
            return (char*)"WL_CONNECT_FAILED"; 
        case 5:
            return (char*)"WL_CONNECTION_LOST"; 
        case 6:
            return (char*)"WL_DISCONNECTED";     

        case 255:

        default:
            return (char*)"WL_STATUS_ERROR";       
    }
}
// 500ms定时任务
void taskLED(void * parameter )
{
    //int i = 0;
    pinMode(LED_BUILTIN, OUTPUT);
    LED_CLOSE;
    delay(500);
    while(1)
    {
        // 联网后蓝灯常亮 
        if (WiFiStatus == WL_CONNECTED)
        {
            LED_OPEN;
        }
        else
        {
            LED_CLOSE;
        }
        delay(1000);  
    }
}

// Wi-Fi连接管理
void taskWiFi(void * parameter )
{
    int i = 0;
    int j = 0;
    delay(500);
    while(1)
    {
        j++;
        // 获取Wi-Fi状态
        WiFiStatus = WiFi.status();
        if (j == 10)
        {
            j = 0;
            Serial.printf("WiFiStatus = %s\r\n", WiFiStatusToString(WiFiStatus));
        }     
        if (WiFiStatus != WL_CONNECTED)
        {
            
            Serial.printf("Connecting to Wi-Fi...\r\n");

            //连接WiFi
            WiFi.begin(ssid, password);
            i = 20;
            do
            {
                delay(1000);
                WiFiStatus = WiFi.status();
                Serial.printf(" WiFi.begin WiFiStatus = %s\r\n", WiFiStatusToString(WiFiStatus));
                i--;

            }while((WiFiStatus != WL_CONNECTED) && (i > 0));

            if (WiFiStatus == WL_CONNECTED)
            {
                Serial.printf("Wi-Fi connect success!\r\n");
            }
        }
        delay(1000);
    }
}

void setup() 
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    delay(200);
    Serial.println("ESP32 Client Start……");
    xTaskCreate(taskLED, "taskLED", 2048, NULL, 1, NULL);
    xTaskCreate(taskWiFi, "taskWiFi", 8192, NULL, 2, NULL);

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