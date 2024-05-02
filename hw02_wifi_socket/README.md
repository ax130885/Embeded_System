**實驗步驟:**
0. source資料夾裡面有兩個main.cpp，有後綴的對應到socket-server.py; 沒有後綴的對應到visual_socket_server.py。兩個main檔案不能同時存在，需要先把另一個檔案刪掉才能編譯。

1. 在 Mbes OS IDE 當中 New “mbed-os-example-sockets” 這個 program。
2. https://os.mbed.com/teams/ST/code/DISCO_L475VG_IOT01-Sensors-BSP/ 是用於sensor 的範例專案，可以直接用此 URL new program。但在此作業中要下滑選擇 BSP_B-L475E-IOT01.lib，再接著往下滑會見到這個網址http://developer.mbed.org/teams/ST/code/BSP_B-L475E-IOT01/#bfe8272ced90，進入後在左邊黃色箭頭下拉選擇 import into mbed studio 複製網址貼到 add library to activateprogram。
3. 打開 mbed_app.json，修改 config.value 和 wifi.ssid (網路名稱)和 wifi.password
4. 打開 targets.json 將"printf_lib": "minimal-printf"改成”std”，否則無法使用浮點數。
5. 新開一個 https://os.mbed.com/teams/ST/code/DISCO_L475VG_IOT01-Sensors-BSP/ 的program，參考 sensor program 的 main.cpp 和上課講義中提供的範例，修改 sockets program 的 main.cpp，使其能送出感測器的訊號。具體修改 main 與 bool send_http_request()裡面的定義。
6. 打開老師給的 socket_server.py 程式，設定裡面的 Host 為 PC 的 IP 地址，port 要與sockets program 裡面 main 當中設定的相同。
7. 先執行 python 程式再執行 socket program 即可得到答案，若先執行 socket 而造成連不上server 會產生 Error! _socket.connect() returned: -3012 的錯誤。