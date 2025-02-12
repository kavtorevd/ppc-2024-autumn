// Copyright 2024 Nesterov Alexander
#include "seq/kavtorev_d_most_different_neighbor_elements/include/ops_seq.hpp"

#include <thread>
#include <algorithm>
#include <cmath>

using namespace std::chrono_literals;

namespace ppc {
namespace kavtorev_d_most_different_neighbor_elements_seq {

template <class InOutType>
bool MostDifferentNeighborElements<InOutType>::pre_processing() {
  internal_order_test();
  input_.assign(reinterpret_cast<InOutType*>(taskData->inputs[0]), 
                reinterpret_cast<InOutType*>(taskData->inputs[0]) + taskData->inputs_count[0]);
  max_difference_ = 0;
  return true;
}

template <class InOutType>
bool MostDifferentNeighborElements<InOutType>::validation() {
  internal_order_test();
  return taskData->inputs_count[0] > 1 && taskData->outputs_count[0] == 1;
}

template <class InOutType>
bool MostDifferentNeighborElements<InOutType>::run() {
  internal_order_test();
  for (size_t i = 1; i < input_.size(); ++i) {
    max_difference_ = std::max(max_difference_, std::abs(input_[i] - input_[i - 1]));
  }
  return true;
}

template <class InOutType>
bool MostDifferentNeighborElements<InOutType>::post_processing() {
  internal_order_test();
  reinterpret_cast<InOutType*>(taskData->outputs[0])[0] = max_difference_;
  return true;
}

// Explicit template instantiation
template class MostDifferentNeighborElements<int>;

}  // namespace kavtorev_d_most_different_neighbor_elements_seq
}  // namespace ppc
