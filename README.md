# SPT_MK1_Better
- Goal: get MK1 running again as quick as possible with bare minimum requirements

## How to setup MK1
# Test Stand
- Plug in the power supply
- Connect the power supply to the test stand
- Connect the ESTOP wires to the test stand
- Go connect the test stand to the computer

# Control Box
- if anything goes wrong, deal with it
- Go to `data_collection/`
- download and install CoolTerm https://freeware.the-meiers.org/
- connect control box esp32 to computer
- if you are on windows, you will need to install the driver for the CP2102 chip https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads
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



