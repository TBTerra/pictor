@echo off
CLS
SET PROG="usbasp"
SET PART="m644p"
ECHO
ECHO	AVR-TOOL v1
ECHO	A simple programming automater by Henry Wilson.
ECHO

:main
ECHO ==============================================================
ECHO 	AVR-TOOL menu; please select an option:
ECHO 		[H] Flash a hex file to AVR.
ECHO 		[C] Build, link and flash a C project to AVR.
ECHO 		[B] Force AVR out of Bootloader.
ECHO 		[T] View memory use in ELF files.
ECHO 		[X] Exit.
CHOICE /C XTBCH /N
IF errorlevel 5 GOTO hexselect
IF errorlevel 4 GOTO select
IF errorlevel 3 GOTO boot
IF errorlevel 2 GOTO selectELF
ECHO Done!
EXIT

:hexselect
SET /P HEX="Please specifiy hex file: "
IF EXIST "%HEX%.hex" GOTO hexflash
ECHO %HEX%.hex not found!
GOTO main

:hexflash
ECHO Flashing...
avrdude -c %PROG% -p %PART% -U flash:w:%HEX%.hex -q -q
IF NOT ERRORLEVEL 1 GOTO success
CHOICE /M "%HEX%.hex not flashed! Do you want to retry "
IF NOT ERRORLEVEL 2 GOTO hexflash
ECHO User cancelled.
CD ..
GOTO main

:boot
ECHO Booting...
avrdude -c %PROG% -p %PART% -q -q -n
GOTO main

:select
SET /P PROJECT="Please specify project file: "
IF EXIST "%PROJECT%.c" GOTO build
ECHO Project not found!
GOTO main

:build
ECHO Building...
avr-gcc -mmcu=atmega644p -DF_CPU=12000000 -O2 %PROJECT%.c -o %PROJECT%.elf
IF NOT ERRORLEVEL 1 GOTO link
ECHO Build failed!
GOTO main

:link
ECHO Linking...
avr-objcopy -O ihex %PROJECT%.elf %PROJECT%.hex
IF NOT ERRORLEVEL 1 GOTO flash
ECHO Linking process failed!
GOTO main

:flash
ECHO Flashing...
avrdude -c %PROG% -p %PART% -U flash:w:%PROJECT%.hex -q -q
IF NOT ERRORLEVEL 1 GOTO success
CHOICE /M "%PROJECT%.hex not flashed! Do you want to retry "
IF NOT ERRORLEVEL 2 GOTO flash
ECHO User cancelled.
GOTO main

:selectELF
SET /P PROJECT="Please specify ELF file: "
IF EXIST "%PROJECT%.elf" GOTO size
ECHO Project not found!
GOTO main

:size
avr-size -C --mcu=atmega644p %PROJECT%.elf
avr-size -B %PROJECT%.elf
GOTO main

:success
ECHO AVR successfully flashed!
GOTO main