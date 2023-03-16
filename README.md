#PianoButLDR

##Project description
###This project has been made for 
* Course name : 01204114-65 Introduction to Computer Hardware Development 
* Department of Computer Engineering, Faculty of Engineering
* Kasetsart University

##Our team 
* พงศธร รักงาน 6510503531
* พิชญา เอี่ยมกล้าหาญ 6510503620
* ภาณุพงศ์ เลิศวีรนนทรัตน์ 6510503671
* วรพล วัลลิภากร 6510503751

##อุปกรณ์ที่ใช้
###บอร์ดแรก
* ESP32-S3 1 ตัว
* LDR 9 ตัว
* ตัวต้านทาน 10K 9 ตัว
###บอร์ดสอง
* ESP32-S3 1 ตัว
* Button 3 ตัว
* ตัวต้านทาน 10K 3 ตัว
* OLED 1 ตัว

##Requirement
```sh
#requirements.txt
paho-mqtt==1.6.1
pygame==2.1.3
pyserial==3.5
```

##Directory
* esp32/ : จะมี folder  sketch ใช้สำหรับ upload ขึ้นบอร์ด esp32 ทั้ง 2 ตัว
* esp32/example_sketch/config.h : คือไฟล์ที่จำเป็นต้องแก้ไขตามด้านในไฟล์
* config.py : คือไฟล์ที่จำเป็นต้องแก้ไขตามด้านในไฟล์
* requirement.txt : library ที่จำเป็นสำหรับ main.py สามารถลงได้ตามขั้นตอนด้านล่าง
* main.py : ไฟล์หลักสำหรับทำให้เปียโนเริ่มทำงาน
* audio/ : หากต้องการเพิ่มเครื่องดนตรี หรือเสียงต่างๆ สามารถเพิ่มได้ที่ audio/ 
* flows.json : flows การทำงานของ node red ที่ export มา สามารถนำไป import เข้า node red ของคุณได้ 

## Getting Started

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/inspirezuza/PianoButLDR
   ```

2. Install libraries from requirements.txt
   ```sh
   pip install -r requirements.txt
   ```

3. Enter your mqtt config in `config.py and esp32\sketch_piano\config.h` here example
   ```python
    #config.py
    PORT = "COM4"
    DEFAULT_INSTRUMENT = "piano"
    MQTT_BROKER = "iot.cpe.ku.ac.th"
    MQTT_USER = "b6510123456" 
    MQTT_PASS = "example.example@ku.th"
    TOPIC_PREFIX = "b6510123456"
    IOT_PORT = 1883
   ```
   ```c
    //config.h
    #define WIFI_SSID    "wifiname"
    #define WIFI_PASS    "12345678"
    #define MQTT_BROKER  "iot.cpe.ku.ac.th"
    #define MQTT_USER    "b6510123456"  
    #define MQTT_PASS    "example.example@ku.th"      
    #define TOPIC_PREFIX "b6510123456"  
   ```

4. Upload code from esp32\sketch_piano\sketch_piano.ino to your esp32s3 dev

5. Run main.py here example code for window
   ```sh
   python main.py
   ```

6. Open node red if you on 01204114-65 course open this link and login
* [iot.cpe.ku.ac.th/red/b65101234567](iot.cpe.ku.ac.th/red/b65101234567) 

7. Import flows.json don't forget to change username (b6510123456)

8. Deploy and open dashboard

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Some list resources that I find helpful and would like to give credit to.

* [Instruments mp3 files](https://github.com/gleitz/midi-js-soundfonts/tree/gh-pages/FatBoy)