import os
import sys
import re
import argparse


def github_debug(message):
    """
    Print a debug message in GitHub Actions format.

    Args:
        message (str): The debug message.
    """
    print(f"::debug::{message}")


def strip_flags(flags):
    return [flag[2:] if flag.startswith('-D') and flag != '-D' else flag for flag in flags if flag != '-D']


def store_firmware_name(target):
    print(f"firmware={target}")
    if os.getenv('GITHUB_ACTIONS'):
        with open(os.getenv('GITHUB_OUTPUT'), 'a') as github_output:
            github_output.write(f"firmware={target}\n")
    else:
        print("::error::Not in GitHub Actions")


def get_bhaptics_name(target, flags):
    """
    Get bHaptics name based on target and flags.

    Args:
        target (str): The target name.
        flags (list): List of flags.
    """
    github_debug(f"Getting bHaptics name for {target}")
    github_debug(f"Flags are {flags}")

    if "SS_USE_NIMBLE=true" in flags:
        github_debug("Nimble is enabled, appending +nimble to the target")
        target += "+nimble"

    if "SS_BATTERY_ENABLED=true" in flags:
        github_debug("Battery is enabled, appending +battery to the target")
        target += "+battery"

    store_firmware_name(target)


def handle_calibration_flag(target, flag, prefix):
    """
    Handle calibration flags.

    Args:
        target (str): The target name.
        flag (str): The flag name.
        prefix (str): The prefix for the calibration.
    """
    if "MinMaxCalibrator" in flag:
        target += f"+{prefix}_minmax"
    elif "FixedCenterPointDeviationCalibrator" in flag:
        target += f"+{prefix}_fcpd"
    elif "CenterPointDeviationCalibrator" in flag:
        target += f"+{prefix}_cpd"

    return target


def handle_opengloves_comm_flag(target, flag):
    """
    Handle OpenGloves communication flags.

    Args:
        target (str): The target name.
        flag (str): The flag name.
    """
    if "OPENGLOVES_COMM_SERIAL" in flag:
        github_debug("Serial is enabled, appending +serial to the target")
        target += "+serial"
    elif "OPENGLOVES_COMM_BTSERIAL" in flag:
        github_debug("Bluetooth Serial is enabled, appending +bluetooth to the target")
        target += "+bluetooth"
    elif "OPENGLOVES_COMM_BLESERIAL" in flag:
        github_debug("BLE Serial is enabled, appending +ble to the target")
        target += "+ble"

    return target


def get_opengloves_name(target, flags):
    """
    Get OpenGloves name based on target and flags.

    Args:
        target (str): The target name.
        flags (list): List of flags.
    """
    github_debug(f"Getting OpenGloves name for {target}")
    github_debug(f"Flags are {flags}")

    for flag in flags:
        if "OPENGLOVES_COMMUNICATION" in flag:
            target = handle_opengloves_comm_flag(target, flag)
        if "CALIBRATION_CURL" in flag:
            target = handle_calibration_flag(target, flag, "curl_calib")
        if "CALIBRATION_SPLAY" in flag:
            target = handle_calibration_flag(target, flag, "splay_calib")

    store_firmware_name(target)


def main():
    parser = argparse.ArgumentParser(description='Get firmware name based on target and flags.')
    parser.add_argument('target', type=str, help='The target name')
    parser.add_argument('flags', type=str, help='List of flags', default='')
    args = parser.parse_args()

    target = args.target
    flags = strip_flags(args.flags.split(' '))

    github_debug(f"Target is {target}")

    if re.match(r'^(bhaptics)', target):
        get_bhaptics_name(target, flags)
    elif re.match(r'^(opengloves|lucidgloves|indexer)', target):
        get_opengloves_name(target, flags)
    else:
        print(f"::error::Unknown target {target}")
        sys.exit(1)


if __name__ == "__main__":
    main()
