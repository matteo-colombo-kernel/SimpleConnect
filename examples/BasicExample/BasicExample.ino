#include <SimpleConnect.h>

void setup()
{
    Serial.begin(115200);

    // Initialize SimpleConnect with only hostname
    SimpleConnect.init("MyDevice");
}

void loop()
{
    SimpleConnect.loop();
}
