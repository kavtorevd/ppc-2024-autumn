#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "seq/kalinin_d_vector_dot_product/include/ops_seq.hpp"
#include "seq/kavtorev_d_most_different_neighbor_elements/include/ops_seq.hpp"

TEST(kavtorev_d_most_different_neighbor_elements_seq, test_task_run) {
  std::vector<int> vec = {1, 2, 3, 10, 5};
  int expected_result = 7;

  std::vector<int> out(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
  taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec.data()));
  taskData->inputs_count.emplace_back(vec.size());
  taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));
  taskData->outputs_count.emplace_back(out.size());

  auto task = std::make_shared<ppc::kavtorev_d_most_different_neighbor_elements_seq::MostDifferentNeighborElements<int>>(taskData);

  ASSERT_TRUE(task->validation());
  ASSERT_TRUE(task->pre_processing());
  ASSERT_TRUE(task->run());
  ASSERT_TRUE(task->post_processing());

  ASSERT_EQ(out[0], expected_result);
}

TEST(kavtorev_d_most_different_neighbor_elements_seq, test_pipeline_run) {
  const int count = 100;

  std::vector<int> vec(count);
  for (int i = 0; i < count; ++i) {
    vec[i] = i % 10;
  }
  int expected_result = 9;

  std::vector<int> out(1, 0);

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec.data()));
  taskDataSeq->inputs_count.emplace_back(vec.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));
  taskDataSeq->outputs_count.emplace_back(out.size());

  auto task = std::make_shared<ppc::kavtorev_d_most_different_neighbor_elements_seq::MostDifferentNeighborElements<int>>(taskDataSeq);

  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(task);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);

  ASSERT_EQ(out[0], expected_result);
}


// TEST(kalinin_d_vector_dot_product_seq, test_task_run) {
//   std::vector<int> vec1 = {1, 2, 3};
//   std::vector<int> vec2 = {4, 5, 6};
//   int expected_result = 1 * 4 + 2 * 5 + 3 * 6;

//   std::vector<int> out(1, 0);

//   std::shared_ptr<ppc::core::TaskData> taskData = std::make_shared<ppc::core::TaskData>();
//   taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec1.data()));
//   taskData->inputs_count.emplace_back(vec1.size());
//   taskData->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec2.data()));
//   taskData->inputs_count.emplace_back(vec2.size());
//   taskData->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));
//   taskData->outputs_count.emplace_back(out.size());

//   auto dotProductTask = std::make_shared<ppc::kalinin_d_vector_dot_product_seq::VectorDotProduct<int>>(taskData);

//   ASSERT_TRUE(dotProductTask->validation());
//   ASSERT_TRUE(dotProductTask->pre_processing());
//   ASSERT_TRUE(dotProductTask->run());
//   ASSERT_TRUE(dotProductTask->post_processing());

//   ASSERT_EQ(out[0], expected_result);
// }

// TEST(kalinin_d_vector_dot_product_seq, test_pipeline_run) {
//   const int count = 100;

//   std::vector<int> vec1(count, 1);
//   std::vector<int> vec2(count, 2);
//   int expected_result = count * 2;

//   std::vector<int> out(1, 0);

//   std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
//   taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec1.data()));
//   taskDataSeq->inputs_count.emplace_back(vec1.size());
//   taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(vec2.data()));
//   taskDataSeq->inputs_count.emplace_back(vec2.size());
//   taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(out.data()));
//   taskDataSeq->outputs_count.emplace_back(out.size());

//   auto dotProductTask = std::make_shared<ppc::kalinin_d_vector_dot_product_seq::VectorDotProduct<int>>(taskDataSeq);

//   auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
//   perfAttr->num_running = 10;
//   const auto t0 = std::chrono::high_resolution_clock::now();
//   perfAttr->current_timer = [&] {
//     auto current_time_point = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
//     return static_cast<double>(duration) * 1e-9;
//   };

//   auto perfResults = std::make_shared<ppc::core::PerfResults>();

//   auto perfAnalyzer = std::make_shared<ppc::core::Perf>(dotProductTask);
//   perfAnalyzer->pipeline_run(perfAttr, perfResults);
//   ppc::core::Perf::print_perf_statistic(perfResults);

//   ASSERT_EQ(out[0], expected_result);
// }
