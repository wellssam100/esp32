# Creating a Project from Scratch

## Understanding the Component Folder from the ESP-IDF library

## Playing with I2C
It looks like the libraries for I2C using ESP-IDF are stored in `ESP-IDF\esp-idf-v5.2.3\components\driver\i2c\include\driver`. 
This is different from how the Arduino IDE manages ESP libraries, in fact it manages the compilation of ESP projects completely differently. [This](https://www.esp32.com/viewtopic.php?t=20447) forum post is pretty interesting and shows the differences between Arduino and what I am doing.<br>
The I2c Port is the physical connection ( the pins of the board) used for i2c communication. The I2C address references the specific device on the bus which we want to communicate.<br>
The master slave relationship dictates some of the syntax required for communication between a master and slave instance
### Master and Slave Devices
Master devices control everything, they actively seek out the peripheral devices and ping them. The slave devices (like sensors) wait to be addressed by master devices. 
Slave writes and reads to buffers that are picked up by the master devices. 

## Arduino to me
Arduino's libraries are all simplified compared to the ESP-IDF implementations. Arduino's target audience is educational while ESP-IDF's is production.
| Arduino dep | ESP-IDF | Domain |
| :-----------: | :-----------: | :-----------: |
|  Wire.h     |  driver/i2c.h      | i2c drivers/ communication|


## FreeRTOS
FreeRTOS works differently from other programs in that it's building blocks are tasks that use multiprocessing. Tasks share memory space, they are not processes

|Tasks|Timers|Queues|SEmaphores|Event Groups| Interrupts|
|Use for independent threads of execution| Use for communication between tasks| Use for synchronization or resource management| Use for periodic or delayed execution| Use for multi-even synchronization|


## i2c translation
An Arduino example of how to start communications looks something like this:
```
#include <Wire.h>
 
void commune() {
  Wire.begin(); //define SDA and SCL pins
  Wire.beginTransmission(address); // Start communication with I2C address 0x3C
  Wire.write(data) // Write a byte
  Wire.endTransmission(); // Stop communication
}
```

When we do this with ESP-IDF we have to add additional steps to configure and manage the communication. <br>The config struct docs can be found [here](https://docs.espressif.com/projects/esp-idf/en/v4.2.3/esp32/api-reference/peripherals/i2c.html#_CPPv412i2c_config_t). <br>This is a master config example:
```
void app_main(void){
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,                // Tells if the driver is a slave or 
        .sda_io_num = I2C_MASTER_SDA_IO,        // GPIO number for sda signal (normally 21)
        .sda_pullup_en = SPIO_PULLUP_ENABLE,    // Internal pull mode for sda
        .scl_io_num = I2C_MASTER_SCL_IO,        // GPIO number for scl signal (normally 22)
        .scl_pullup_en = GPIO_PULLUP_ENABLE,    // Internal pull mode for scl
        .master.clk_speed = I2C_MASTER_FREQ_HZ, //clock frequency for master mode ( no more that 1MHz)
    }
}
```

After the driver configuration is defined we can configure it,<br> `i2c_param_config` [docs](https://docs.espressif.com/projects/esp-idf/en/v4.2.3/esp32/api-reference/peripherals/i2c.html#_CPPv416i2c_param_config10i2c_port_tPK12i2c_config_t)<br> `i2c_driver_install` [docs](https://docs.espressif.com/projects/esp-idf/en/v4.2.3/esp32/api-reference/peripherals/i2c.html#_CPPv418i2c_driver_install10i2c_port_t10i2c_mode_t6size_t6size_ti)

```
i2c_param_config(I2C_NUM_0, &conf); 
i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
```
Here `I2C_NUM_O` is the port number we are communicating with. We can use the I2C lister to see what ports are available.
This covers the first `Wire.begin()` command for setup. 

Now we can tackle the `Wire.beginTransmission(address)` command
For ESP-IDF we use the `i2c_master_write_to_deivce` command ([doc](https://docs.espressif.com/projects/esp-idf/en/v4.2.3/esp32/api-reference/peripherals/i2c.html#_CPPv416i2c_master_write16i2c_cmd_handle_tP7uint8_t6size_tb)). This command is slightly different to `i2c_master_write` in that it manages the command link automatically. 

When does `i2c_driver_delete` need to be called? When all threads have stopped using the driver in any way ( thread safety not included) and I2C communication is not required. It releases allocated resources for I2C communication