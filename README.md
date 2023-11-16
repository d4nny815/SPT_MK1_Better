# SPT_MK1_Better
- Goal: get MK1 running again as quick as possible with bare minimum requirements

## How project is organized
- `master_test_stand` 
- `slave_control_board`
- `data_collection/` is for Serial monitoring
- `ltspice` test circuits in LTSPICE
- `mk1_better_pcb` schematics for making a custom PCB

## How to build
- connect teensy to computer
- configure COM port
- click upload in bottom left corner of VSCode

## How to add new module
- create new folder in `lib/`
- create new file `lib/<module_name>/<module_name>.cpp`
- create new file `lib/<module_name>/<module_name>.h`
- header file should contain all function declarations
- source file should contain all function definitions

## How to view serial monitor
- download and install CoolTerm
- configure COM port
- configure baud rate to 115200 
- connect teensy to computer
- open MK1_monitor.stc in data/ folder
- click connect in CoolTerm
- start recording to a txt file in data_collection/
- stop recording when done 

