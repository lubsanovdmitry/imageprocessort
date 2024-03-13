#pragma once

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>

namespace image_processor {

template <typename T>
class ClampedVector {
public:
    ClampedVector() = default;

    explicit ClampedVector(size_t size) : vector_(size, T()) {
    }

    ClampedVector(size_t size, const T& value) : vector_(size, T()) {
    }

    ClampedVector(const ClampedVector& other) = default;

    ClampedVector(const ClampedVector&& other) noexcept = default;

    ClampedVector& operator=(const ClampedVector& other) = default;

    ClampedVector& operator=(const ClampedVector&& other) noexcept = default;

    const T& operator[](size_t i) const {
        return vector_[std::clamp(i, 0ul, vector_.size() - 1)];
    }

    T& operator[](size_t i) {
        return vector_[std::clamp(i, 0ul, vector_.size() - 1)];
    }

    size_t Size() const noexcept {
        return vector_.size();
    }

    bool Empty() const noexcept {
        return vector_.empty();
    }

    void Clear() {
        vector_.clear();
    }

    using Iterator = typename std::vector<T>::iterator;

    using ConstIterator = typename std::vector<T>::const_iterator;

    Iterator begin() noexcept {  // NOLINT
        return vector_.begin();
    }

    ConstIterator begin() const noexcept {  // NOLINT
        return vector_.begin();
    }

    ConstIterator cbegin() const noexcept {  // NOLINT
        return vector_.cbegin();
    }

    Iterator end() noexcept {  // NOLINT
        return vector_.end();
    }

    ConstIterator end() const noexcept {  // NOLINT
        return vector_.end();
    }

    ConstIterator cend() const noexcept {  // NOLINT
        return vector_.cend();
    }

    constexpr Iterator Insert(ConstIterator it, const T& value) {
        return vector_.insert(it, value);
    }

    constexpr Iterator Insert(ConstIterator it, T&& value) {
        return vector_.insert(it, std::move(value));
    }

    template <class InputIt>
    constexpr Iterator Insert(ConstIterator it, InputIt first, InputIt last) {
        return vector_.insert(it, first, last);
    }

    template <class... Args>
    constexpr Iterator Emplace(ConstIterator it, Args&&... args) {
        return Insert(it, T(std::forward<Args>(args)...));
    }

    constexpr void PushBack(const T& value) {
        vector_.push_back(value);
    }

    constexpr void PushBack(T&& value) {
        vector_.push_back(std::move(value));
    }

    template <class... Args>
    constexpr Iterator EmplaceBack(Args&&... args) {
        return PushBack(T(std::forward<Args>(args)...));
    }

    constexpr void PopBack() {
        vector_.pop_back();
    }

    void Resize(size_t new_size) {
        vector_.resize(new_size);
    }

    void Resize(size_t new_size, const T& value) {
        vector_.resize(new_size, T());
    }

private:
    std::vector<T> vector_;
};

}  // namespace image_processor
