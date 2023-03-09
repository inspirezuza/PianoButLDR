Best-README-Template/pull/73 -->
<a name="readme-top"></a>


<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown from this link XD
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

<!-- PROJECT LOGO -->
<br />
<div align="center">

  <h3 align="center">PianoButLDR</h3>

  <p align="center">
    Wanna play piano using LDR sensor?

    This project is for 01204114-65 Introduction to Computer Hardware Development (Kasetsart University first year cpe course)
  </p>
</div>

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
    PORT = "COM4"
    DEFAULT_INSTRUMENT = "piano"
    MQTT_BROKER = "iot.cpe.ku.ac.th"
    MQTT_USER = "b6510123456" 
    MQTT_PASS = "example.example@ku.th"
    TOPIC_PREFIX = "b6510123456"
    IOT_PORT = 1883
   ```
   ```c
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

6. Open node red if you on 01204114-65 course open this link and login iot.cpe.ku.ac.th/red/b65101234567
Node Red Link: [iot.cpe.ku.ac.th/red/b65101234567](iot.cpe.ku.ac.th/red/b65101234567) 

7. Import flows.json don't forget to change username (b6510123456)

8. Deploy and open dashboard

9. Enjoy


<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Some list resources that I find helpful and would like to give credit to.

* [Instruments mp3](https://github.com/gleitz/midi-js-soundfonts/tree/gh-pages/FatBoy)

<p align="right">(<a href="#readme-top">back to top</a>)</p>
