#include <Helper.h>

Helper::Helper()
{

    Serial.begin(9600);
    Serial.flush();

    ledcSetup(0,5000,8);
    ledcAttachPin(ONBOARD_LED,0);
    buildin_led=ledcRead(0);

}

void Helper::update_buildin_led()
{
    if(buildin_led) {
        ledcWrite(0,buildin_led_brightness);
    } else {
        ledcWrite(0,0);
    }    
}

void Helper::switch_buildin_led()
{
    buildin_led=ledcRead(0);
    buildin_led=buildin_led!=true;
}

void Helper::dim_buildin_led(int brightness)
{
    buildin_led_brightness=max(0,min(brightness,255));
}

void Helper::send_handshake_packet(AsyncUDPPacket& packet)
{
    stringstream ss;
    ss << handshake_msg << ':' << ledcRead(0);

    string s=ss.str();
    udp->writeTo((const uint8_t*)s.c_str(),s.size(),packet.remoteIP(),19680);

}

void Helper::create_udb_server()
{
    udp=new AsyncUDP();

    if(udp->listen(UDP_PORT))
    {
        udp->onPacket([&](AsyncUDPPacket packet)
        {
            Serial.println((const char*)packet.data());
            Helper::UDP_COMMAND cmd=NONE;

            const char* data=nullptr;
            string data_str=(const char*)packet.data();
            data_str.resize(packet.length());
          
            int pos=data_str.find(":");
            if(pos!=string::npos) {
                cmd=UDP_COMMAND(atoi(data_str.substr(0,pos).c_str()));
                data=data_str.substr(pos+1).c_str();
            } else {
                cmd=UDP_COMMAND(atoi(data_str.c_str()));
            }

            switch(cmd)
            {
                case BUILDIN_LED_SWITCH:
                    Helper::switch_buildin_led();
                    break;
                case BUILDIN_LED_DIMM:
                    Helper::dim_buildin_led(atoi(data));
                    break;
                case RGBSTRIP_SWITCH:
                    break;
                case RGBSTRIP_DIMM:
                    break;
                case HANDSHAKE:
                    Helper::send_handshake_packet(packet);
                    break;

                default:
                    break;
            }            

        });

    }
}

bool Helper::WIFI_setup()
{
    try
    {
        WiFi.disconnect();
        WiFi.softAPdisconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAP(WIFI_SSID,WIFI_PWD,1,1,1);
        WiFi.softAPConfig(IPAddress((const uint8_t*)ip),IPAddress((const uint8_t*)ip),IPAddress((const uint8_t*)sub));
        create_udb_server();
    }
    catch(const std::exception& e)
    {
        Serial.println(e.what());
        return false;
    }
    
    return true;
}

