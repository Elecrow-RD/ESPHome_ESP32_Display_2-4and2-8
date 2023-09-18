esphome:
  name: hmi-3-5
  friendly_name: HMI_3-5

esp32:
  board: denky32
  framework:
    type: arduino

# Enable logging
logger:

# Enable Home Assistant API
api:
  encryption:
    key: "UI2nUrGRQsc8dUzu589iMGCijuyJ9n2nz4YNymbNb24="

ota:
  password: "25738a4c101cfc2c3f972cffe3400413"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Hmi-3-5 Fallback Hotspot"
    password: "WTYUR3CrGlSN"

captive_portal:
font:
  - file: 'slkscr.ttf'
    id: font1
    size: 10

  - file: 'BebasNeue-Regular.ttf'
    id: font2
    size: 48

  - file: 'arial.ttf'
    id: font3
    size: 22

  - file: 'arial.ttf'
    id: font4
    size: 18

switch:
  - platform: gpio
    name: "led"
    id: led24
    pin: 25
    inverted: no
    #初始化为off
    restore_mode: ALWAYS_OFF
  - platform: gpio
    name: "bck"
    id: bck24
    pin: 27
    inverted: no
    #初始化为off
    restore_mode: ALWAYS_ON

i2c:
  sda: 22
  scl: 21

sensor:
  - platform: aht10 #dht12 #am2320 #
    temperature:
      name: "HMI-24 Temperature"
      id: temperature24
    humidity:
      name: "HMI-24 Humidity"
      id: humidity24
    update_interval: 10s

# sensor:
#   - platform: dht
#     pin: 25
#     temperature:
#       name: "HMI-24 Temperature"
#       id: temperature24
#     humidity:
#       id: humidity24
#       name: "HMI-24 Humidity"
#     update_interval: 10s
#     model: DHT11

spi:
  clk_pin: 14
  mosi_pin: 13
  miso_pin: 12


  
image:
#  - file: "ELE.png"
#    id: my_image
#    resize: 320x240
#    type: RGB24
  - file: "menu.png"
    id: menu
    resize: 320x240
    type: RGB24
  - file: "ON_Light.png"
    id: ON_Light
    resize: 320x240
    type: RGB24
  - file: "OFF_Light.png"
    id: OFF_Light
    resize: 320x240
    type: RGB24

color:
  - id: my_red
    red: 100%
    green: 0%
    blue: 0%
  - id: my_color
    red: 100%
    green: 100%
    blue: 0%
  - id: my_green
    red: 0%
    green: 100%
    blue: 0%
  - id: my_blue
    red: 0%
    green: 0%
    blue: 100%

touchscreen:
  platform: xpt2046
  id: my_touchscreen
  cs_pin: 33
  update_interval: 500ms
  report_interval: 500ms
  threshold: 400
  calibration_x_min: 3848
  calibration_x_max: 320
  calibration_y_min: 3848
  calibration_y_max: 240
  swap_x_y: false
  on_touch:
    - lambda: |-
          ESP_LOGI("cal", "x=%d, y=%d, x_raw=%d, y_raw=%0d",
              id(my_touchscreen).x,
              id(my_touchscreen).y,
              id(my_touchscreen).x_raw,
              id(my_touchscreen).y_raw
              );
          if((id(my_touchscreen).x > 160) && (id(my_touchscreen).y < 200)) {
            id(led24).toggle();
          }
              
display:
  - platform: ili9xxx
    model: ili9341
    dc_pin: 2
    cs_pin: 15
    rotation: 90
    lambda: |-     
      it.image(0, 0, id(menu));
      it.printf(221, 63, id(font3), id(my_red), "%.1f", id(temperature24).state);
      it.printf(221, 162, id(font3), id(my_blue), "%.1f", id(humidity24).state);
      if (id(led24).state) {
        it.image(0, 0, id(ON_Light));
      } 
      else {
        it.image(0, 0, id(OFF_Light));
      }

# display:
#   - platform: ili9xxx
#     model: ili9341
#     dc_pin: 2
#     cs_pin: 15
#     rotation: 90
#     lambda: |-     
#       it.fill(id(my_color));
#       it.fill(id(my_color));
#       it.image(0, 0, id(my_image));
#       it.print(0, 0, id(font3), id(my_red), TextAlign::TOP_LEFT, "WillPower:");    