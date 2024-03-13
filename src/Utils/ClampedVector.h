#pragma once

#include <algorithm>
#include <cstddef>

#include <cstdint>
#include <utility>
#include <vector>

namespace image_processor {

template <class T>
class ClampedVector {
public:
    ClampedVector() = default;

    ~ClampedVector() = default;

    explicit ClampedVector(size_t size) : vector_(size, T()) {
    }

    ClampedVector(size_t size, const T& value) : vector_(size, value) {
    }

    ClampedVector(const ClampedVector& other) : vector_(other.vector_) {
    }

    ClampedVector(ClampedVector&& other) noexcept : vector_(std::move(other.vector_)) {
    }

    ClampedVector& operator=(const ClampedVector& other) {
        vector_ = other.vector_;
        return *this;
    }

    ClampedVector& operator=(ClampedVector&& other) noexcept {
        Swap(other);
        return *this;
    }

    const T& operator[](int64_t i) const {
        return vector_[std::clamp(i, 0l, static_cast<int64_t>(vector_.size() - 1))];
    }

    T& operator[](int64_t i) {
        return vector_[std::clamp(i, 0l, static_cast<int64_t>(vector_.size() - 1))];
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

    constexpr Iterator Insert(ConstIterator it, size_t count, const T& value) {
        return vector_.insert(it, count, value);
    }

    template <class InputIt>
    constexpr Iterator Insert(ConstIterator it, InputIt first, InputIt last) {
        return vector_.insert(it, first, last);
    }

    template <class... Args>
    constexpr Iterator Emplace(ConstIterator it, Args&&... args) {
        return Insert(it, T(std::forward<Args>(args)...));
    }

    constexpr Iterator Erase(ConstIterator it) {
        return vector_.erase(it);
    }

    constexpr Iterator Erase(ConstIterator first, ConstIterator last) {
        return vector_.erase(first, last);
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
        vector_.resize(new_size, value);
    }

    constexpr void Swap(ClampedVector& other) {
        std::swap(vector_, other.vector_);
    }

private:
    std::vector<T> vector_;
};

}  // namespace image_processor
