#include <Helper.h>

Helper *helper;

void setup()
{
    helper=new Helper();

    if(helper->WIFI_setup())
    {
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println("Not connected. Code: "+WiFi.status());
    }

}

void loop() 
{
    helper->update_buildin_led();
}