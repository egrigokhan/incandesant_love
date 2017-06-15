# Arduino Morse Code Decoder Using LDR

This is a simple Morse code decoder which uses Arduino in order to read light signals as dashes and dots which are then converted into characters from the English alphabet.

# Libraries

- Wire.h
- LiquidCrystal_I2C.h

While the Wire library comes pre-installed with the Wire library, the LiquidCrystal_I2C library needs to be manually installed from https://playground.arduino.cc/Code/LCD.

#Materials
<table>

  <thead>
   <tr>
    <th>Label</th>
    <th>Part Type</th>
    <th>Properties</th>
    </tr>
  </thead>
  <tbody>
  <tr>
    <td>LCM1602 IIC3</td>
    <td>LCM1602 IIC</td>
    <td class="props">package DIP (Dual Inline) [THT]; editable pin labels false; chip label LCM1602 IIC; variant variant 2; pins 22; part # LCM1602 IIC</td>
</tr><tr>
    <td>LED1</td>
    <td>Red (633nm) LED</td>
    <td class="props">package 1206 [SMD]; color Red (633nm)</td>
</tr><tr>
    <td>Part1</td>
    <td>Arduino Uno (Rev3)</td>
    <td class="props">type Arduino UNO (Rev3)</td>
</tr><tr>
    <td>R1</td>
    <td>Photocell (LDR)</td>
    <td class="props">package THT; resistance@ luminance 16 kOhms@ 10 lux; resistance@ dark 300 kOhms@ 10 seconds</td>
</tr><tr>
    <td>R2</td>
    <td>100kΩ Resistor</td>
    <td class="props">package 2010 [SMD]; tolerance ±5%; resistance 100kΩ</td>
</tr><tr>
    <td>U1</td>
    <td>LCD-16X2</td>
    <td class="props">package lcd-16x2; characters 16x2; variant silk</td>
</tr>
  </tbody>
</table>
