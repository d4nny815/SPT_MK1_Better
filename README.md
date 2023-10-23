# SPT_MK1_Better
- Goal: get MK1 running again as quick as possible with bare minimum requirements

## How project is organized
- `src/` contains all source code, firmware built just for teensy
- `lib/` contains all libraries, modules are made to be portable. Module should be plug and play
- `data/` is for Serial monitoring

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
- start recording to a txt file in data/
- stop recording when done 

