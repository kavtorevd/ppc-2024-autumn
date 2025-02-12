// Copyright 2023 Nesterov Alexander
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <vector>

#include "mpi/kavtorev_d_most_different_neighbor_elements/include/ops_mpi.hpp"
#include "core/task/include/task.hpp"

namespace kavtorev_d_most_different_neighbor_elements_mpi {

std::vector<int> getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100;  // Генерация случайных чисел от 0 до 99
  }
  return vec;
}

  TEST(kavtorev_d_most_different_neighbor_elements_mpi, Test) {
    boost::mpi::communicator world;
    int size = 100;
  
    std::vector<int> global_vector = getRandomVector(size);
    std::vector<int32_t> global_different(1, 0);
  
    // Create TaskData
    std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
  
    if (world.rank() == 0) {
      taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_vector.data()));
      taskDataPar->inputs_count.emplace_back(size);
      taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t*>(global_different.data()));
      taskDataPar->outputs_count.emplace_back(global_different.size());
    }
  
    kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborElementsParallel<int> testMpiTaskParallel(taskDataPar);
    ASSERT_EQ(testMpiTaskParallel.validation(), true);
    testMpiTaskParallel.pre_processing();
    testMpiTaskParallel.run();
    testMpiTaskParallel.post_processing();
  
    if (world.rank() == 0) {
      // Create data
      std::vector<int32_t> reference_different(1, 0);
  
      // Create TaskData
      std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
      taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_vector.data()));
      taskDataSeq->inputs_count.emplace_back(global_vector.size());
      taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(reference_different.data()));
      taskDataSeq->outputs_count.emplace_back(reference_different.size());
  
      // Create Task
      kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborElements<int> testMpiTaskSequential(
          taskDataSeq);
      ASSERT_EQ(testMpiTaskSequential.validation(), true);
      testMpiTaskSequential.pre_processing();
      testMpiTaskSequential.run();
      testMpiTaskSequential.post_processing();
  
      ASSERT_EQ(reference_different[0], global_different[0]);
    }
  }
}
// TEST(kavtorev_d_most_different_neighbor_elements_mpi, Test_MostDifferentNeighbors_Parallel) {
//   boost::mpi::communicator world;
//   std::vector<int> global_vec;
//   std::pair<int, int> global_result(0, 0);

//   std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();

//   if (world.rank() == 0) {
//     const int count_size_vector = 240;
//     global_vec = kavtorev_d_most_different_neighbor_elements_mpi::getRandomVector(count_size_vector);
//     taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_vec.data()));
//     taskDataPar->inputs_count.emplace_back(global_vec.size());
//     taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t*>(&global_result));
//     taskDataPar->outputs_count.emplace_back(sizeof(global_result));
//   }

//   MostDifferentNeighborsParallel testMpiTaskParallel(taskDataPar, "max_diff_neighbors");
//   ASSERT_EQ(testMpiTaskParallel.validation(), true);
//   testMpiTaskParallel.pre_processing();
//   testMpiTaskParallel.run();
//   testMpiTaskParallel.post_processing();

//   if (world.rank() == 0) {
//     std::pair<int, int> reference_result(0, 0);
//     std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
//     taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_vec.data()));
//     taskDataSeq->inputs_count.emplace_back(global_vec.size());
//     taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&reference_result));
//     taskDataSeq->outputs_count.emplace_back(sizeof(reference_result));

//     MostDifferentNeighborsSequential testMpiTaskSequential(taskDataSeq, "max_diff_neighbors");
//     ASSERT_EQ(testMpiTaskSequential.validation(), true);
//     testMpiTaskSequential.pre_processing();
//     testMpiTaskSequential.run();
//     testMpiTaskSequential.post_processing();

//     ASSERT_EQ(reference_result, global_result);
//   }
// }

// }  // namespace kavtorev_d_most_different_neighbor_elements_mpi

