// // Copyright 2023 Kavtorev Dmitry
// #include "mpi/kavtorev_d_most_different_neighbor_elements/include/ops_mpi.hpp"

// #include <algorithm>
// #include <functional>
// #include <random>
// #include <string>
// #include <thread>
// #include <vector>

// using namespace std::chrono_literals;


// bool kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsSequential::pre_processing() {
//   input_ = std::vector<int>(taskData->inputs_count[0]);
//   auto* tmp_ptr = reinterpret_cast<int*>(taskData->inputs[0]);  // Пытаемся безопасно преобразовать указатель
//   for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
//     input_[i] = tmp_ptr[i];  // Копируем данные в локальный вектор
//   }
//   result_ = {0, 0};
//   return true;
// }

// bool kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsSequential::validation() {
//   return taskData->outputs_count[0] == sizeof(std::pair<int, int>);  // Проверка корректности размера
// }

// bool kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsSequential::run() {
//   int max_diff = 0;
//   for (size_t i = 0; i < input_.size() - 1; ++i) {
//     int diff = std::abs(input_[i] - input_[i + 1]);
//     if (diff > max_diff) {
//       max_diff = diff;
//       result_ = {input_[i], input_[i + 1]};
//     }
//   }
//   return true;
// }

// bool kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsSequential::post_processing() {
//   auto* output_ptr = reinterpret_cast<std::pair<int, int>*>(taskData->outputs[0]);
//   *output_ptr = result_;
//   return true;
// }

// bool kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsParallel::pre_processing() {
//   unsigned int delta = 0;

//   if (world.rank() == 0) {
//     delta = taskData->inputs_count[0] / world.size();  // Вычисляем распределение данных
//   }
//   broadcast(world, delta, 0);  // Рассылаем значение delta всем процессам

//   if (world.rank() == 0) {
//     input_ = std::vector<int>(taskData->inputs_count[0]);
//     auto* tmp_ptr = reinterpret_cast<int*>(taskData->inputs[0]);  // Получаем указатель на данные
//     for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
//       input_[i] = tmp_ptr[i];  // Копируем данные в локальный вектор
//     }

//     // Отправляем данные остальным процессам
//     for (int proc = 1; proc < world.size(); proc++) {
//       world.send(proc, 0, input_.data() + proc * delta, delta);
//     }
//   }

//   if (world.rank() == 0) {
//     local_input_ = std::vector<int>(input_.begin(), input_.begin() + delta);
//   } else {
//     local_input_.resize(delta);
//     world.recv(0, 0, local_input_.data(), delta);  // Получаем данные от процесса 0
//   }

//   if (world.rank() == 0 && world.size() > 1) {
//     local_input_.push_back(input_[delta]);  // Добавляем оставшийся элемент для процесса 0
//   }
//   return true;
// }

// bool kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsParallel::validation() {
//   if (world.rank() == 0) {
//     std::cout << "inputs_count[0]: " << taskData->inputs_count[0] << std::endl;
//     std::cout << "outputs_count[0]: " << taskData->outputs_count[0] << std::endl;
//     if (taskData->inputs_count[0] <= 0) {
//       std::cerr << "Error: Invalid inputs_count[0]" << std::endl;
//     }
//     if (taskData->outputs_count[0] != sizeof(std::pair<int, int>)) {
//       std::cerr << "Error: Invalid outputs_count[0]" << std::endl;
//     }
//     return taskData->outputs_count[0] == sizeof(std::pair<int, int>);
//   }
//   return true;
// }


// bool kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsParallel::run() {
//   int local_max_diff = 0;
//   std::pair<int, int> local_result = {0, 0};

//   // Поиск наибольшей разности среди соседних элементов
//   for (size_t i = 0; i < local_input_.size() - 1; ++i) {
//     int diff = std::abs(local_input_[i] - local_input_[i + 1]);
//     if (diff > local_max_diff) {
//       local_max_diff = diff;
//       local_result = {local_input_[i], local_input_[i + 1]};
//     }
//   }

//   Result local = {local_max_diff, local_result.first, local_result.second};
//   Result global = {0, 0, 0};

  
//   // Выполнение редукции для получения глобального максимума
//   reduce(world, local, global, [](const Result& a, const Result& b) {
//     return (a.max_diff > b.max_diff) ? a : b;
//   }, 0);

//   if (world.rank() == 0) {
//     result_ = {global.first, global.second};  // Сохраняем результат в глобальную переменную
//   }
//   return true;
// }

// bool kavtorev_d_most_different_neighbor_elements_mpi::MostDifferentNeighborsParallel::post_processing() {
//   if (world.rank() == 0) {
//     auto* output_ptr = reinterpret_cast<std::pair<int, int>*>(taskData->outputs[0]);
//     *output_ptr = result_;
//   }
//   return true;

