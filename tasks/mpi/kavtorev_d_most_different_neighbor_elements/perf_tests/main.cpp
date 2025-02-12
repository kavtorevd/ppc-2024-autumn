// #include <gtest/gtest.h>
// #include <mpi.h>
// #include <vector>
// #include <memory>
// #include "core/perf/include/perf.hpp"
// #include "mpi/kavtorev_d_most_different_neighbor_elements/include/ops_mpi.hpp"

// TEST(kavtorev_d_most_different_neighbor_elements_mpi, test_pipeline_run) {
//   boost::mpi::communicator world;
//   int rank, size;
//   // MPI_Comm_rank(world, &rank);  // Получаем ранг процесса
//   // MPI_Comm_size(world, &size);  // Получаем количество процессов

//   std::vector<int> global_vec;
//   std::vector<int32_t> global_sum(1, 0);

//   // Создание TaskData
//   std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
//   int count_size_vector;
//   if (rank == 0) {
//     count_size_vector = 120;  // Размер вектора
//     global_vec = std::vector<int>(count_size_vector, 1);  // Заполнение вектора единицами
//     taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_vec.data()));
//     taskDataPar->inputs_count.emplace_back(global_vec.size());
//     taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t*>(global_sum.data()));
//     taskDataPar->outputs_count.emplace_back(global_sum.size());
//   }

//   // Инициализация параллельной задачи
//   auto testMpiTaskParallel = std::make_shared<kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsParallel>(taskDataPar, "+");

//   // Логирование данных перед валидацией
//   if (rank == 0) {
//     std::cout << "Validation inputs_count: " << taskDataPar->inputs_count[0] << std::endl;
//     std::cout << "Validation outputs_count: " << taskDataPar->outputs_count[0] << std::endl;
//   }

//   // Проверка валидации
//   ASSERT_EQ(testMpiTaskParallel->validation(), true);

//   // Предобработка, выполнение и постобработка задачи
//   testMpiTaskParallel->pre_processing();
//   testMpiTaskParallel->run();
//   testMpiTaskParallel->post_processing();

//   // Создание атрибутов для измерения производительности
//   auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
//   perfAttr->num_running = 10;

//   // Используем MPI_Wtime для замера времени
//   double start_time = MPI_Wtime();
  
//   // Присваиваем лямбду для времени
//   perfAttr->current_timer = [start_time]() -> double { 
//     return MPI_Wtime() - start_time; 
//   };

//   // Создание результатов производительности
//   auto perfResults = std::make_shared<ppc::core::PerfResults>();

//   // Создание и запуск анализа производительности
//   auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testMpiTaskParallel);
//   perfAnalyzer->pipeline_run(perfAttr, perfResults);

//   // Вывод результатов только для процесса 0
//   if (rank == 0) {
//     ppc::core::Perf::print_perf_statistic(perfResults);  // Печать статистики
//     ASSERT_EQ(count_size_vector, global_sum[0]);  // Проверка, что результат соответствует размеру вектора
//   }

//   MPI_Finalize();  // Завершение работы с MPI
// }

// TEST(kavtorev_d_most_different_neighbor_elements_mpi, test_task_run) {
//   boost::mpi::communicator world;

//   int rank, size;
//   MPI_Comm_rank(world, &rank);  // Получаем ранг процесса
//   MPI_Comm_size(world, &size);  // Получаем количество процессов

//   std::vector<int> global_vec;
//   std::vector<int32_t> global_sum(1, 0);

//   // Создание TaskData
//   std::shared_ptr<ppc::core::TaskData> taskDataPar = std::make_shared<ppc::core::TaskData>();
//   int count_size_vector;
//   if (rank == 0) {
//     count_size_vector = 120;  // Размер вектора
//     global_vec = std::vector<int>(count_size_vector, 1);  // Заполнение вектора единицами
//     taskDataPar->inputs.emplace_back(reinterpret_cast<uint8_t*>(global_vec.data()));
//     taskDataPar->inputs_count.emplace_back(global_vec.size());
//     taskDataPar->outputs.emplace_back(reinterpret_cast<uint8_t*>(global_sum.data()));
//     taskDataPar->outputs_count.emplace_back(global_sum.size());
//   }

//   // Инициализация параллельной задачи
//   auto testMpiTaskParallel = std::make_shared<kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsParallel>(taskDataPar, "+");

//   // Логирование данных перед валидацией
//   if (rank == 0) {
//     std::cout << "Validation inputs_count: " << taskDataPar->inputs_count[0] << std::endl;
//     std::cout << "Validation outputs_count: " << taskDataPar->outputs_count[0] << std::endl;
//   }

//   // Проверка валидации
//   ASSERT_EQ(testMpiTaskParallel->validation(), true);

//   // Предобработка, выполнение и постобработка задачи
//   testMpiTaskParallel->pre_processing();
//   testMpiTaskParallel->run();
//   testMpiTaskParallel->post_processing();

//   // Создание атрибутов для измерения производительности
//   auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
//   perfAttr->num_running = 10;

//   // Используем MPI_Wtime для замера времени
//   double start_time = MPI_Wtime();

//   // Присваиваем лямбду для времени
//   perfAttr->current_timer = [start_time]() -> double { 
//     return MPI_Wtime() - start_time; 
//   };

//   // Создание результатов производительности
//   auto perfResults = std::make_shared<ppc::core::PerfResults>();

//   // Создание и запуск анализа производительности
//   auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testMpiTaskParallel);
//   perfAnalyzer->task_run(perfAttr, perfResults);

//   // Вывод результатов только для процесса 0
//   if (rank == 0) {
//     ppc::core::Perf::print_perf_statistic(perfResults);  // Печать статистики
//     ASSERT_EQ(count_size_vector, global_sum[0]);  // Проверка, что результат соответствует размеру вектора
//   }

//   MPI_Finalize();  // Завершение работы с MPI
// }
