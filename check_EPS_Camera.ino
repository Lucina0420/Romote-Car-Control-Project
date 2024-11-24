#include <esp_system.h>
#include <esp_camera.h> 

//Camera related constants
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void CheckEsp()
{
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  Serial.println("\nESP32 Model Information:");
  Serial.printf("Chip Model: %s\n", CONFIG_IDF_TARGET);
  Serial.printf("Chip Cores: %d\n", chip_info.cores);
  Serial.printf("Chip Revision: %d\n", chip_info.revision);
  Serial.printf("Has PSRAM: %s\n", (chip_info.features & CHIP_FEATURE_EMB_PSRAM) ? "Yes" : "No");
}

void CheckCamera()
{
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_2;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_RGB565;
  
  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  // Init camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }
  Serial.println("Camera init succeeded");

  // Print chip info
  Serial.println("ESP32 Model Information:");
  Serial.print("Chip Model: ");
  Serial.println(ESP.getChipModel());       // Get Chip model
  Serial.print("Chip Revision: ");
  Serial.println(ESP.getChipRevision());    // get chip revision
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());        // get CPU frequence
  Serial.println(" MHz");

  sensor_t *sensor = esp_camera_sensor_get();
  if (sensor) {
    Serial.println("Camera sensor detected!");
    Serial.print("Sensor PID: 0x");
    Serial.println(sensor->id.PID, HEX);    // Get Camera sensor ID
  } else {
    Serial.println("Failed to get sensor info!");
  }
  
  // Get Camera info
  sensor_t *s = esp_camera_sensor_get();
  if (s != nullptr) {
    Serial.println("\nCamera Information:");
    uint16_t manufacturer_id = (s->id.MIDH << 8) | s->id.MIDL;  
    Serial.printf("Camera Manufacturer ID: 0x%04x\n", manufacturer_id);

    // Get manufacturer ID 
    if (manufacturer_id == 0x7FA2) {
      Serial.println("Camera Manufacturer: OmniVision");
    } else {
      Serial.println("Camera Manufacturer: Unknown");
    }

    Serial.printf("Camera Model ID: 0x%04x\n", s->id.PID);

    // Print camera model
    if (s->id.PID == 0x2642) {
      Serial.println("Detected Camera Model: OV2640");
    } else if (s->id.PID == 0x7725) {
      Serial.println("Detected Camera Model: OV7725");
    } else {
      Serial.println("Detected Camera Model: Unknown");
    }
  } else {
    Serial.println("Failed to retrieve camera sensor information");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  CheckEsp(); 
  CheckCamera(); 
}

void loop() {
  // 
}
