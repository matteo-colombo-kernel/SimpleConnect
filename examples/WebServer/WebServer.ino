#include <SimpleConnect.h>

void setup()
{
    Serial.begin(115200);

    // Initialize with only hostname
    SimpleConnect.init("MyDevice");

    // Add a simple web server endpoint
    SimpleConnect.webServer.on("/hello", []()
                               { SimpleConnect.webServer.send(200, "text/plain", "Hello from SimpleConnect!"); });
}

void loop()
{
    SimpleConnect.loop();
}
