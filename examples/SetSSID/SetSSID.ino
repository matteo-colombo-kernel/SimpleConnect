#include <SimpleConnect.h>

void setup()
{
    Serial.begin(115200);

    // Initialize with hostname, SSID, and passphrase
    SimpleConnect.init("MyDevice", "MyWiFiNetwork", "MyWiFiPassword");

    // OPTIONAL: Full static IP configuration (uncomment if needed)
    /*
    SimpleConnect.init(
      "MyDevice",
      "MyWiFiNetwork",
      "MyWiFiPassword",
      IPAddress(192, 168, 1, 42),
      IPAddress(192, 168, 1, 1),
      IPAddress(255, 255, 255, 0)
    );
    */
}

void loop()
{
    SimpleConnect.loop();
}
