# SimpleConnect

**SimpleConnect** is a ESP32 library that simplifies Wi-Fi connection management of your microcontroller.

With just two lines of code, you can add Wi-Fi capabilities to your ESP32 project, and also make it reachable with the address `http://<yourHostname>.local`:

```cpp
#include <SimpleConnect.h>

void setup() {
  // This will spin-up an access point where you can connect in order to choose a Wi-Fi network
  SimpleConnect.init("yourHostname");

  // Or, if you want to connect to a specific Wi-Fi network:
  // SimpleConnect.init("yourHostname", "wiFiName", "wiFiPassphrase");
}

void loop() {
  SimpleConnect.loop();
}
```

## Features

Just with the code above:

- You can provide hardcoded Wi-Fi credentials (SSID and passphrase), or you can manage connection credentials during runtime with a web interface provided by this library
- The connection status is continuously monitored, and if disconnected, reconnection attempts are automatically and actively handled, in a more active way than the native ESP Wi-Fi implementation
- If no SSID is provided or a connection fails, the device automatically falls back to Access Point mode.
- In Access Point mode, connected devices are presented with a [captive portal](https://en.wikipedia.org/wiki/Captive_portal), redirecting users to a built-in Wi-Fi settings page.
- The microcontroller becomes accessible via [mDNS](https://en.wikipedia.org/wiki/Multicast_DNS), allowing other devices to reach it using <yourHostname>.local, where yourHostname is the one you passed to SimpleConnect.init().
- The Wi-Fi settings page lets users connect to new networks and manage the current connection.

The library manages an internal `WebServer` instance on port 80, which is exposed for your use. You can register your own routes without interfering with the captive portal or Wi-Fi management interface using this exposed instance (see below).

## Dependencies

This library depends on:

- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

The web interface is based on the built-in [`WebServer`](https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer) class from the [ESP32 Arduino core](https://github.com/espressif/arduino-esp32), so **no external web server library is required**.

These dependencies are taken care of by PlatformIO or the Arduino Library Manager, but you may want to install them manually if you are using ArduinoIDE and manually including this library in the `libraries/` folder.

## Installation

1. Download or clone this repository;
2. copy it into your Arduino `libraries/` folder, or in your PlatformIO project's `lib/` subfolder.

## Usage

The only header file to be included is `SimpleConnect.h`. All dependencies are managed by the library:

```cpp
#include <SimpleConnect.h>
```

The minimal set of functions to call is:

- the `SimpleConnect.init()` function inside your `setup()`, which requires at least one argument (the hostname), but can also accept the SSID, if you want to hardcode the Wi-Fi network, the passphrase, if the hardcoded network is secured, and three IP addresses if you want to manually set the IP address of the microcontroller, the IP address of the gateway, and the subnet mask;
- the `SimpleConnect.loop()` function inside your `loop()`, with no arguments, which takes care of runtime operations.

The provided hostname is used to generate the mDNS address (such as `yourHostname.local`)

```cpp
// In your setup():

// FASTEST METHOD: initialize without hardcoding Wi-Fi credentials
// You'll be able to connect to Wi-Fi using the web interface
SimpleConnect.init("yourHostname");

// OR: Initialize and hardcode an open Wi-Fi network
SimpleConnect.init("yourHostname", "openWiFiName");

// OR: Initialize and hardcode a secured Wi-Fi network
SimpleConnect.init("yourHostname", "securedWiFiName", "securedWiFiPassphrase");

// ADVANCED: initialize and hardcode a secured Wi-Fi network, with manual IP settings
// This is needed if your network doesn’t assign those values automatically via DHCP
SimpleConnect.init(
  "yourHostname", "securedWiFiName", "securedWiFiPassphrase",
  IPAddress(192, 168, 0, 50), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0)
);
```

To keep the functionalities on, you also need to put the `SimpleConnect.loop()` function in the `loop()` of your project:

```cpp
// In your loop():

SimpleConnect.loop();
```

### Access point and web interface

If no network is provided on init, and/or if unable to connect to the saved network, the library automatically spins up an Access Point, so that you can use another device (e.g. a smartphone) to connect to the microcontroller via the Access Point and visit the Wi-Fi settings page.

Since this library conforms to the captive portal detection systems of major vendors, you'll see a notification inviting you to manage the Wi-Fi settings as soon as you connect your device to the Access Point.

If the notification doesn't appear, you can directly visit `http://192.168.0.1/wifi/settings` to get to the Wi-Fi configuration web interface.

On the web interface, you can:

- connect to available networks
- forget the saved network, so that you can connect to a new one
- see the status of the Access Point, if it is currently on

Note that the web interface is also available when the Access Point is off and the microcontroller is successfully connected to a Wi-Fi network: it provides an interface to manage the current connection.

> TODO: add screenshots of the web interface

### Adding endpoints to the web server

This library makes use of a web server to serve the Wi-Fi settings page and related APIs. As a result, if you want to serve other pages on this microcontroller, you **SHOULD NOT initialize another web server**, but instead use the web server instance used by this library.

An instance of `WebServer` (from the ESP32 Arduino core) is exposed on port 80. You can use this instance to register additional endpoints using `on(...)`, `send(...)`, and other standard methods. For more details, refer to the [`WebServer` documentation](https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer).

The instance is already initialized, therefore you **don't need to initialize** the server with further methods.

```cpp
// No need to import additional libraries

// In your setup(), use "SimpleConnect.webServer" to handle your endpoints:
SimpleConnect.webServer.on("/", []() {
  SimpleConnect.webServer.send(200, "text/plain", "Hello world!");
});

```

## Methods and properties

### `SimpleConnect.init()`

```cpp
void init(const char* hostname, const char* ssid = "", const char* passphrase = "", IPAddress IP = IPAddress(), IPAddress gateway = IPAddress(), IPAddress subnet = IPAddress())
```

Initializes the Wi-Fi connection manager with _optional_ SSID, passphrase and static IP config.

### `SimpleConnect.isAPStarted()`

```cpp
bool isAPStarted() const
```

Returns true if the fallback Access Point is currently active.

### `SimpleConnect.isConnected()`

```cpp
bool isConnected() const
```

Returns true if the device is currently connected to a Wi-Fi network.

### `SimpleConnect.reset()`

```cpp
void reset() const
```

Delete saved credentials and settings from memory. **You still need to reboot the microcontroller to disconnect from current WiFi**.

### `SimpleConnect.loop()`

```cpp
void loop()
```

Call this periodically (e.g. in loop()) to handle connection state, reconnections, start the AP fallback if needed, and handle the web server calls. The .loop() function also takes care of keeping the web server responding.

### `SimpleConnect.webServer`

An instance of [`WebServer`](https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer) on port 80. Refer to that library documentation for available methods.

## Advanced | Setting a custom log handler

By default, SimpleConnect logs messages to the Serial console. You can customize logging by providing your own handler using setLogger().

This allows you to redirect logs to another output (e.g., file, remote server, on-screen display, etc.) or filter them based on severity.

```cpp
void myLogger(const String &message, int level) { // Level is an integer between 0 and 7
  Serial.print("[MY LOG LEVEL ");
  Serial.print(level);
  Serial.print("] ");
  Serial.println(message);
}

void setup() {
  SimpleConnect.setLogger(myLogger);
  SimpleConnect.init("yourHostname");
}
```

## Advanced | API endpoints

The web interface uses some API endpoints that you can also use if you want to provide your implementation.

| **Endpoint**        | **Method** | **Description**                                     |
| ------------------- | ---------- | --------------------------------------------------- |
| `/wifi/api/status`  | `GET`      | Returns current Wi-Fi and AP status as JSON.        |
| `/wifi/api/scan`    | `GET`      | Scans and returns available Wi-Fi networks as JSON. |
| `/wifi/api/connect` | `POST`     | Connects to a new Wi-Fi network.                    |
| `/wifi/api/forget`  | `POST`     | Forgets stored Wi-Fi credentials.                   |

### Status

By calling this endpoint, you get such a response if the Wi-Fi network is connected:

```json
[
  {
    "hostname": "yourHostname",
    "ssid": "MyWiFiNetwork",
    "strength": 3,
    "ip": "192.168.1.42",
    "gateway": "192.168.1.1",
    "subnet": "255.255.255.0"
  },
  {
    "active": false,
    "hostname": "yourHostname",
    "ssid": "yourHostname_AP",
    "ip": "",
    "subnet": ""
  }
]
```

Instead, you get such a response if the Wi-Fi network is not connected and thus the Access Point is acive (notice that the SSID could be saved and returned, but the `strength` value is `0`):

```json
{
  "network": {
    "hostname": "yourHostname",
    "ssid": "MyWiFiNetwork",
    "strength": 0,
    "ip": "",
    "gateway": "",
    "subnet": ""
  },
  "ap": {
    "active": true,
    "hostname": "yourHostname",
    "ssid": "yourHostname_AP",
    "ip": "192.168.0.1",
    "subnet": "255.255.255.0"
  }
}
```

Strength is a value from 1 to 4, with 0 meaning disconnected.

### Scan

By calling this endpoint, you get such a response. Please be aware that scanning is a blocking operation, do not abuse!

```json
{
  "networks": [
    {
      "ssid": "HomeNetwork",
      "strength": 4,
      "secure": true
    },
    {
      "ssid": "Cafe_WiFi",
      "strength": 2,
      "secure": false
    },
    {
      "ssid": "OfficeNet",
      "strength": 3,
      "secure": true
    }
  ]
}
```

Strength is a value from 1 to 4.

### Connect

By sending a POST request to this endpoint, you save the provided credentials, so that the microcontroller can attempt to connect.

The body of the request, in JSON, is something like:

```json
{
  "ssid": "YourNetworkName",
  "passphrase": "YourPassword", // (pass an empty string if the network is not secured)
  "ip": "192.168.1.100", // (optional)
  "gateway": "192.168.1.1", // (optional)
  "subnet": "255.255.255.0" // (optional)
}
```

### Forget

This endpoint clears any stored Wi-Fi credentials and IP settings, disconnects from the current Wi-Fi network, and starts the fallback Access Point mode.

No body is required.

## Advanced | Behind the scenes: WebServer methods

As stated above, the library provides a WebServer instance. All WebServer methods are available.

However, behind the scenes, two methods behave a little differently.

### .on

The `.on` method, used to declare endpoints, works exactly like the original WebServer implementation, it prevents overriding reserved endpoints used by the captive portal and Wi-Fi web interface. These are:

- "/wifi/settings",
- "/wifi/api/status",
- "/wifi/api/scan",
- "/wifi/api/connect",
- "/wifi/api/forget",
- "/generate_204",
- "/fwlink",
- "/hotspot-detect.html",
- "/ncsi.txt",
- "/connecttest.txt"

### .onNotFound

The `.onNotFound` method registers a fallback handler for requests that don't match any declared endpoint. In AP mode, this handler is temporarily ignored, and requests are redirected to /wifi/settings to trigger the captive portal. Outside AP mode, your custom handler will be used if set.

## License

This project is licensed under the [MIT License](LICENSE).
