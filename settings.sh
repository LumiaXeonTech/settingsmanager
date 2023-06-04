#!/bin/bash

# Function to change desktop background
change_desktop_background() {
    local wallpaper_path="$1"
    gsettings set org.cinnamon.desktop.background picture-uri "file://$wallpaper_path"
    echo "Desktop background changed to: $wallpaper_path"
}

# Function to change screensaver settings
change_screensaver_settings() {
    local lock_enabled="$1"
    local idle_activation_enabled="$2"
    gsettings set org.cinnamon.desktop.screensaver lock-enabled "$lock_enabled"
    gsettings set org.cinnamon.desktop.screensaver idle-activation-enabled "$idle_activation_enabled"
    echo "Screensaver settings updated"
}

# Function to change power settings
change_power_settings() {
    local sleep_timeout="$1"
    local sleep_type="$2"
    gsettings set org.cinnamon.settings-daemon.plugins.power sleep-inactive-ac-timeout "$sleep_timeout"
    gsettings set org.cinnamon.settings-daemon.plugins.power sleep-inactive-ac-type "$sleep_type"
    echo "Power settings updated"
}

# Function to change sound settings
change_sound_settings() {
    local sound_device="$1"
    local sound_level="$2"
    if [[ "$sound_device" == "hdmi" ]]; then
        pactl set-sink-volume @DEFAULT_SINK@ "$sound_level%"
        echo "Sound output set to HDMI. Volume set to $sound_level%"
    else
        pacmd set-sink-volume 0 "$sound_level%"
        echo "Sound volume set to $sound_level%"
    fi
}

# Function to list available sound cards
list_available_sound_cards() {
    echo "Available sound cards:"
    pacmd list-sinks | awk '/index:/{print "Sound card "$2}'
}

# Function to change computer name
change_computer_name() {
    local computer_name="$1"
    sudo hostnamectl set-hostname "$computer_name"
    echo "Computer name set to: $computer_name"
}

# Function to change user name
change_user_name() {
    local user_name="$1"
    sudo usermod -l "$user_name" "$USER"
    echo "User name set to: $user_name"
}

# Function to change user password
change_user_password() {
    local user_name="$1"
    local password="$2"
    echo "$user_name:$password" | sudo chpasswd
    echo "Password updated for user: $user_name"
}

# Function to list available Wi-Fi cards
list_available_wifi_cards() {
    echo "Available Wi-Fi cards:"
    iwconfig 2>/dev/null | awk '/^[a-zA-Z]/{print $1}'
}

# Function to list available Wi-Fi networks for a card
list_available_wifi_networks() {
    local wifi_card="$1"
    echo "Available Wi-Fi networks for $wifi_card:"
    sudo iwlist "$wifi_card" scan | awk '/ESSID/{print "SSID: "$1}'
}

# Function to connect to a Wi-Fi network
connect_to_wifi_network() {
    local ssid="$1"
    local password="$2"
    local wifi_card="$3"
    if [ -z "$password" ]; then
        sudo nmcli device wifi connect "$ssid" ifname "$wifi_card"
    else
        sudo nmcli device wifi connect "$ssid" password "$password" ifname "$wifi_card"
    fi
    echo "Connecting to Wi-Fi network: $ssid"
}

# Function to list available Bluetooth devices
list_available_bluetooth_devices() {
    echo "Available Bluetooth devices:"
    bluetoothctl devices | awk '{print $2}'
}

# Function to connect to a Bluetooth device
connect_to_bluetooth_device() {
    local device_address="$1"
    bluetoothctl connect "$device_address"
    echo "Connecting to Bluetooth device: $device_address"
}

# Function to list available disks and their storage information
list_available_disks() {
    echo "Available disks:"
    df -h | awk '/^\/dev/{print "Disk: "$1", Free: "$4", Available: "$3}'
}

# Function to enable or disable the touchscreen
toggle_touchscreen() {
    local touchscreen_state="$1"
    if [[ "$touchscreen_state" == "enable" ]]; then
        xinput --enable "$(xinput | awk '/Touchscreen/{print $6}' | sed 's/id=//')"
        echo "Touchscreen enabled"
    elif [[ "$touchscreen_state" == "disable" ]]; then
        xinput --disable "$(xinput | awk '/Touchscreen/{print $6}' | sed 's/id=//')"
        echo "Touchscreen disabled"
    fi
}

# Function to display system specifications
display_system_specs() {
    echo "System Specifications:"
    uname -a
}

# Function to display help page
display_help() {
    echo "Usage: ./settings.sh [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  wallpaper <file_path>            Change desktop background to the specified image file"
    echo "  screensaver <lock> <idle>        Change screensaver settings: <lock> (true/false) <idle> (true/false)"
    echo "  power <timeout> <type>           Change power settings: <timeout> (in seconds) <type> (suspend/hybrid-sleep)"
    echo "  sound <device> <level>           Change sound settings: <device> (hdmi/default) <level> (0-100)"
    echo "  window <focus> <auto_raise>      Change window settings: <focus> (click/mouse) <auto_raise> (true/false)"
    echo "  list displays                    List available displays"
    echo "  brightness <level>               Set the brightness level (0-100)"
    echo "  sound <level>                    Set the sound volume level (0-100)"
    echo "  sound hdmi                       Set sound output to HDMI"
    echo "  list sound                       List available sound cards"
    echo "  name machine <name>              Set the computer name"
    echo "  name user <name>                 Set the main user's name"
    echo "  passwd <user> <password>         Set the password for a user"
    echo "  list wifi                        List available Wi-Fi cards"
    echo "  wifi list <card>                 List available Wi-Fi networks for a card"
    echo "  wifi connect <ssid> <password> <card>    Connect to a Wi-Fi network"
    echo "  list bluetooth                   List available Bluetooth devices"
    echo "  bluetooth connect <address>      Connect to a Bluetooth device"
    echo "  list disks                       List available disks"
    echo "  touch enable/disable             Enable or disable the touchscreen"
    echo "  specs                            Display system specifications"
    echo "  -h, --help                       Display this help page"
}

# Start the script

# Check if arguments are provided
if [ $# -eq 0 ]; then
    display_help
    exit 1
fi

# Parse the command-line arguments
case $1 in
    wallpaper) shift; change_desktop_background "$@" ;;
    screensaver) shift; change_screensaver_settings "$@" ;;
    power) shift; change_power_settings "$@" ;;
    sound) shift; change_sound_settings "$@" ;;
    window) shift; change_window_settings "$@" ;;
    list)
        case $2 in
            displays) echo "HDMI0, HDMI1" ;;
            sound) list_available_sound_cards ;;
            wifi) list_available_wifi_cards ;;
            bluetooth) list_available_bluetooth_devices ;;
            disks) list_available_disks ;;
            *) echo "Invalid option. Available list options: displays, sound, wifi, bluetooth, disks" ;;
        esac
        ;;
    brightness) shift; echo "Brightness set to: $1" ;;
    name)
        case $2 in
            machine) shift 2; change_computer_name "$@" ;;
            user) shift 2; change_user_name "$@" ;;
            *) echo "Invalid option. Available name options: machine, user" ;;
        esac
        ;;
    passwd) shift; change_user_password "$@" ;;
    wifi)
        case $2 in
            list) shift; list_available_wifi_networks "$@" ;;
            connect) shift; connect_to_wifi_network "$@" ;;
            *) echo "Invalid option. Available wifi options: list, connect" ;;
        esac
        ;;
    bluetooth)
        case $2 in
            list) list_available_bluetooth_devices ;;
            connect) shift; connect_to_bluetooth_device "$@" ;;
            *) echo "Invalid option. Available bluetooth options: list, connect" ;;
        esac
        ;;
    touch) shift; toggle_touchscreen "$@" ;;
    specs) display_system_specs ;;
    -h|--help) display_help ;;
    *) echo "Invalid option. Use -h or --help to see the available options." ;;
esac
