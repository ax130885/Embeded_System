# Peripheral週邊(STM32) 和 Central(RPi)
### 【實驗步驟】
**1. import program**
在Mbed Studio 中 import empty os  
add lib: https://os.mbed.com/teams/ST/code/BSP_B-L475E-IOT01/  

下載網址裡面的main.cpp取代empty os的: https://github.com/ARMmbed/mbed-os-example-ble/tree/development/BLE_GattServer_AddService  
也可以使用downgit下載整個專案(不知道為甚麼沒辦法直接import)(如果要使用要去把mbed_app.json刪掉或使用其他專案的mbed_app.json取代 否則會無法編譯)  

參考這兩個檔案撰寫自己的service
mbed-os/connectivity/FEATURE_BLE/include/ble/services/EnvironmentalService.h // 程式碼較簡單
mbed-os/connectivity/FEATURE_BLE/include/ble/services/HeartRateService.h // 程式碼較難

修改main.cpp和上次作業中在RPi執行的py檔案

**2. 復現方式**
new新的empty os  
add lib: https://os.mbed.com/teams/ST/code/BSP_B-L475E-IOT01/  
將本專案附的"main.cpp", "HeartRateService.h", "MagnetoService.h", "pretty_printer.h"直接複製進去
在RPi執行ble_scan_connect-1.py  
找到裝置名稱為Heartrate的連接即可

**3. 其他實作server方式**
預設為計時的服務:https://os.mbed.com/teams/mbed-os-examples/code/mbed-os-example-ble-GattServer/

**4. 實作結果**