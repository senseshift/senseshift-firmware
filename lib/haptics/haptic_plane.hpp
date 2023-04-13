#pragma once

#include <abstract_actuator.hpp>
#include <utility.hpp>
#include <types.hpp>

#include <list>
#include <map>
#include <vector>

namespace OH {
  /**
   * Output "plane" (e.g. Chest, Palm, Finger, etc.)
   */
  class HapticPlane {
   protected:
    std::list<oh_output_point_t> points{};
    oh_output_writers_map_t writers{};
    std::map<oh_output_point_t, oh_output_state_t> states{};

    void setOutputs(oh_output_writers_map_t&);

   public:
    HapticPlane(oh_output_writers_map_t& outputs) {
      this->setOutputs(outputs);
    };
    std::list<oh_output_point_t>* getOutputPoints(void) {
      return &this->points;
    };
    std::map<oh_output_point_t, oh_output_state_t>* getOutputStates(void) {
      return &this->states;
    };
    virtual void writeOutput(const oh_output_data_t&);
    void setup();
  };

  class HapticPlane_Closest : public HapticPlane {
   protected:
    oh_output_point_t findClosestPoints(std::list<oh_output_point_t>& pts, const oh_output_point_t& target);
    void setOutputs(oh_output_writers_map_t&);

   public:
    HapticPlane_Closest(oh_output_writers_map_t& outputs) : HapticPlane(outputs) {};
    void writeOutput(const oh_output_data_t&) override;
  };

  /**
   * Re-maps a point index to output coordinate.
   * @tparam _Tp The type of the point index.
   */
  template <typename _Tp>
  inline oh_output_point_t* mapPoint(_Tp x, _Tp y, _Tp x_max, _Tp y_max) {
    const oh_output_coord_t x_coord = map<_Tp>(x + 1, 0, x_max + 2, 0, OH_OUTPUT_COORD_MAX);
    const oh_output_coord_t y_coord = map<_Tp>(y + 1, 0, y_max + 2, 0, OH_OUTPUT_COORD_MAX);

    return new oh_output_point_t(x_coord, y_coord);
  }

  template <typename _Tp>
  inline std::map<oh_output_point_t, _Tp*> mapMatrixCoordinates(std::vector<std::vector<_Tp*>> map2d) {
    std::map<oh_output_point_t, _Tp*> points{};

    size_t y_size = map2d.size();
    size_t y_max = y_size - 1;

    for (size_t y = 0; y < y_size; ++y) {
      auto row = map2d.at(y);
      size_t x_size = row.size();
      size_t x_max = x_size - 1;

      for (size_t x = 0; x < x_size; ++x) {
        AbstractActuator* wr = row.at(x);
        oh_output_point_t* coord = mapPoint(x, y, x_max, y_max);

        points[*coord] = wr;
      }
    }

    return points;
  };
}  // namespace OH
