#ifndef OPS_MPI_HPP
#define OPS_MPI_HPP

#include <gtest/gtest.h>

#include <algorithm>
#include <boost/mpi.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>
#include <cmath>

#include "core/task/include/task.hpp"

using namespace std::chrono_literals;

namespace kavtorev_d_most_different_neighbor_elements_mpi {

template <typename T>
class MostDifferentNeighborElementsParallel : public ppc::core::Task {
 public:
  explicit MostDifferentNeighborElementsParallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}

  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  boost::mpi::communicator world;

  std::vector<T> input_;
  T size_;
  T max_difference_ = 0;
};

template <typename T>
bool MostDifferentNeighborElementsParallel<T>::pre_processing() {
  internal_order_test();

  if (world.rank() == 0) {
    size_ = reinterpret_cast<int*>(taskData->inputs_count[0])[0];
    auto* vect = reinterpret_cast<int*>(taskData->inputs[0]);
    input_.assign(vect, vect + size_);
    max_difference_ = 0;
  }

  return true;
}

template <typename T>
bool MostDifferentNeighborElementsParallel<T>::validation() {
  internal_order_test();

  if (world.rank() == 0) {
    return taskData->inputs_count[0] > 1 && taskData->outputs_count[0] == 1;
  }
  return true;
}



template <typename T>
bool MostDifferentNeighborElementsParallel<T>::run() {
  internal_order_test();  // Тестируем порядок элементов (если необходимо)

  int rank = world.rank();  // Получаем ID текущего процесса
  int size = world.size();  // Получаем количество процессов

  int delta;
  int remainder;
  if (rank == 0) {
    delta = input_.size() / size;  // Разбиение данных между процессами
    remainder = input_.size() % size;
  }

  // Рассылаем информацию о дельте и остатке всем процессам
  boost::mpi::broadcast(world, delta, 0);
  boost::mpi::broadcast(world, remainder, 0);

  std::vector<int> distr(size, delta);  // Количество элементов, обрабатываемых каждым процессом
  std::vector<int> displ(size, 0);      // Индексы начала данных для каждого процесса

  // Корректируем распределение для остаточных элементов
  for (int i = 0; i < remainder; ++distr[i], ++i);
  for (int i = 1; i < size; ++i) {
    displ[i] = displ[i - 1] + distr[i - 1];  // Вычисляем смещения
  }

  // Локальный массив для каждого процесса
  std::vector<T> local_data(distr[rank]);

  // Разделяем данные между процессами с помощью scatterv
  if (rank == 0) {
    boost::mpi::scatterv(world, input_, distr, displ, local_data.data(), distr[0], 0);
  } else {
    boost::mpi::scatterv(world, local_data.data(), distr[rank], 0);
  }

  // Локальное вычисление максимальной разницы в соседних элементах
  T local_max_diff = 0;
  for (size_t i = 1; i < local_data.size(); ++i) {
    local_max_diff = std::max(local_max_diff, std::abs(local_data[i] - local_data[i - 1]));
  }

  // Сбор максимальных разниц с помощью reduce
  T global_max_diff = 0;
  boost::mpi::reduce(world, local_max_diff, global_max_diff, boost::mpi::maximum<T>(), 0);

  // Процесс 0 сохраняет глобальный результат
  if (rank == 0) {
    max_difference_ = global_max_diff;
  }

  return true;
}


template <typename T>
bool MostDifferentNeighborElementsParallel<T>::post_processing() {
  internal_order_test();

  if (world.rank() == 0) {
    reinterpret_cast<T*>(taskData->outputs[0])[0] = max_difference_;
  }

  return true;
}


template <typename T> 
class MostDifferentNeighborElements : public ppc::core::Task {
 public:
  explicit MostDifferentNeighborElements(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}

  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<T> input_;
  std::vector<T> size_;
  T max_difference_ = 0;
};


template <typename T>
bool MostDifferentNeighborElements<T>::pre_processing() {
  internal_order_test();
  input_.assign(reinterpret_cast<T*>(taskData->inputs[0]), 
                reinterpret_cast<T*>(taskData->inputs[0]) + taskData->inputs_count[0]);
  max_difference_ = 0;
  return true;
}

template <typename T>
bool MostDifferentNeighborElements<T>::validation() {
  internal_order_test();
  return taskData->inputs_count[0] > 1 && taskData->outputs_count[0] == 1;
}

template <typename T>
bool MostDifferentNeighborElements<T>::run() {
  internal_order_test();
  for (size_t i = 1; i < input_.size(); ++i) {
    max_difference_ = std::max(max_difference_, std::abs(input_[i] - input_[i - 1]));
  }
  return true;
}

template <typename T>
bool MostDifferentNeighborElements<T>::post_processing() {
  internal_order_test();
  reinterpret_cast<T*>(taskData->outputs[0])[0] = max_difference_;
  return true;
}

}  // namespace belov_a_max_value_of_matrix_elements_mpi

#endif  // OPS_MPI_HPP