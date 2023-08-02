#!/usr/bin/env bash

getBhapticsName() {
    target=$1
    # rest of params
    flags=${@:2}

    echo "::debug::Getting bHaptics name for $target"
    echo "::debug::Flags are $flags"

    if [[ $flags =~ BLUETOOTH_USE_NIMBLE=true ]]; then
        echo "::debug::Nimble is enabled, appending +nimble to the target"
        target="$target+nimble"
    fi

    if [[ $flags =~ BATTERY_ENABLED=true ]]; then
        echo "::debug::Battery is enabled, appending +battery to the target"
        target="$target+battery"
    fi

    if [[ $flags =~ SERIAL_PLOTTER=true ]]; then
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

getOpenGlovesName() {
    local target=$1
    # rest of params
    local flags=${@:2}

    echo "::debug::Getting OpenGloves name for $target"
    echo "::debug::Flags are $flags"

    if [[ $flags =~ OPENGLOVES_COMM_SERIAL ]]; then
        echo "::debug::Serial is enabled, appending +serial to the target"
        target="$target+serial"
    fi

    if [[ $flags =~ OPENGLOVES_COMM_BTSERIAL ]]; then
        echo "::debug::Bluetooth Serial is enabled, appending +bluetooth to the target"
        target="$target+bluetooth"
    fi

    if [[ $flags =~ CALIBRATION_CURL=OH::MinMaxCalibrator ]]; then
        echo "::debug::MinMaxCalibrator is enabled, appending +curl_minmax to the target"
        target="$target+curl_minmaxcalib"
    fi

    if [[ $flags =~ CALIBRATION_CURL=OH::CenterPointDeviationCalibrator ]]; then
        echo "::debug::CenterPointDeviationCalibrator is enabled, appending +curl_cpcalib to the target"
        target="$target+curl_cpcalib"
    fi

    if [[ $flags =~ CALIBRATION_CURL=OH::FixedCenterPointDeviationCalibrator ]]; then
        echo "::debug::FixedCenterPointDeviationCalibrator is enabled, appending +curl_fcpcalib to the target"
        target="$target+curl_fcpcalib"
    fi

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
    getBhapticsName $@
elif [[ $target =~ ^(opengloves|lucidgloves|indexer) ]]; then
    getOpenGlovesName $@
else
    echo "::error::Unknown target $target"
    exit 1
fi
