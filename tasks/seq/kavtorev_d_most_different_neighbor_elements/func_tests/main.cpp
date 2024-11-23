#include <gtest/gtest.h>

#include <memory>
#include <utility>
#include <vector>

#include "seq/kavtorev_d_most_different_neighbor_elements/include/ops_seq.hpp"

using InOutType = int;

TEST(kavtorev_d_most_different_neighbor_elements_seq, TestMostDifferentNeighbors10) {
  std::vector<InOutType> in = {1, 11};
  std::pair<InOutType, InOutType> out_element;

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&out_element));
  taskDataSeq->outputs_count.emplace_back(1);

  ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  testTaskSequential.set_data(in);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out_element.first, 1);
  ASSERT_EQ(out_element.second, 11);
}

TEST(kavtorev_d_most_different_neighbor_elements_seq, TestMostDifferentNeighborsSame) {
  std::vector<InOutType> in = {5, 5};
  std::pair<InOutType, InOutType> out_element;

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&out_element));
  taskDataSeq->outputs_count.emplace_back(1);

  ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  testTaskSequential.set_data(in);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out_element.first, 5);
  ASSERT_EQ(out_element.second, 5);
}

TEST(kavtorev_d_most_different_neighbor_elements_seq, TestMostDifferentNeighborsMultiple) {
  std::vector<InOutType> in = {1, 3, 7, 2, 10};
  std::pair<InOutType, InOutType> out_element;

  auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&out_element));
  taskDataSeq->outputs_count.emplace_back(sizeof(out_element));

  ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  testTaskSequential.set_data(in);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(out_element.first, 2);
  ASSERT_EQ(out_element.second, 10);
}
