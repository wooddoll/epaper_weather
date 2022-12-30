## e-paper(GDEW042T2)와 esp32(wemos R1 D32)를 이용해서 openweather.com에서 받아온 날씨 정보를 표시한다.

'G6EJD/ESP32-e-Paper-Weather-Display'를 사용하려고 했지만, e-paper가 4색 표현이 가능한데 bw 2색으로만 사용하고 있어서 약간 변경하기로 했다.
4색에서 글 표시가 가능하도록 'sookmook/EURK_Arduino'를 약간 수정해서 추가했는데,
본래 ui가 12px와 24px font를 사용하는데 반해 한글 폰트는 16px로 다른 크기를 추가하기 어려워서 결국 전체 ui를 모두 수정했다.

1. 400x300 해상도에서 회색조 4색 - 흑백+회색2 - 으로 한글을 사용해서 날씨 정보를 표시
2. platformio에서 arduino모드로 빌드했음
3. wifi설정명 openweather api key등의 설정은 /data/credentials.json 에서 수정

![Alt text](/images/sample.jpg)

ref. 
1. https://github.com/G6EJD/ESP32-e-Paper-Weather-Display
2. https://github.com/sookmook/EURK_Arduino
3. https://github.com/adafruit/Adafruit-GFX-Library

***
## 3d cad file upload; .step
![Alt text](/images/3dprinting-20221229-163515-001.png)

