// Copyright 2024 Kavtorev Dmitry
#include "seq/kavtorev_d_most_different_neighbor_elements/include/ops_seq.hpp"

#include <cmath>    // для std::abs
#include <utility>  // для std::pair
#include <iostream> // для отладочного вывода

namespace ppc {
namespace kavtorev_d_most_different_neighbor_elements_seq {

bool testTaskSequential::pre_processing() {
    if (taskData->inputs.empty() || taskData->inputs[0] == nullptr) {
        std::cerr << "Pre-processing failed: inputs are empty or null." << std::endl;
        if (taskData->inputs.empty()) {
            std::cerr << "Inputs vector is empty." << std::endl;
        } else {
            std::cerr << "First input is null." << std::endl;
        }
        return false;
    }

    // Отладка входных данных
    auto* input_data = reinterpret_cast<int*>(taskData->inputs[0]);
    size_t input_size = taskData->inputs_count[0];
    std::cout << "Pre-processing: Input size = " << input_size << std::endl;

    for (size_t i = 0; i < input_size; ++i) {
        std::cout << "Input[" << i << "] = " << input_data[i] << std::endl;
    }

    return true;
}


bool testTaskSequential::validation() {
    if (taskData->inputs_count.empty() || taskData->outputs_count.empty() ||
        taskData->inputs[0] == nullptr || taskData->outputs[0] == nullptr) {
        std::cerr << "Validation failed: invalid taskData inputs or outputs." << std::endl;
        return false;
    }

    // Дополнительная проверка для пустых данных
    size_t input_size = taskData->inputs_count[0];

    if (input_size == 0) {
        std::cerr << "Validation passed: input data is empty." << std::endl;
        return true;  // Пустой массив считаем допустимым
    }

    return true;
}


bool testTaskSequential::run() {
    if (taskData->inputs.empty() || taskData->inputs[0] == nullptr) {
        std::cerr << "Run failed: inputs are empty or null." << std::endl;
        return false;
    }

    // Преобразование входных данных
    auto* input_data = reinterpret_cast<int*>(taskData->inputs[0]);
    size_t input_size = taskData->inputs_count[0];

    // Проверка на пустой массив
    if (input_size == 0) {
        std::cerr << "Run successful: no data to process, returning default pair (0, 0)." << std::endl;
        result_ = {0, 0};  // Вернем стандартную пару для пустых данных
        return true;
    }

    // Локальная обработка для поиска максимальной разницы
    int max_diff = 0;
    std::pair<int, int> max_pair = {input_data[0], input_data[1]};

    for (size_t i = 0; i < input_size - 1; ++i) {
        int diff = std::abs(input_data[i] - input_data[i + 1]);
        if (diff > max_diff) {
            max_diff = diff;
            max_pair = {input_data[i], input_data[i + 1]};
        }
    }

    result_ = max_pair;

    std::cout << "Run successful: most different neighbors are (" 
              << result_.first << ", " << result_.second << ") with difference " 
              << max_diff << std::endl;

    return true;
}



bool testTaskSequential::post_processing() {
    if (taskData->outputs.empty() || taskData->outputs[0] == nullptr) {
        std::cerr << "Post-processing failed: outputs are empty or null." << std::endl;
        return false;
    }

    // Запись значений в taskData->outputs
    auto* output = reinterpret_cast<int*>(taskData->outputs[0]);
    output[0] = result_.first;  // Первый элемент пары
    output[1] = result_.second; // Второй элемент пары

    std::cout << "Post-processing successful: result stored in outputs [" 
              << output[0] << ", " << output[1] << "]." << std::endl;

    return true;
}




void testTaskSequential::set_data(const std::vector<int>& data) {
    data_ = data;
}

std::pair<int, int> testTaskSequential::get_most_different_neighbors() const {
    return result_;
}

}  // namespace kavtorev_d_most_different_neighbor_elements_seq
}  // namespace ppc
