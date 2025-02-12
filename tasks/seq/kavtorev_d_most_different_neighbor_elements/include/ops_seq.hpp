#ifndef MODULES_KAVTOREV_D_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_TASK_HPP_
#define MODULES_KAVTOREV_D_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_TASK_HPP_

#include <memory>
#include <vector>

#include "core/task/include/task.hpp"

namespace ppc {
namespace kavtorev_d_most_different_neighbor_elements_seq {

template <class InOutType>
class MostDifferentNeighborElements : public ppc::core::Task {
 public:
  explicit MostDifferentNeighborElements(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}

  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<InOutType> input_;
  std::vector<InOutType> size_;
  InOutType max_difference_ = 0;
};

}  // namespace kavtorev_d_most_different_neighbor_elements_seq
}  // namespace ppc

#endif  // MODULES_KAVTOREV_D_MOST_DIFFERENT_NEIGHBOR_ELEMENTS_TASK_HPP_
