#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "seq/kavtorev_d_most_different_neighbor_elements/include/ops_seq.hpp"

TEST(kavtorev_d_most_different_neighbor_elements_seq, MostDifferentNeighborElements_3_Elements) {
  std::vector<int> vec = {1, 5, 3};
  int expected_result = 4;

  std::vector<int> out(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  ppc::kavtorev_d_most_different_neighbor_elements_seq::MostDifferentNeighborElements<int> testTask(taskDataSeq);

  ASSERT_TRUE(testTask.validation());
  ASSERT_TRUE(testTask.pre_processing());
  ASSERT_TRUE(testTask.run());
  ASSERT_TRUE(testTask.post_processing());

  ASSERT_EQ(out[0], expected_result);
}

TEST(kavtorev_d_most_different_neighbor_elements_seq, MostDifferentNeighborElements_5_Elements) {
  std::vector<int> vec = {2, 3, 10, 5, 6};
  int expected_result = 7;

  std::vector<int> out(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  ppc::kavtorev_d_most_different_neighbor_elements_seq::MostDifferentNeighborElements<int> testTask(taskDataSeq);

  ASSERT_TRUE(testTask.validation());
  ASSERT_TRUE(testTask.pre_processing());
  ASSERT_TRUE(testTask.run());
  ASSERT_TRUE(testTask.post_processing());

  ASSERT_EQ(out[0], expected_result);
}

TEST(kavtorev_d_most_different_neighbor_elements_seq, MostDifferentNeighborElements_Zero_Vectors) {
  std::vector<int> vec = {0, 0, 0};
  int expected_result = 0;

  std::vector<int> out(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  ppc::kavtorev_d_most_different_neighbor_elements_seq::MostDifferentNeighborElements<int> testTask(taskDataSeq);

  ASSERT_TRUE(testTask.validation());
  ASSERT_TRUE(testTask.pre_processing());
  ASSERT_TRUE(testTask.run());
  ASSERT_TRUE(testTask.post_processing());

  ASSERT_EQ(out[0], expected_result);
}
