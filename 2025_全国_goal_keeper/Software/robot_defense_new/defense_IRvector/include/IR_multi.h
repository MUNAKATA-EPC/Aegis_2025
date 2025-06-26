#include <Arduino.h>

float readMux(int channel)
{
    uint8_t mul_address = channel;
    uint8_t bit;

    for (uint8_t j = 0; j < 4; j++)
    {
        bit = 1 << j;

        if (bit & mul_address)
        {
            digitalWrite(j + 1, HIGH);
        }
        else
        {
            digitalWrite(j + 1, LOW);
        }
    }

    delayMicroseconds(10);

    return analogRead(0);
}

