# SPT_MK1_Better
- **Goal**: get MK1 running again as quick as possible with bare minimum requirements

## How to setup MK1

# **For Test Stand still using the TEENSY**

### Test Stand
- If anything goes wrong, I can't help you. all the software in the repo is for the ESP32 now, you'd have to go back to an older commit
- Plug in the power supply
- Connect the DB9 cable to the test stand
- Connect the ESTOP wires to the test stand
- Connect the power supply to the test stand
- Go connect the control Box to the computer
- If you have to connect to the teensy make sure that 24V **IS NOT** connect to the 5V buck converter, This can fry the board.
- Then connect the usb cable to the teensy for debugging

### Control Box
**NOTE: data accuracy over Serial for the teensy rev is spotty, I don't think its accurate. It's fixed with the esp rev. But the controls with all still work**
- connect the DB9 cable
- connect ESTOP cable to VCC and GND
- connect control box esp32 to computer
- go to `data_collection/`
- open `mk1_better_v1_monitor.ctl` in CoolTerm
- configure COM port of esp32 in CoolTerm
- configure baud rate to 115200
- click connect in CoolTerm
- on top toolbar, click `View Chart` under the `View` dropdown
- under the `Connection` -> `Capture to Text/Binary File` -> `Start Capture`
- name the file <month.day.year>_<typeOfRun>_run<Number>.txt (ex. 11.19.23_hotfire_run1.txt)
- pause the recording `Capture to Text/Binary File` -> `Pause Capture` or Ctrl+Alt+R **at the beginning** so the file doesn't get too big 
- right before you start the run on the test stand, click `Capture to Text/Binary File` -> `Start Capture` or Ctrl+R to start recording
- after the run, click `Capture to Text/Binary File` -> `Stop Capture` or Ctrl+Shift+R to stop recording
- run `format_to_csv.py` format the file to csv and clean up the time
- open the csv file in excel and save as .xlsx if you're boring
- if youre a CHAD open `mk1_better_v1.m` in matlab
- change the file name to the file you just recorded
- run the script
- Oooooooh pretty graphs



### This is for the new esp32 boards [**DO NOT USE**]

# Test Stand
- Plug in the power supply
- Connect the power supply to the test stand
- Connect the ESTOP wires to the test stand
- Go connect the control Box to the computer

# Control Box
- if anything goes wrong, deal with it
- download and install CoolTerm https://freeware.the-meiers.org/
- if you are on windows, you will need to install the driver for the CP2102 chip https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads
- Go to `data_collection/`
- connect control box esp32 to computer
- open mk1_better_v2_monitor.ctl in CoolTerm
- configure COM port of esp32 in CoolTerm
- configure baud rate to 230400 
- click connect in CoolTerm
- on top toolbar, click `View Chart` under the `View` dropdown
- under the `connection` dropdown, click `Capture to Text/Binary File` -> `Start Capture`
- name the file <month.day.year>_<typeOfRun>_run<Number>.txt
- pause the recording `Capture to Text/Binary File` -> `Pause Capture` or Ctrl+Alt+R so the file doesn't get too big 
- right before you start the run, click `Capture to Text/Binary File` -> `Start Capture` or Ctrl+R to start recording
- after the run, click `Capture to Text/Binary File` -> `Stop Capture` or Ctrl+Shift+R to stop recording
- run `format_to_csv.py` format the file to csv and clean up the time
- open the csv file in excel and save as .xlsx if you're boring
- if youre a CHAD open `mk1_better_v2.m` in matlab
- change the file name to the file you just recorded
- run the script
- Oooooooh pretty graphs

## How project is organized
- `master_test_stand` code for test stand 
- `slave_control_box` code for  control box
- `data_collection/` is for Serial monitoring
- `ltspice` test circuits in LTSPICE



