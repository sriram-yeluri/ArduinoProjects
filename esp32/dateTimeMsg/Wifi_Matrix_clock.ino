#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <WiFi.h>
#include <time.h>
#include <SPI.h>

// WiFi credentials
const char* ssid = "xxxxxxx";
const char* password = "xxxxxxxx";

// Define NTP Settings
const char* ntpServer          = "pool.ntp.org";
const long  gmtOffset_sec      = 19800; // Change to your timezone offset in seconds (e.g., 19800 for GMT+5:30)
const int   daylightOffset_sec = 0;     // Change if your area observes Daylight Saving Time

// Global char buffer to hold formatted date and time text
char msgBuffer[40];
char timeBuffer[40];

// MAX7219 config
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8

#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Text to be displayed (can be changed according to needs)
const char* runningText = "Devaki Devi is the Queen of our house";

unsigned long lastSwitch = 0;
int mode = 0; // 0: jam, 1: running text
const int switchInterval = 12000; 


// Array containing your sequential messages
const char *messages[] = {
  "Devaki Devi is the Queen of our house",
  "VijayKrishnaSai",
  "Vishwateja",
  "Sriram"
};

// Calculate the number of items in the message array
const uint8_t totalMessages = sizeof(messages) / sizeof(messages[0]);
uint8_t currentMsgIndex = 0; // Tracks the current active message

void obtainTime() {
  struct tm timeinfo;

  // Try to obtain the local system time
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    display.print("Err Time");
    delay(1000);
    return;
  }
  // Format the date/time string. 
  // Custom format examples: 
  // "%d/%m/%Y" -> 31/07/2026
  // "%A, %b %d" -> Friday, Jul 31
  strftime(timeBuffer, sizeof(timeBuffer), "%H: %M: %S", &timeinfo);
  display.displayText(timeBuffer, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
}

void obtainDate() {
  struct tm timeinfo;

  // Try to obtain the local system time
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    display.print("Err Time");
    delay(1000);
    return;
  }
  // Format the date/time string. 
  // Custom format examples: 
  // "%d/%m/%Y" -> 31/07/2026
  // "%A, %b %d" -> Friday, Jul 31
  strftime(msgBuffer, sizeof(msgBuffer), "%A, %b %d", &timeinfo);
  // display.print(msgBuffer);
  if (display.displayAnimate()){
    display.displayScroll(msgBuffer, PA_LEFT, PA_SCROLL_LEFT, 50);
    display.displayReset();
  }
}

void displayMsg(){
  display.displayScroll(runningText, PA_LEFT, PA_SCROLL_LEFT, 50);
}

void setup() {
  Serial.begin(115200);
  display.begin();            // Initialize display
  display.setIntensity(8);    // Set brightness (0 to 15)
  display.displayClear();     // // Clear initial artifacts

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // Synchronize internal clock with NTP Server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  display.print("NTP clock Sync");
  delay(2000); 

  // Geolocation API endpoint
  // const char* GEOLOCATION_API = "http://ip-api.com/json/?fields=country,city,lat,lon,timezone,offset";

  // Show the scroll while waiting for available time
  display.displayScroll("Loading the time ...", PA_LEFT, PA_SCROLL_LEFT, 50);
  while (!time(nullptr)) {
    delay(1000);
  }
  display.displayClear();
}

void loop() {

  displayMsg();
  // Logic to switch between dateTime and scrolling text message
  unsigned long nowMillis = millis();

  // Change the mode every few seconds
  if (nowMillis - lastSwitch > switchInterval) {
    mode = 1 - mode; // toggle antara 0 dan 1
    lastSwitch = nowMillis;
    display.displayClear();
  }

  if (mode == 0) {
      // Digital clock mode
    static char timeStr[9]; // "HH:MM:SS"
    time_t now = time(nullptr);
    struct tm* tm_info = localtime(&now);
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);

    display.setTextAlignment(PA_CENTER);
    display.print(timeStr);

    delay(1000); // update time every second
    }else {
    obtainDate();
  }
}
