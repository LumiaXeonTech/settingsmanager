#include <iostream>
#include <cstdlib>
#include <cstring>

void changeDesktopBackground(const char* wallpaperPath) {
    std::string command = "gsettings set org.cinnamon.desktop.background picture-uri \"file://" + std::string(wallpaperPath) + "\"";
    std::system(command.c_str());
    std::cout << "Desktop background changed to: " << wallpaperPath << std::endl;
}

void changeScreensaverSettings(const char* lockEnabled, const char* idleActivationEnabled) {
    std::string lockCommand = "gsettings set org.cinnamon.desktop.screensaver lock-enabled " + std::string(lockEnabled);
    std::string idleCommand = "gsettings set org.cinnamon.desktop.screensaver idle-activation-enabled " + std::string(idleActivationEnabled);
    std::system(lockCommand.c_str());
    std::system(idleCommand.c_str());
    std::cout << "Screensaver settings updated" << std::endl;
}

void changePowerSettings(const char* sleepTimeout, const char* sleepType) {
    std::string timeoutCommand = "gsettings set org.cinnamon.settings-daemon.plugins.power sleep-inactive-ac-timeout " + std::string(sleepTimeout);
    std::string typeCommand = "gsettings set org.cinnamon.settings-daemon.plugins.power sleep-inactive-ac-type " + std::string(sleepType);
    std::system(timeoutCommand.c_str());
    std::system(typeCommand.c_str());
    std::cout << "Power settings updated" << std::endl;
}

void changeSoundSettings(const char* soundDevice, const char* soundLevel) {
    if (std::strcmp(soundDevice, "hdmi") == 0) {
        std::string command = "pactl set-sink-volume @DEFAULT_SINK@ " + std::string(soundLevel) + "%";
        std::system(command.c_str());
        std::cout << "Sound output set to HDMI. Volume set to " << soundLevel << "%" << std::endl;
    } else {
        std::string command = "pacmd set-sink-volume 0 " + std::string(soundLevel) + "%";
        std::system(command.c_str());
        std::cout << "Sound volume set to " << soundLevel << "%" << std::endl;
    }
}

void listAvailableSoundCards() {
    std::cout << "Available sound cards:" << std::endl;
    std::system("pacmd list-sinks | awk '/index:/{print \"Sound card \" $2}'");
}

void changeComputerName(const char* computerName) {
    std::string command = "sudo hostnamectl set-hostname " + std::string(computerName);
    std::system(command.c_str());
    std::cout << "Computer name set to: " << computerName << std::endl;
}

void changeUserName(const char* userName) {
    std::string command = "sudo usermod -l " + std::string(userName) + " $USER";
    std::system(command.c_str());
    std::cout << "User name set to: " << userName << std::endl;
}

void changeUserPassword(const char* userName, const char* password) {
    std::string command = "echo " + std::string(userName) + ":" + std::string(password) + " | sudo chpasswd";
    std::system(command.c_str());
    std::cout << "Password updated for user: " << userName << std::endl;
}

void listAvailableWifiCards() {
    std::cout << "Available Wi-Fi cards:" << std::endl;
    std::system("iwconfig 2>/dev/null | awk '/^[a-zA-Z]/{print $1}'");
}

void listAvailableWifiNetworks(const char* wifiCard) {
    std::cout << "Available Wi-Fi networks for " << wifiCard << ":" << std::endl;
    std::string command = "sudo iwlist " + std::string(wifiCard) + " scan | awk '/ESSID/{print \"SSID: \" $1}'";
    std::system(command.c_str());
}

void connectToWifiNetwork(const char* ssid, const char* password, const char* wifiCard) {
    std::string command;
    if (std::strlen(password) == 0) {
        command = "sudo nmcli device wifi connect " + std::string(ssid) + " ifname " + std::string(wifiCard);
    } else {
        command = "sudo nmcli device wifi connect " + std::string(ssid) + " password " + std::string(password) + " ifname " + std::string(wifiCard);
    }
    std::system(command.c_str());
    std::cout << "Connecting to Wi-Fi network: " << ssid << std::endl;
}

void listAvailableBluetoothDevices() {
    std::cout << "Available Bluetooth devices:" << std::endl;
    std::system("bluetoothctl devices | awk '{print $2}'");
}

void connectToBluetoothDevice(const char* deviceAddress) {
    std::string command = "bluetoothctl connect " + std::string(deviceAddress);
    std::system(command.c_str());
    std::cout << "Connecting to Bluetooth device: " << deviceAddress << std::endl;
}

std::string executeCommand(const char* command) {
    std::string result = "";
    FILE* pipe = popen(command, "r");
    if (!pipe) {
        return result;
    }
    char buffer[128];
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}

void listAvailableDisplays() {
    std::string command = "xrandr -q | awk '/ connected/{print $1}'";
    std::string output = executeCommand(command.c_str());
    if (!output.empty()) {
        std::cout << "Available displays: " << output;
    } else {
        std::cout << "No displays found." << std::endl;
    }
}

void setBrightnessLevel(const char* level) {
    std::string command = "xrandr --output $(xrandr -q | awk '/ connected primary/{print $1}') --brightness " + std::string(level);
    executeCommand(command.c_str());
    std::cout << "Brightness set to: " << level << std::endl;
}

void listAvailableDisks() {
    std::cout << "Available disks:" << std::endl;
    std::system("df -h | awk '/^\\/dev/{print \"Disk: \" $1 \", Free: \" $4 \", Available: \" $3}'");
}

void toggleTouchscreen(const char* touchscreenState) {
    std::string idCommand = "xinput | awk '/Touchscreen/{print $6}' | sed 's/id=//'";
    std::string idOutput = executeCommand(idCommand.c_str());
    if (!idOutput.empty()) {
        std::string enableCommand = "xinput --enable " + idOutput;
        std::string disableCommand = "xinput --disable " + idOutput;
        if (std::string(touchscreenState) == "enable") {
            executeCommand(enableCommand.c_str());
            std::cout << "Touchscreen enabled" << std::endl;
        } else if (std::string(touchscreenState) == "disable") {
            executeCommand(disableCommand.c_str());
            std::cout << "Touchscreen disabled" << std::endl;
        } else {
            std::cout << "Error: Invalid touchscreen state." << std::endl;
        }
    } else {
        std::cout << "Error: Touchscreen not found." << std::endl;
    }
}

void displaySystemSpecs() {
    std::cout << "System Specifications:" << std::endl;
    std::system("uname -a");
}

void displayHelp() {
    std::cout << "Usage: ./settings [OPTIONS]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  wallpaper <file_path>            Change desktop background to the specified image file" << std::endl;
    std::cout << "  screensaver <lock> <idle>        Change screensaver settings: <lock> (true/false) <idle> (true/false)" << std::endl;
    std::cout << "  power <timeout> <type>           Change power settings: <timeout> (in seconds) <type> (suspend/hybrid-sleep)" << std::endl;
    std::cout << "  sound <device> <level>           Change sound settings: <device> (audio device) <level> (0-100)" << std::endl;
    std::cout << "  list displays                    List available displays" << std::endl;
    std::cout << "  brightness <level>               Set the brightness level (0-100)" << std::endl;
    std::cout << "  sound <level>                    Set the sound volume level (0-100)" << std::endl;
    std::cout << "  sound <audio_device>             Set sound output to the provided Audio Device" << std::endl;
    std::cout << "  list sound                       List available sound cards" << std::endl;
    std::cout << "  name machine <name>              Set the computer name" << std::endl;
    std::cout << "  name user <name>                 Set the main user's name" << std::endl;
    std::cout << "  passwd <user> <password>         Set the password for a user" << std::endl;
    std::cout << "  list wifi                        List available Wi-Fi cards" << std::endl;
    std::cout << "  wifi list <card>                 List available Wi-Fi networks for a card" << std::endl;
    std::cout << "  wifi connect <ssid> <password> <card>    Connect to a Wi-Fi network" << std::endl;
    std::cout << "  list bluetooth                   List available Bluetooth devices" << std::endl;
    std::cout << "  bluetooth connect <address>      Connect to a Bluetooth device" << std::endl;
    std::cout << "  list disks                       List available disks" << std::endl;
    std::cout << "  touch enable/disable             Enable or disable the touchscreen" << std::endl;
    std::cout << "  specs                            Display system specifications" << std::endl;
    std::cout << "  -h, --help                       Display this help page" << std::endl;
}

int main(int argc, char* argv[]) {
    // Check if arguments are provided
    if (argc == 1) {
        displayHelp();
        return 1;
    }

    // Parse the command-line arguments
    std::string option = argv[1];

    if (option == "wallpaper") {
        if (argc < 3) {
            std::cout << "Error: Missing file path for wallpaper option." << std::endl;
            return 1;
        }
        changeDesktopBackground(argv[2]);
    } else if (option == "screensaver") {
        if (argc < 4) {
            std::cout << "Error: Missing arguments for screensaver option." << std::endl;
            return 1;
        }
        changeScreensaverSettings(argv[2], argv[3]);
    } else if (option == "power") {
        if (argc < 4) {
            std::cout << "Error: Missing arguments for power option." << std::endl;
            return 1;
        }
        changePowerSettings(argv[2], argv[3]);
    } else if (option == "sound") {
        if (argc < 4) {
            std::cout << "Error: Missing arguments for sound option." << std::endl;
            return 1;
        }
        changeSoundSettings(argv[2], argv[3]);
    } else if (option == "list") {
        if (argc < 3) {
            std::cout << "Error: Missing list option argument." << std::endl;
            return 1;
        }
        std::string listOption = argv[2];
        if (listOption == "displays") {
            listAvailableDisplays();
        } else if (listOption == "sound") {
            listAvailableSoundCards();
        } else if (listOption == "wifi") {
            listAvailableWifiCards();
        } else if (listOption == "bluetooth") {
            listAvailableBluetoothDevices();
        } else if (listOption == "disks") {
            listAvailableDisks();
        } else {
            std::cout << "Error: Invalid list option." << std::endl;
            return 1;
        }
    } else if (option == "brightness") {
        if (argc < 3) {
            std::cout << "Error: Missing brightness level." << std::endl;
            return 1;
        }
        setBrightnessLevel(argv[2]);
    } else if (option == "name") {
        if (argc < 4) {
            std::cout << "Error: Missing arguments for name option." << std::endl;
            return 1;
        }
        std::string nameOption = argv[2];
        if (nameOption == "machine") {
            changeComputerName(argv[3]);
        } else if (nameOption == "user") {
            changeUserName(argv[3]);
        } else {
            std::cout << "Error: Invalid name option." << std::endl;
            return 1;
        }
    } else if (option == "passwd") {
        if (argc < 4) {
            std::cout << "Error: Missing arguments for passwd option." << std::endl;
            return 1;
        }
        changeUserPassword(argv[2], argv[3]);
    } else if (option == "wifi") {
        if (argc < 3) {
            std::cout << "Error: Missing wifi option argument." << std::endl;
            return 1;
        }
        std::string wifiOption = argv[2];
        if (wifiOption == "list") {
            if (argc < 4) {
                std::cout << "Error: Missing Wi-Fi card argument." << std::endl;
                return 1;
            }
            listAvailableWifiNetworks(argv[3]);
        } else if (wifiOption == "connect") {
            if (argc < 5) {
                std::cout << "Error: Missing arguments for Wi-Fi connect option." << std::endl;
                return 1;
            }
            connectToWifiNetwork(argv[3], argv[4], argv[5]);
        } else {
            std::cout << "Error: Invalid wifi option." << std::endl;
            return 1;
        }
    } else if (option == "bluetooth") {
        if (argc < 3) {
            std::cout << "Error: Missing bluetooth option argument." << std::endl;
            return 1;
        }
        std::string bluetoothOption = argv[2];
        if (bluetoothOption == "connect") {
            if (argc < 4) {
                std::cout << "Error: Missing Bluetooth device address." << std::endl;
                return 1;
            }
            connectToBluetoothDevice(argv[3]);
        } else {
            std::cout << "Error: Invalid bluetooth option." << std::endl;
            return 1;
        }
    } else if (option == "touch") {
        if (argc < 3) {
            std::cout << "Error: Missing touch option argument." << std::endl;
            return 1;
        }
        toggleTouchscreen(argv[2]);
    } else if (option == "specs") {
        displaySystemSpecs();
    } else if (option == "-h" || option == "--help") {
        displayHelp();
    } else {
        std::cout << "Error: Invalid option." << std::endl;
        return 1;
    }

    return 0;
}
