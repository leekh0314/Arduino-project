//http://rtrobot.org
#include <CCS811.h>
// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (A5 on UNO)
// Connect SDA to I2C data pin (A4 on UNO)
// Connect WAKE to ground

CCS811 ccs = CCS811();

void setup() {
  Serial.begin(9600);
  if (!ccs.begin(0x5A))
    Serial.println("Not found the CCS811");
  while(!ccs.DataReady());
  double temp=ccs.Temperature();
  ccs.tempoffset=temp - 25.0;
}

void loop()
{
  if(ccs.DataReady())
  {
    uint16_t eCO2,TVOC;
    ccs.ReadResult(&eCO2, &TVOC);
    Serial.print("CO2:");
    Serial.print(eCO2);
    Serial.print("ppm\r\nTVOC:");
    Serial.print(TVOC);
    Serial.print("ppb\r\nTemp:");
    double temp=ccs.Temperature();
    Serial.println(temp);
  }
  delay(500);
}
