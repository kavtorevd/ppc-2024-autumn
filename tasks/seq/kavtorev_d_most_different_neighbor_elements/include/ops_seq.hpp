#pragma once

#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "core/task/include/task.hpp"

namespace ppc {
namespace kavtorev_d_most_different_neighbor_elements_seq {

class testTaskSequential : public ppc::core::Task {
 public:
  explicit testTaskSequential(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {
    if (!taskData_ || taskData_->inputs.empty() || taskData_->inputs[0] == nullptr) {
      std::cerr << "Error: taskData_->inputs is empty or null" << std::endl;
      return;
    }

    if (taskData_->inputs_count.empty() || taskData_->inputs_count[0] <= 0) {
      std::cerr << "Error: taskData_->inputs_count is empty or invalid" << std::endl;
      return;
    }

    size_t size = taskData_->inputs_count[0];
    data_.resize(size);
    std::memcpy(data_.data(), taskData_->inputs[0], size * sizeof(int));
  }

  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

  void set_data(const std::vector<int>& data);
  std::pair<int, int> get_most_different_neighbors() const;

 private:
  std::vector<int> data_;
  std::pair<int, int> result_;
};

}  // namespace kavtorev_d_most_different_neighbor_elements_seq
}  // namespace ppc
