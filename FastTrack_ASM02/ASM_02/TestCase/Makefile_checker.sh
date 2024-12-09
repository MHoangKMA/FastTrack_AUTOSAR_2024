#!/bin/bash
clear
# @echo off

# -----------------------------------------------------------------------------
# Title: Check build makefile example, using bash
# Usage:
#   Enviroment Cygwin: install make package
#   update PROJECT_PATH
#   update GCC_DIR
#   ./makefile_checker.sh file
# -----------------------------------------------------------------------------


# =============================================================================
# Define Macro
# =============================================================================

export PROJECT_PATH=$(pwd)/../Project_Demo

# export GCC_DIR on your machine. (for test local)
# export GCC_DIR=E:/NXP/NXP_ProgramFiles/S32DS/S32DS.3.5/S32DS/build_tools/gcc_v9.2/gcc-9.2-arm32-eabi/bin

OUTPUT_PATH="$PROJECT_PATH/output"
OJECT_PATH="$OUTPUT_PATH/object"
DEPEND_PATH="$OUTPUT_PATH/depend"

README_PATH="$PROJECT_PATH/README.md"

# Declare the array for object file to verify
VERIFY_LIST=("Can" "Clock" "GPIO" "Init" "interrupt_manager" "LED" "LPIT" "main" "PMC" "RTC" "Sch" "startup" "startup_S32K144" "system_S32K144" "Task" "WDog")

MAP_FILE_PATH="$OUTPUT_PATH/S32K144_Demo.map"
MAP_FILE_SIZE_MIN=70 # Kb

ELF_FILE_PATH="$OUTPUT_PATH/S32K144_Demo.elf"
ELF_FILE_SIZE_MIN=600 # Kb


SEPARATION="======================================================"
TAG="
==== [TAG]:"
INFO="      [INFO]:"
ERROR="      [ERROR]:"

# Declare the array for check error point
ERROR_POINT=()

# Calcuate point
POINT=0.0

STATUS_BOOL=true


# =============================================================================
# FUNCTION GLOBAL
# =============================================================================

Float_Calculate() { awk "BEGIN{print $*}"; }


Check_Readme () {
    echo $SEPARATION
    echo "$TAG Check README.md exist"

    if [ -f "$README_PATH" ]; then
        echo "$INFO README.md already exists. => OK!"
    else
        echo "$ERROR Path $README_PATH cannot found. => NOT OK!"
        ERROR_POINT+=("README.md cannot found")
    fi
}


Build_On () {
    echo $SEPARATION
    echo "$TAG Build on $1"

    make build LOAD_TO=$1 | tee ./build_log_$1.txt
    log_build=`cat build_log_$1.txt`

    echo "$TAG Check build on $1"
    if [[ "${log_build,,}" =~ "warning" || "${log_build,,}" =~ "error" ]] ; then
        echo "$ERROR Build $1: There is a warning or error => NOT OK"
        echo $log_build
        ERROR_POINT+=("$ERROR Build $1: There is a warning or error")
    else
        echo "$INFO Build $1: No warning or error => OK"
        POINT=$(Float_Calculate $POINT + 2)
    fi
}


Check_Build_2nd () {
    echo $SEPARATION
    echo "$TAG Check build on $1 2nd"

    # Build again
    make build LOAD_TO=$1 | tee ./build_log_$1.txt

    # Get first line of log file
    log_build="$(head -1 build_log_$1.txt)"

    if [[ "$log_build" = "==> BUILD on $1 Complete!" ]] ; then
        echo "$INFO Build $1: The program does not rebuild => OK"
        POINT=$(Float_Calculate $POINT + 2)
    else
        log="$ERROR Build $1: The program has been rebuilt => NOT OK"
        echo $log
        ERROR_POINT+=($log)
    fi
}


Check_Build_Modify () {
    echo $SEPARATION
    echo "$TAG Check build $1 affter modify"

    echo "$INFO Add new line character at the end of main.c file"
    echo ""  >> ./src/main.c

    echo "$INFO Add new line character at the end of S32K144.h file"
    echo ""  >> ./include/S32K144.h

    # Build again
    make build LOAD_TO=$1 | tee ./build_log_$1.txt
    log_build=`cat build_log_$1.txt`

    if [[ "$log_build" =~ "COMPILING Completed! : ./src/main.c" || "$log_build" =~ "==> COMPILING Completed! : ./Project_Settings/Startup_Code/system_S32K144.c" ]] ; then
        echo "$INFO Build $1: Rebuild OK"
        POINT=$(Float_Calculate $POINT + 2)
    else
        log="$ERROR Build $1: Rebuild NOT OK"
        echo $log
        ERROR_POINT+=($log)
    fi

    echo "$INFO Revert main.c file"
    truncate -s -1 ./src/main.c

    echo "$INFO Revert S32K144.h file"
    truncate -s -1 ./include/S32K144.h
}



Check_Object_File () {
    echo $SEPARATION

    echo "$TAG Build $1: CHECK OBJECT PATH EXIST"
    if [ -d "$OJECT_PATH" ]; then
        echo "$INFO Path ./output/object already exists. OK"
        POINT=$(Float_Calculate $POINT + 0.2)

        STATUS_BOOL=true
        echo "$TAG Check all object file exist in ./output/object/*"
        for obj_file_name in `ls $OJECT_PATH`; do
            if [[ " ${VERIFY_LIST[@]} " =~ " ${obj_file_name%.o} " ]]; then
                echo "$INFO File $obj_file_name already exists => OK"
            else
                echo "$ERROR Missing file $obj_file_name => NOT OK"
                ERROR_POINT+=("Build $1: Missing file $obj_file_name")
                STATUS_BOOL=false
            fi
        done

        if [ "$STATUS_BOOL" = true ] ; then
            POINT=$(Float_Calculate $POINT + 0.3)
        fi
    else
        echo "$ERROR Path $OJECT_PATH cannot found.! => NOT OK"
        ERROR_POINT+=("Build $1: $OJECT_PATH cannot found")
    fi
}


Check_Depend_File () {
    echo $SEPARATION

    echo "$TAG Build $1: CHECK DEPEND PATH EXIST"
    if [ -d "$DEPEND_PATH" ]; then
        echo "$INFO Path ./output/depend already exists => OK!"
        POINT=$(Float_Calculate $POINT + 0.2)

        STATUS_BOOL=true
        echo "$TAG Check all depend file exist in ./output/depend/*"
        for depend_file_name in `ls $DEPEND_PATH`; do
            if [[ " ${VERIFY_LIST[@]} " =~ " ${depend_file_name%.d} " ]]; then
                echo "$INFO File $depend_file_name already exists => OK"
            else
                echo "$ERROR Missing file $depend_file_name => NOT OK"
                ERROR_POINT+=("Build $1: Missing file $depend_file_name")
                STATUS_BOOL=false
            fi
        done

        if [ "$STATUS_BOOL" = true ] ; then
            POINT=$(Float_Calculate $POINT + 0.3)
        fi
    else
        echo "$ERROR Path $DEPEND_PATH cannot found.!"
        ERROR_POINT+=("Build $1: $DEPEND_PATH cannot found")
    fi
}


Check_Map_File () {
    echo $SEPARATION

    echo "$TAG Build $1: CHECK MAP FILE EXIST"
    if [ -e "$MAP_FILE_PATH" ]; then
        echo "$INFO Path MAP file already exists => OK!"
        POINT=$(Float_Calculate $POINT + 0.2)

        echo "$TAG Build $1: Check MAP file capacity (KiloBytes)"
        map_file_size_current=`expr $(wc -c $MAP_FILE_PATH | awk '{print $1}') / 1024`
        echo "$INFO MAP file size current: $map_file_size_current Kb"

        if [ $map_file_size_current -le $MAP_FILE_SIZE_MIN ]; then
            echo "$ERROR MAP file lack of capacity.! => NOT OK"
            ERROR_POINT+=("Build $1: MAP file lack of capacity. MAP file size must be greater than $MAP_FILE_SIZE_MIN")
        else
            echo "$INFO Size of MAP file is OK.!"
            POINT=$(Float_Calculate $POINT + 0.4)
        fi

        STATUS_BOOL=true
        echo "$TAG Build $1: Check MAP file have all object file"
        declare file_content=`cat $MAP_FILE_PATH`
        for obj_name in "${VERIFY_LIST[@]}"; do
            if [[ " $file_content " =~ $obj_name.o ]]; then
                echo "$INFO Object file found $obj_name.o => OK"
            else
                echo "$ERROR Object file not found $obj_name.o => NOT OK"
                ERROR_POINT+=("Build $1: Object file not found $obj_name.o")
                STATUS_BOOL=false
            fi
        done

        if [ "$STATUS_BOOL" = true ] ; then
            POINT=$(Float_Calculate $POINT + 0.4)
        fi
    else
        echo "$ERROR Path $MAP_FILE_PATH cannot found.! => NOT OK"
        ERROR_POINT+=("Build $1: Path $MAP_FILE_PATH cannot found")
    fi
}


Check_Elf_File () {
    echo $SEPARATION

    echo "$TAG Build $1: CHECK ELF FILE EXIST"
    if [ -e "$ELF_FILE_PATH" ]; then
        echo "$INFO Path ELF file already exists. OK!"
        POINT=$(Float_Calculate $POINT + 0.5)

        echo "$TAG Build $1: Check ELF file capacity (KiloBytes)"
        elf_file_size_current=`expr $(wc -c $ELF_FILE_PATH | awk '{print $1}') / 1024`
        echo "$INFO ELF file size current: $elf_file_size_current Kb"

        if [ $elf_file_size_current -le $ELF_FILE_SIZE_MIN ]; then
            echo "$ERROR ELF file lack of capacity.! => NOT OK"
            ERROR_POINT+=("Build $1: ELF file lack of capacity. ELF file size must be greater than $ELF_FILE_SIZE_MIN")
        else
            echo "$INFO Size of ELF file is OK.!"
            POINT=$(Float_Calculate $POINT + 0.5)
        fi
    else
        echo "$ERROR Path $ELF_FILE_PATH cannot found.! => NOT OK"
        ERROR_POINT+=("Build $1: Path $ELF_FILE_PATH cannot found")
    fi
}


Check_Clean () {
    echo $SEPARATION
    echo "$TAG Build $1: MAKE CLEAN"

    # Check Output Folder Exsit
    if [ -d "$OUTPUT_PATH" ]; then
        # Call make clean
        make clean

        if [ ! -d "$OUTPUT_PATH" ]; then
            echo "$INFO Done. Output Folder was removed => OK"
            POINT=$(Float_Calculate $POINT + 0.5)
        else
            if [ "$(ls -A $OUTPUT_PATH)" ]; then
                echo "$ERROR After call make clean, the output directory is not empty => NOT OK"
            fi
        fi
    else
        echo "$INFO Output Folder does Not Exsit Before"
    fi
}


Score () {
    echo "$TAG Total score for the exercise is: $POINT"

    if awk "BEGIN {exit !($POINT < 10)}"; then
        Check_Error_Point
    fi
}


Check_Error_Point () {
    echo $SEPARATION

    echo "$TAG Check ERROR point"
    echo "$INFO Total number Error: ${#ERROR_POINT[@]}"

    for value in "${ERROR_POINT[@]}"
    do
        echo $value
    done
}


# -----------------------------------------------------------------------------
# Main Function
cd ../Project_Demo
# Check_Readme


# Check Build Project on Flash
Build_On "FLASH"
Check_Object_File "FLASH"
Check_Map_File "FLASH"
Check_Elf_File "FLASH"
Check_Clean "FLASH"


# Check Build Project on Ram
Build_On "RAM"
Check_Object_File "RAM"
Check_Map_File "RAM"
Check_Elf_File "RAM"
Check_Clean "RAM"

# Check Point for makefile
Score

echo "$TAG END to check makefile"
echo $SEPARATION
echo $SEPARATION