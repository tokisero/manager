#include "VectorIt.h"

template<typename T>
void VectorIt<T>::resize(size_t new_capacity) {
    T* new_data = new T[new_capacity];
    for (size_t i = 0; i < size; ++i)
        new_data[i] = data[i];
    delete [] data;
    data = new_data;
    capacity = new_capacity;
}

template<typename T>
VectorIt<T>::Iterator::Iterator(T *p) : ptr(p) {};

template<typename T>
T &VectorIt<T>::Iterator::operator*() {
    return *ptr;
}

template<typename T>
VectorIt<T>::Iterator &VectorIt<T>::Iterator::operator++() {
    ++ptr;
    return *this;
}

template<typename T>
bool VectorIt<T>::Iterator::operator!=(const VectorIt::Iterator &other) const {
    return ptr != other.ptr;
}

template<typename T>
bool VectorIt<T>::Iterator::operator==(const VectorIt::Iterator &other) const {
    return ptr == other.ptr;
}

template<typename T>
bool VectorIt<T>::Iterator::operator<(const VectorIt::Iterator &other) const {
    return ptr < other.ptr;
}

template<typename T>
bool VectorIt<T>::Iterator::operator>(const VectorIt<T>::Iterator &other) const {
    return ptr > other.ptr;
}

template<typename T>
bool VectorIt<T>::Iterator::operator<=(const VectorIt::Iterator &other) const {
    return ptr <= other.ptr;
}

template<typename T>
bool VectorIt<T>::Iterator::operator>=(const VectorIt::Iterator &other) const {
    return ptr >= other.ptr;
}

template<typename T>
VectorIt<T>::Iterator VectorIt<T>::Iterator::operator+(int d) {
    return Iterator(ptr + d);
}

template<typename T>
VectorIt<T>::Iterator VectorIt<T>::Iterator::operator-(int d) {
    return Iterator(ptr - d);
}

template<typename T>
int VectorIt<T>::Iterator::operator-(const Iterator& other) {
    return ptr - other.ptr;
}

template<typename T>
VectorIt<T>::VectorIt() : data(nullptr), capacity(0), size(0) {};

template<typename T>
VectorIt<T>::VectorIt(const VectorIt<T>& other) : data(nullptr), capacity(0), size(0) {
    resize(other.size);
    for (size_t i = 0; i < other.size; ++i)
        data[i] = other.data[i];
    size = other.size;
}

template<typename T>
VectorIt<T>::VectorIt(VectorIt<T>&& other) noexcept : data(other.data), capacity(other.capacity),
    size(other.size) {
    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;
}

template<typename T>
VectorIt<T> &VectorIt<T>::operator=(const VectorIt<T> &other) {
    if (this != &other) {
        delete [] data;
        data = nullptr;
        resize(other.size);
        for (size_t i = 0; i < size; ++i)
            data[i] = other.data[i];
        size = other.size;
    }

    return *this;
}

template<typename T>
VectorIt<T> &VectorIt<T>::operator=(VectorIt<T> &&other) noexcept {
    if (this != &other) {
        delete [] data;
        data = other.data;
        capacity = other.capacity;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    return *this;
}

template<typename T>
VectorIt<T>::~VectorIt() {
    delete [] data;
}

template<typename T>
void VectorIt<T>::push_back(const T &value) {
    if (size >= capacity) {
        size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
        T* new_data = new T[new_capacity];

        for (size_t i = 0; i < size; ++i) {
            new_data[i] = std::move(data[i]);
        }

        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

    data[size++] = value;
}

template<typename T>
void VectorIt<T>::pop_back() {
    if (size > 0)
        --size;
    else
        throw std::out_of_range("Vector is empty");
}

template<typename T>
T &VectorIt<T>::operator[](size_t index) {
    if (index >= size)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template<typename T>
size_t VectorIt<T>::get_size() const {
    return size;
}

template<typename T>
bool VectorIt<T>::is_empty() {
    return size == 0;
}

template<typename T>
VectorIt<T>::Iterator VectorIt<T>::begin() {
    return Iterator(data);
}

template<typename T>
VectorIt<T>::Iterator VectorIt<T>::end() {
    return Iterator(data + size);
}

template<typename T>
void VectorIt<T>::remove_at(size_t index) {
    if (index >= size)
        throw std::out_of_range("Index out of range");

    for (size_t i = index; i < size; ++i)
        data[i] = data[i + 1];

    --size;
}

template<typename T>
void VectorIt<T>::remove(const T &value) {
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == value) {
            for (size_t j = i; j < size - 1; ++j) {
                data[j] = data[j + 1];
            }
            --size;
            data[size] = T();
            return;
        }
    }
    throw std::runtime_error("Element not found");
}

template<typename T>
std::vector<T> VectorIt<T>::toStdVector() const {
    return std::vector<T>(data, data + size);
}

template<typename T>
void VectorIt<T>::fromStdVector(const std::vector<T> &vec) {
    size = vec.size();
    capacity = size;
    data = new T[capacity];
    for (size_t i = 0; i < size; ++i)
        data[i] = std::move(vec[i]);
}

template<typename T>
template<typename Predicate>
VectorIt<T> VectorIt<T>::find(Predicate pred) {

    VectorIt<T> vector_result;

    for (size_t i = 0; i < size; ++i) {
        if (find(data[i]))
            vector_result.push_back(data[i]);
    }

    return vector_result;
}

