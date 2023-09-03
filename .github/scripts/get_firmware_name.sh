#!/usr/bin/env bash

getBhapticsName() {
    target=$1
    # rest of params
    flags=${@:2}

    echo "::debug::Getting bHaptics name for $target"
    echo "::debug::Flags are $flags"

    if [[ $flags =~ SENSESHIFT_BLE_USE_NIMBLE=true ]]; then
        echo "::debug::Nimble is enabled, appending +nimble to the target"
        target="$target+nimble"
    fi

    if [[ $flags =~ SENSESHIFT_BATTERY_ENABLED=true ]]; then
        echo "::debug::Battery is enabled, appending +battery to the target"
        target="$target+battery"
    fi

    if [[ $flags =~ SENSESHIFT_SERIAL_PLOTTER=true ]]; then
        echo "::debug::Serial Plotter is enabled, appending +serialplotter to the target"
        target="$target+serialplotter"
    fi

    echo "firmware=$target"
    if [[ -n "$GITHUB_ACTIONS" ]]; then
        echo "firmware=$target" >> "$GITHUB_OUTPUT"
        return
    else
        echo "::error::Not in GitHub Actions"
    fi
}

handleCalibrationFlag() {
    local target=$1
    local flag=$2
    local prefix=$3

    if [[ $flag =~ MinMaxCalibrator ]]; then
        target="$target+${prefix}_minmax"
    elif [[ $flag =~ FixedCenterPointDeviationCalibrator ]]; then
        target="$target+${prefix}_fcpd"
    elif [[ $flag =~ CenterPointDeviationCalibrator ]]; then
        target="$target+${prefix}_cpd"
    fi

    echo "$target"
}

getOpenGlovesName() {
    local target=$1
    # rest of params
    local flags=${@:2}

    echo "::debug::Getting OpenGloves name for $target"
    echo "::debug::Flags are $flags"

    if [[ $flags =~ OPENGLOVES_COMM_SERIAL ]]; then
        echo "::debug::Serial is enabled, appending +serial to the target"
        target="$target+serial"
    elif [[ $flags =~ OPENGLOVES_COMM_BTSERIAL ]]; then
        echo "::debug::Bluetooth Serial is enabled, appending +bluetooth to the target"
        target="$target+bluetooth"
    elif [[ $flags =~ OPENGLOVES_COMM_BLESERIAL ]]; then
        echo "::debug::BLE Serial is enabled, appending +ble to the target"
        target="$target+ble"
    fi

    for flag in "${@:2}"; do
        if [[ $flag =~ CALIBRATION_CURL ]]; then
            target=$(handleCalibrationFlag "$target" "$flag" "curl_calib")
        fi

        if [[ $flag =~ CALIBRATION_SPLAY ]]; then
            target=$(handleCalibrationFlag "$target" "$flag" "splay_calib")
        fi
    done

    echo "firmware=$target"
    if [[ -n "$GITHUB_ACTIONS" ]]; then
        echo "firmware=$target" >> "$GITHUB_OUTPUT"
        return
    else
        echo "::error::Not in GitHub Actions"
    fi
}

target=$1
echo "::debug::Target is $target"
if [[ $target =~ ^(bhaptics) ]]; then
    getBhapticsName "${@}"
elif [[ $target =~ ^(opengloves|lucidgloves|indexer) ]]; then
    getOpenGlovesName "${@}"
else
    echo "::error::Unknown target $target"
    exit 1
fi
