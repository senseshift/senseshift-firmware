Import("env")

env.AddCustomTarget(
    name="release-archive",
    dependencies="$BUILD_DIR/${PROGNAME}.elf",
    actions=[
        "$PYTHONEXE ./scripts/targets/create-release-archive.py $BUILD_DIR/${PROGNAME}.zip $FLASH_EXTRA_IMAGES $ESP32_APP_OFFSET $SOURCE",
    ]
)
