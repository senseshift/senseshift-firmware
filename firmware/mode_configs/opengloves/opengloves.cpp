#include <senseshift/freertos/task.hpp>
#include <senseshift/opengloves/autoconfig.hpp>
#include <senseshift/opengloves/opengloves_component.hpp>

using namespace SenseShift::OpenGloves;

void setupMode()
{
    auto* communication = AutoConfig::createTransport();
    auto* encoding = new og::AlphaEncoder();

    auto input_sensors = AutoConfig::createInput();

    OpenGlovesTrackingComponent::Config tracking_config(CALIBRATION_DURATION, CALIBRATION_ALWAYS_CALIBRATE);
    auto* og_tracking = new OpenGlovesTrackingComponent(tracking_config, input_sensors, communication, encoding);

    auto* og_tracking_task = new SenseShift::FreeRTOS::ComponentUpdateTask<OpenGlovesTrackingComponent>(
      og_tracking,
      1000 / UPDATE_RATE,
      {
        .name = "OG_TRACKING",
        .stackDepth = 8192,
        .priority = 1,
      }
    );
    og_tracking_task->begin();

#if FFB_ENABLED
    auto output_writers = AutoConfig::createFfbOutputs();

    auto* og_ffb = new OpenGlovesForceFeedbackComponent(output_writers, communication, encoding);

    auto* og_ffb_task = new ::SenseShift::FreeRTOS::ComponentUpdateTask<OpenGlovesForceFeedbackComponent>(
      og_ffb,
      1000 / UPDATE_RATE,
      {
        .name = "OG_FFB",
        .stackDepth = 8192,
        .priority = 1,
      }
    );
    og_ffb_task->begin();
#endif
}

void loopMode()
{
    vTaskDelete(NULL);
}
