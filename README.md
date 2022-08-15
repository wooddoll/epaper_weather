e-paper(GDEW042T2)와 esp32를 이용해서 openweather.com에서 받아온 날씨 정보를 표시함


1. 400x300 해상도에서 회색조 4색 - 흑백+회색2 - 으로 한글을 사용해서 날씨 정보를 표시
2. platformio에서 arduino모드로 빌드했음
3. wifi설정명 openweather api key등의 설정은 /data/credentials.json 에서 수정

![Alt text](/images/sample.jpg)

ref. 
1. https://github.com/G6EJD/ESP32-e-Paper-Weather-Display
2. https://github.com/sookmook/EURK_Arduino
3. https://github.com/adafruit/Adafruit-GFX-Library
