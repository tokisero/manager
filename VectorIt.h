#ifndef VECTORIT_H
#define VECTORIT_H
#include <algorithm>
#include <vector>
#include <stdexcept>


template <typename T>
class VectorIt
{
private:
    T* data;
    size_t capacity;
    size_t size;
    void resize(size_t capacity);


public:
    class Iterator {
    private:
        T* ptr;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator (T* p);

        T& operator*();
        Iterator& operator++();

        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
        bool operator<(const Iterator& other) const;
        bool operator>(const Iterator& other) const;
        bool operator<=(const Iterator& other) const;
        bool operator>=(const Iterator& other) const;

        Iterator operator+(int d);
        Iterator operator-(int d);
        int operator-(const Iterator& other);

        T* operator->() { return ptr; }
        const T* operator->() const { return ptr; }

        Iterator operator--() { return Iterator(--ptr); }
        const Iterator operator++(int) { return Iterator(ptr++); }
        const Iterator operator--(int) { return Iterator(ptr--); }
    };
    Iterator begin();
    Iterator end();

    VectorIt();
    VectorIt(const VectorIt<T>& other);
    VectorIt(VectorIt<T>&& other) noexcept;
    VectorIt& operator=(const VectorIt<T>& other);
    VectorIt& operator=(VectorIt<T>&& other) noexcept;
    ~VectorIt();

    void push_back(const T& value);
    void pop_back();
    size_t get_size() const;
    bool is_empty();
    void remove_at(size_t index);
    void remove(const T& value);
    std::vector<T> toStdVector() const;
    void fromStdVector(const std::vector<T>& vec);

    template<typename Predicate>
    VectorIt<T> find(Predicate pred);
    T& operator[](size_t index);
};




#endif // VECTORIT_H
