#pragma once
#include <vector>
#include <stdexcept>
#include <QModelIndex>

template <typename T>
class Queue {
private:
    std::vector<T> data;

public:
    void enqueue(const T& element) {
        data.push_back(element);
    }
    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty. Cannot dequeue.");
        }
        T frontElement = data.front();
        data.erase(data.begin());
        return frontElement;
    }
    T& front();
    const T& front() const;
    bool isEmpty() const {
        return data.empty();
    }
    size_t size() const;
    std::vector<T> toVector() const {
        return std::vector<T>(data.begin(), data.end());
    }
    void removeElement(const T& element) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (*it == element) {
                data.erase(it);
                return;
            }
        }
    }
    T& at(size_t index) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range.");
        }
        return data[index];
    }

};

