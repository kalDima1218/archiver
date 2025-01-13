//
// Created by Дмитрий Калугин on 17.10.2024.
//

#ifndef CPP_HSE_HEAP_H
#define CPP_HSE_HEAP_H

#include <vector>
#include <stdexcept>

template <typename T, typename Compare>
class Heap {
private:
    std::vector<T> data_;
    Compare cmp_;

    void SiftUp(size_t index) {
        while (index > 0) {
            size_t parent_index = (index - 1) / 2;
            if (cmp_(data_[index], data_[parent_index])) {
                std::swap(data_[index], data_[parent_index]);
                index = parent_index;
            } else {
                break;
            }
        }
    }

    void SiftDown(size_t index) {
        size_t size = data_.size();
        while (index < size) {
            size_t left_child = 2 * index + 1;
            size_t right_child = 2 * index + 2;
            size_t selected_child = index;
            if (left_child < size && cmp_(data_[left_child], data_[selected_child])) {
                selected_child = left_child;
            }
            if (right_child < size && cmp_(data_[right_child], data_[selected_child])) {
                selected_child = right_child;
            }
            if (selected_child != index) {
                std::swap(data_[index], data_[selected_child]);
                index = selected_child;
            } else {
                break;
            }
        }
    }

public:
    Heap() {
    }

    Heap(std::initializer_list<T> il) {
        for (const T& i : il) {
            Push(i);
        }
    }

    void Push(const T& value) {
        data_.push_back(value);
        SiftUp(data_.size() - 1);
    }

    void Pop() {
        if (IsEmpty()) {
            throw std::runtime_error("Heap::Pop Error: heap is empty");
        }
        data_[0] = data_.back();
        data_.pop_back();
        if (!IsEmpty()) {
            SiftDown(0);
        }
    }

    T Top() const {
        return data_[0];
    }

    size_t Size() const {
        return data_.size();
    }

    bool IsEmpty() const {
        return data_.empty();
    }
};

#endif  // CPP_HSE_HEAP_H
