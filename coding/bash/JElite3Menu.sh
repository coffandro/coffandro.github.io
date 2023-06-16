#!/bin/bash
# Bash Menu Script Example

MACAdd=2C:E0:32:A5:D4:26

PS3='Please choose an action for your Jabra Elite 3: '
options=("Connect" "Disconnect" "Info" "Quit")
select opt in "${options[@]}"
do
    case $opt in
        "Connect")
            bluetoothctl connect $MACAdd
            ;;
        "Disconnect")
            bluetoothctl disconnect $MACAdd
            ;;
        "Info")
            bluetoothctl info $MACAdd
            ;;
        "Quit")
            break
            ;;
        *) echo "invalid option $REPLY";;
    esac
done

