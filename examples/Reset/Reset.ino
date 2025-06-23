#include <SimpleConnect.h>

const int buttonPin = 4; // Put your pin number

void setup()
{
    Serial.begin(115200);

    pinMode(buttonPin, INPUT_PULLUP);

    // Initialize SimpleConnect
    SimpleConnect.init("MyDevice");
}

void loop()
{
    SimpleConnect.loop();

    // Example condition: press the button to reset credentials
    if (digitalRead(buttonPin) == LOW)
    {
        Serial.println("Resetting saved WiFi credentials...");
        SimpleConnect.reset();

        // IMPORTANT: You must reboot the ESP32 after reset
        Serial.println("Rebooting device...");
        delay(1000);
        ESP.restart();
    }
}
