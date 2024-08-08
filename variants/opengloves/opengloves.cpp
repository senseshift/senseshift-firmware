#include "senseshift/freertos/task.hpp"
#include "senseshift/opengloves/autoconfig.hpp"
#include "senseshift/opengloves/opengloves_component.hpp"

#include "opengloves/alpha.hpp"

using namespace SenseShift::OpenGloves;
namespace og = ::opengloves;

void setup()
{
    auto* communication = AutoConfig::createTransport();

    auto input_sensors = AutoConfig::createInput();

    OpenGlovesTrackingComponent<og::AlphaEncoding>::Config tracking_config(
      CALIBRATION_DURATION,
      CALIBRATION_ALWAYS_CALIBRATE
    );
    auto* og_tracking =
      new OpenGlovesTrackingComponent<og::AlphaEncoding>(tracking_config, input_sensors, communication);

    auto* og_tracking_task =
      new SenseShift::FreeRTOS::ComponentUpdateTask<OpenGlovesTrackingComponent<og::AlphaEncoding>>(
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

    auto* og_ffb = new OpenGlovesForceFeedbackComponent<og::AlphaEncoding>(output_writers, communication);

    auto* og_ffb_task =
      new ::SenseShift::FreeRTOS::ComponentUpdateTask<OpenGlovesForceFeedbackComponent<og::AlphaEncoding>>(
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

void loop()
{
    vTaskDelete(NULL);
}
