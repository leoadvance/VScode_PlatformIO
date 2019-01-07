#include <Arduino.h>

                                                // 定时器
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Cleargrass_SZ";
const char* password =  "cleargrass2015";
const char* HTTPServerAddr =  "192.168.1.140";
const char* HTTPServerPort =  "80";

int LED_BUILTIN = 2;
// 默认Wi-Fi 未连接
wl_status_t WiFiStatus = WL_DISCONNECTED;
bool HTTPSendSucdess = false;

#define LED_CLOSE   digitalWrite(LED_BUILTIN, LOW)
#define LED_OPEN   digitalWrite(LED_BUILTIN, HIGH)

// 字符串显示Wi-Fi状态
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
    bool LED_ON = false;
    Serial.printf("taskLED start...\r\n");
    pinMode(LED_BUILTIN, OUTPUT);
    LED_CLOSE;
    delay(500);
    while(1)
    {
        // 联网后蓝灯常亮 
        if (WiFiStatus == WL_CONNECTED)
        {
            // HTTP 不在发送状态 灯常亮
            if (HTTPSendSucdess == false)
            {
                LED_OPEN;
                LED_ON = true;
            }
            else
            {
                // HTTP发送 灯闪烁
                if (LED_ON == false)
                {
                    LED_OPEN; 
                    LED_ON = true;   
                }
                else
                {
                    LED_CLOSE;
                    LED_ON = false;    
                }
            }
            
        }
        else
        {
            LED_CLOSE;
            LED_ON = false;
        }
        delay(500);  
    }
}

// Wi-Fi连接管理
void taskWiFi(void * parameter )
{
    int i = 0;
    int j = 0;
    Serial.printf("taskWiFi start...\r\n");
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

        // 未联网，准备连接Wi-Fi   
        if (WiFiStatus != WL_CONNECTED)
        {
            WiFi.disconnect(true);
            delay(1000);
            Serial.printf("Connecting to Wi-Fi...\r\n");
            WiFi.mode(WIFI_AP_STA);
            delay(500);
            //连接WiFi 并等待连接成功
            WiFi.begin(ssid, password);
            i = 20;
            do
            {
                delay(2000);
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

// HTTP发送测试
void taskHTTP(void * parameter )
{
    char httpURL[256];
    int i = 0, j = 0;
    Serial.printf("taskHTTP start...\r\n");
    while(1)
    {
        // Check the current connection status
        if (WiFiStatus == WL_CONNECTED) 
        { 
            i++;
            HTTPClient http;
            memset(httpURL, 0, 256);
            sprintf(httpURL, "http://%s:%s/?deviceID=Test1&temp=%d&humi=%d", HTTPServerAddr, HTTPServerPort, j, i);
            Serial.printf("httpURL: %s\r\n", httpURL);
            http.begin(httpURL); 
            http.addHeader("Content-Type", "text/plain");  //Specify the URL
            int httpCode = http.GET();                                        //Make the request

            // http发送成功
            if (httpCode == 200) 
            { //Check for the returning code
                String payload = http.getString();
                Serial.println(payload);
                HTTPSendSucdess = true;
                j++;
            }
            else 
            {
                Serial.printf("Error on HTTP request, httpCode = %d\r\n", httpCode);
                HTTPSendSucdess = false;
            }

            http.end(); //Free the resources
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
    xTaskCreate(taskWiFi, "taskWiFi", 16000, NULL, 3, NULL);
    xTaskCreate(taskHTTP, "taskHTTP", 16000, NULL, 2, NULL);

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

    delay(1000);
}