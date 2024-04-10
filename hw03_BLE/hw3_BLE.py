from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate
import time

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)
    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print ("Discovered device", dev.addr)
        elif isNewData:
            print ("Received new data from", dev.addr)
            
scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan (3.0)
n=0
addr = []
for dev in devices:
    print ("%d: Device %s (%s), RSSI=%d dB" % (n, dev.addr , dev.addrType , dev.rssi))
    addr.append(dev.addr)
    n += 1
    for (adtype , desc , value) in dev.getScanData():
        print (" %s = %s" % (desc ,value))
number = input('Enter your device number:')
print ('Device', number)
num= int (number)
print (addr[num])
print ("Connecting...")
dev = Peripheral(addr [num], 'random')
dev.setDelegate(ScanDelegate())
print ("Services...")
for svc in dev.services:
    print (str(svc))

testService = dev.getServiceByUUID (UUID(0x9999))
try:
    ch = testService.getCharacteristics(UUID(0x9191))[0]

    print(ch.getDescriptors(UUID(0x2902))) # 0x2902 是Bluetooth SIG定義標準CCCD的handle
    cccd = ch.getDescriptors(UUID(0x2902))[0]
    cccd.write(b"\x01\x00") # \x02\x00代表啟動notify; \x02\x00代表啟動indicate; 不可使用\x0200取代，因為這樣透過ascii轉換後僅代表一個字元。
    
    while 1:
        # 如果有收到notify才會print 如果沒有收到進行下一次迴圈 每次wait 1sec
        if dev.waitForNotifications(2.0):
            print("Received notify:")  # 印出收到的通知数据
            if (ch.supportsRead()):
                print (ch.read())
            continue

finally:
    dev.disconnect()
