/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* MBED_DEPRECATED */
#ifndef MBED_BLE_MAGNETO_SERVICE_H__
#define MBED_BLE_MAGNETO_SERVICE_H__

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/GattServer.h"
#include "cstdio"
#include <cstdint>
#include <stdint.h>
#if BLE_FEATURE_GATT_SERVER

/**
 * BLE Heart Rate Service.
 *
 * @par purpose
 *
 * Fitness applications use the heart rate service to expose the heart
 * beat per minute measured by a heart rate sensor.
 *
 * Clients can read the intended location of the sensor and the last heart rate
 * value measured. Additionally, clients can subscribe to server initiated
 * updates of the heart rate value measured by the sensor. The service delivers
 * these updates to the subscribed client in a notification packet.
 *
 * The subscription mechanism is useful to save power; it avoids unecessary data
 * traffic between the client and the server, which may be induced by polling the
 * value of the heart rate measurement characteristic.
 *
 * @par usage
 *
 * When this class is instantiated, it adds a heart rate service in the GattServer.
 * The service contains the location of the sensor and the initial value measured
 * by the sensor.
 *
 * Application code can invoke updateHeartRate() when a new heart rate measurement
 * is acquired; this function updates the value of the heart rate measurement
 * characteristic and notifies the new value to subscribed clients.
 *
 * @note You can find specification of the heart rate service here:
 * https://www.bluetooth.com/specifications/gatt
 *
 * @attention The service does not expose information related to the sensor
 * contact, the accumulated energy expanded or the interbeat intervals.
 *
 * @attention The heart rate profile limits the number of instantiations of the
 * heart rate services to one.
 */
class MagnetoService {
public:
    /**
     * Intended location of the heart rate sensor.
     */
    enum SensorAxis {
        /**
         * Other location.
         */
        MAGNETO_X = 0,

        /**
         * Chest.
         */
        MAGNETO_Y = 1,

        /**
         * Wrist.
         */
        MAGNETO_Z = 2,
    };

public:
    /**
     * Construct and initialize a heart rate service.
     *
     * The construction process adds a GATT heart rate service in @p _ble
     * GattServer, sets the value of the heart rate measurement characteristic
     * to @p hrmCounter and the value of the body sensor location characteristic
     * to @p location.
     *
     * @param[in] _ble BLE device that hosts the magneto service.
     * @param[in] value get from the magneto
     * @param[in] axis of the magneto
     */
    MagnetoService(BLE &_ble, int16_t magnetoCounter, SensorAxis axis, uint16_t UUID) :
        ble(_ble),
        valueBytes(magnetoCounter),
        magnetoValue(
            UUID+1,
            valueBytes.getPointer(),
            valueBytes.getNumValueBytes(),
            MagnetoValueBytes::MAX_VALUE_BYTES,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
        ),
        magnetoAxis(
            UUID+2,
            reinterpret_cast<uint8_t*>(&axis)
        )
    {
        setupService(UUID);
    }

    /**
     * Update the heart rate that the service exposes.
     *
     * The server sends a notification of the new value to clients that have
     * subscribed to updates of the heart rate measurement characteristic; clients
     * reading the heart rate measurement characteristic after the update obtain
     * the updated value.
     *
     * @param[in] hrmCounter Heart rate measured in BPM.
     *
     * @attention This function must be called in the execution context of the
     * BLE stack.
     */
    void updateMagnetoValue(int16_t magnetoCounter) {
        valueBytes.updateMagnetoValue(magnetoCounter);
        ble.gattServer().write(
            magnetoValue.getValueHandle(),
            valueBytes.getPointer(),
            valueBytes.getNumValueBytes()
        );
    }

protected:
    /**
     * Construct and add to the GattServer the heart rate service.
     */
    void setupService(uint16_t UUID) {
        GattCharacteristic *charTable[] = {
            &magnetoValue,
            &magnetoAxis
        };
        GattService magnetoService(
            UUID,
            charTable,
            sizeof(charTable) / sizeof(charTable[0])
        );

        ble.gattServer().addService(magnetoService);
    }

protected:
    struct MagnetoValueBytes {
        /* 1 byte for the Flags, and up to two bytes for heart rate value. */
        static const unsigned MAX_VALUE_BYTES = 3;

        MagnetoValueBytes(int16_t magnetoCounter) : valueBytes()
        {
            updateMagnetoValue(magnetoCounter);
        }

        void updateMagnetoValue(int16_t magnetoCounter)
        {
            // std::sprintf((char*)valueBytes,"%04X",(char)magnetoCounter);
            valueBytes[0]=(magnetoCounter&0xFF0000)>>16;
            valueBytes[1]=(magnetoCounter&0x00FF00)>>8;
            valueBytes[2]=(magnetoCounter&0x0000FF);
            printf("%d, %x, %x, %x \n",magnetoCounter,valueBytes[0],valueBytes[1],valueBytes[2]);
        }

        uint8_t *getPointer()
        {
            return valueBytes;
        }

        const uint8_t *getPointer() const
        {
            return valueBytes;
        }

        unsigned getNumValueBytes() const
        {
            return MAX_VALUE_BYTES;
        }

    private:
        uint8_t valueBytes[MAX_VALUE_BYTES];
    };

protected:
    BLE &ble;
    MagnetoValueBytes valueBytes;
    GattCharacteristic magnetoValue;
    ReadOnlyGattCharacteristic<uint8_t> magnetoAxis;
};

#endif // BLE_FEATURE_GATT_SERVER

#endif /* #ifndef MBED_BLE_HEART_RATE_SERVICE_H__*/
