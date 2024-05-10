# DHT11 溫溼度感測
## 【基本資料】
作業: HW8  
姓名: 林育新  
學號: R12631070  
GitHub: https://github.com/ax130885/Embeded_System/tree/main/hw08_DHT11%E6%BA%AB%E6%BA%BC%E5%BA%A6%E6%84%9F%E6%B8%AC  

## 【實驗題目】
1. Observe the DHT11 signal using a Logic  Analyzer.  Show some screen shots and steps in your lab.
2. Explain or discuss the following terms:
   1. What is Linux IIO subsystem? Please explain IIO in terms of its purpose and user interface provision.  
      (references: https://www.kernel.org/doc/html/latest/driver-api/iio/index.html)  
      (To access IIO devices, Linux application can use libiio: https://analogdevicesinc.github.io/libiio/v0.20/index.html)  
   2. ow is the efficiency difference when compared between interrupt-driven I/O and programming I/O (polling I/O)?  
<br/>

## 【問題回答與實作】
#### 使用邏輯分析儀觀察 DHT11 訊號。顯示實驗中的一些螢幕截圖和步驟。
````python
# 全程在RPI當中操作
# CircuitPython-DHT Library is based on CircuitPython Library
sudo pip3 install --upgrade setuptools
sudo pip3 install --upgrade adafruit-python-shell
wget https://raw.githubusercontent.com/adafruit/Raspberry-Pi-
Installer-Scripts/master/raspi-blinka.py
sudo python3 raspi-blinka.py

# Installing the CircuitPython-DHT Library
sudo apt-get install libgpiod2
sudo pip3 install adafruit-circuitpython-dht

# Testing the CircuitPython DHT Library
git clone https://github.com/adafruit/Adafruit_CircuitPython_DHT.git
cd Adafruit_CircuitPython_DHT

# 打開 examples/dht_simpletest.py 更改一開始dhtDevice的定義為
dhtDevice = adafruit_dht.DHT11(board.D4, use_pulseio=False)

# 用python程式持續讀值
python examples/dht_simpletest.py 
````
````bash
vim /boot/config.txt
# 添加這一行在文件最後 ctrl+F到最後 :o插入下一行 修改完成以後:wq保存並退出
dtoverlay=dht11
sudo reboot

# 重開之後
cd /sys/bus/iio/devices/iio:device0.
cat in_temp_input # 即可得到溫度感測的值
cat in_humidityrelative_input # 即可得到濕度的值
# 如果cat顯示IO錯誤 是DHT11版本的問題
````
````
邏輯分析儀設定
memory size = 256K
sample rate = 1M/Hz  (1us/sample)
zoom = 1%
觸發條件 raise edge
觸發次數 2
````
<!-- 如果要做1x4的表格 -->
<!--
| ![](fig/result1.png) | ![](fig/result2.png) | ![](fig/result3.png) | ![](fig/result4.png) |
| :------------------: | :------------------: | :------------------: | :------------------: |
-->
| ![](fig/result1.png) | ![](fig/result2.png) |
| :------------------: | :------------------: |
| ![](fig/result3.png) | ![](fig/result4.png) |

<div align="center">Fig. 特別長的High=1;而較短的Hight代表0</div>
<br/>

#### 什麼是Linux IIO子系統？請解釋 IIO 的用途和使用者介面。
Linux IIO（工業輸入/輸出）子系統是用來處理各種感測器和ADC（類比數位轉換器）等工業輸入/輸出設備的架構。它提供了一個標準化的方式來存取這些設備，包括溫度感測器、加速度計、壓力感測器等，以及用於讀取和控制這些設備的API。  
* IIO的用途
感測器資料收集：IIO可以用來從各種感測器（如溫度、濕度、光線等）獲取資料。
ADC資料收集：對類比信號進行數位轉換，用於讀取類比感測器或外部設備的資料。
資料處理：IIO提供了一些資料處理功能，如資料濾波、資料校準等。
驅動程式抽象：它為開發者提供了一個抽象層，使他們可以編寫通用的驅動程式，而不必關心底層硬體細節。  
* 使用者介面
IIO的使用者介面主要是透過sysfs（系統檔案系統）和使用者空間庫來實現的。使用者可以透過sysfs檔案系統中的節點來存取IIO設備，包括讀取感測器資料、配置感測器參數等。此外，使用者還可以使用IIO庫來更方便地存取IIO設備，這些庫提供了一些函式和工具，簡化了與IIO設備的互動過程。  
#### 中斷和輪詢I/O之間的效率差異如何？
* 傳輸時間可預測的情況下，輪詢的效率比較好。例如，固定周期從感測器讀取資料。
* 中斷在數據傳輸不可預測或大量傳輸的情況下是有效的。例如，從網路當中接收封包，時間和大小可能會有很大差異。中斷驅動系統允許 CPU 執行其他任務，直到封包到達，它會向CPU發送中斷，然後CPU再去處理此ISR。

<br/>

## 【實驗討論】
因為鄰近課程尾聲，要開始做期末專題，這次實驗是課程以來最簡單的一次。不過在實作過程中還是有遇到一點小問題，就是我自己也有其他的DHT11模組。在使用老師提供的程式時，不知道為甚麼，會辨識不到I/O。但是用先前原本DHT11的程式，其實還是能夠正常執行，證明我原本的DHT11是沒有壞的。並且老師新給的DHT11，也能在我先前的程式當中順利執行。可能是DHT11版本的問題導致的，但IC封裝並沒有產品的型號，實在很難判斷到底是什麼問題。