#!/bin/bash
# Bash Menu Script Example

MACAdd=2C:E0:32:A5:D4:26

PS3='Please enter your choice: '
options=("Info" "Connect" "Disconnect" "Quit")
select opt in "${options[@]}"
do
    case $opt in
        "Info")
            bluetoothctl info $MACAdd
            ;;
        "Connect")
            bluetoothctl connect $MACAdd
            ;;
        "Disconnect")
            bluetoothctl disconnect $MACAdd
            ;;
        "Quit")
            break
            ;;
        *) echo "invalid option $REPLY";;
    esac
done

