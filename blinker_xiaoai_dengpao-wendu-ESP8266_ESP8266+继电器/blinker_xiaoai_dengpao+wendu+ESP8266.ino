#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET   //小爱同学

#include <Blinker.h>
#include <DHT.h>

char auth[] = "3569de57b901";
char ssid[] = "happy";
char pswd[] = "66666666";
BlinkerButton Button1("btn-on");     //定义按钮数据
BlinkerButton Button2("btn-off"); 
BlinkerButton Button3("btn-aaa");
BlinkerNumber HUMI("humi");    //定义湿度数据键名
BlinkerNumber TEMP("temp");    //定义温度数据键名 
#define DHTPIN 2      //定义DHT11模块连接管脚io2
 
#define DHTTYPE DHT11   // 使用DHT 11温度湿度模块 
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
DHT dht(DHTPIN, DHTTYPE);    //定义dht
 
float humi_read = 0, temp_read = 0; 
bool oState = false;
int counter = 0;
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {     //小爱同学控制开命令
        digitalWrite(0, LOW);

        BlinkerMIOT.powerState("on");

        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {   //小爱同学控制关命令
        digitalWrite(0,HIGH);

        BlinkerMIOT.powerState("off");

        BlinkerMIOT.print();

        oState = false;
    }
}
void heartbeat()
{
    HUMI.print(humi_read);        //给blinkerapp回传湿度数据
    TEMP.print(temp_read);        //给blinkerapp回传温度数据
}

void miotQuery(int32_t queryCode)      //小爱同学控制
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);
    int humi_read_int=humi_read;     //去掉湿度浮点
    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
    
    BlinkerMIOT.humi(humi_read_int);     //小爱接收湿度
    BlinkerMIOT.temp(temp_read);      //小爱接收温度
    BlinkerMIOT.print();
}

void dataRead(const String & data)      // 如果未绑定的组件被触发，则会执行其中内容
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}

void button1_callback(const String & state)     //点灯app内控制按键触发
{
    
    digitalWrite(0,LOW);
    BLINKER_LOG("get button state:on", state);


}
void button2_callback(const String & state)     //点灯app内控制按键触发
{
    
    digitalWrite(0,HIGH);
    BLINKER_LOG("get button state:off", state);


}

void button3_callback(const String & state)     //点灯app内控制按键触发
{  
  
    BLINKER_LOG("get button state:on", state);
     Blinker.print(state);
    if(state=="on"){
    digitalWrite(0,LOW);
   // Button3.print("on");
     }else if(state =="off"){
            digitalWrite(0,HIGH);
        
    //        Button3.print("off");
    }


}



void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(0, OUTPUT);              //定义io口为输出
    digitalWrite(0, LOW);           //定义io默认为高电平

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    dht.begin();       
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
    Button1.attach(button1_callback);  
    Button2.attach(button2_callback);     
    Button3.attach(button3_callback); 
 
}

void loop()
{
    Blinker.run();

    float h = dht.readHumidity();
    float t = dht.readTemperature();
 
    if (isnan(h) || isnan(t))
    {
        BLINKER_LOG("Failed to read from DHT sensor!");
        Blinker.delay(2000);
    }
    else
    {
        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        humi_read = h;
        temp_read = t;
        Blinker.delay(2000);
    }
}
