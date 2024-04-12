# Smart Parking

## Purpose

The Smart Parking project aims to expand functionality beyond simple parking. The parking lot will be equipped with features such as calculating the number and location of vehicles in the lot, interacting with and managing vehicle entry and exit via PC, displaying parking duration and fees. This is intended to enhance users' parking experience, making it easier for them to find available spots, saving time, and increasing convenience when using parking services.

## Feature

Smart Parking project includes the following feature:

1. **Entry/Exit using RFID cards:** Users press the button and scan the RFID card to allow the system to save the ID of the card and confirm vehicle entry or exit the parking lot through the entry or exit button.
2. **Display of time, parking fees, and parking slot status:** The OLED screen will display the entry/exit time of vehicles and use it to accurately calculate parking fees. Additionally, it will show the status of parking slot.
3. **Interaction for information management on PC:** The number of vehicles entering/exiting the parking lot, the available slots, and parking fees will be displayed on the PC.
4. **Fire protection system:** This system use to detect a fire in the parking lot.

## Required Modules

| Module       |            Functionality                   |
|--------------|--------------------------------------------|
| STM32F411VGE | Main microcontroller for the parking       |
| DS1307       | Real-time clock module for manage time     |
| SSD1306      | Display module for showing information to user                                                        |
| RFID-RC522   | RFID module for entry/exit                 |
| BUTTON       | User interaction module for choose entry or exit                                                        |
| CP1202 MINI  | UART module for communicate between main microcontroller and PC                                      |
| LED RGB      | Status lights for the parking slot         |
| RF CARD      | Card for save information                  |
| MQ135        | Smoke sensor for detecting fire            |
| Buzzer       | Audio module for fire alarm                |

## Detailed Feature

1. **Entry/Exit using RF cards**
   
| ID | Functionality        |            Note              |
|----|----------------------|------------------------------|
| 01 | Save car information | RFID card saves information about time entry, time exit from RTC module, total parking time, parking fees, status of the car in parking           |
| 02 | Button entry         | Before user swipes RF card, push button to choose entry the parking                    |
| 03 | Button exit          | Before user swipes RF card, push button to choose exit the parking                     |
| 04 | Buzzer               | After user swipes RF card, buzzer will sound to indicate the status of entry or exit  |

2. **Display of time, parking fees, and parking space status:**
   
| ID | Functionality           |          Note             |
|----|-------------------------|---------------------------|
| 01 | Showing car information | OLED screen display time entry and time exit the parking, total time parking, parking fees and status car to user when they swipes RF card       |

3. **Interaction for information management via PC:**

| ID | Functionality         |            Note             |
|----|-----------------------|-----------------------------|
| 01 | Manage information    | Management information about time entry or exit, parking fees and slots status via PC   |
| 02 | Change information    | The manager can change the parking fees via UART                                      |
4. **Fire alarm system:**
   
| ID | Functionality         |            Note             |
|----|-----------------------|-----------------------------|
| 01 | Detecting a fire      | The buzzer turned on when MQ135 sensor detects smoke in parking lot and send a warning via UART to PC                                             |
## Firmware Architecture

![Smart Parking Architecture](./smart_parking.svg)

## Developer Team
1. **Nguyen Thanh Minh**
2. **Le Minh**
3. **Thien Vuong**