#ifndef SEARCH_TT_HPP
#define SEARCH_TT_HPP

#include <cstdint>
#include <cstring>

namespace search {

template <class T>
class TT {
   public:
    [[nodiscard]] TT(unsigned int mb) : filled_{0} {
        if (mb < 1) {
            mb = 1;
        }
        max_entries_ = (mb * 1024) / sizeof(T) * 1024;
        entries_ = new T[max_entries_];
    }

    ~TT() {
        delete entries_;
    }

    [[nodiscard]] T poll(const std::uint64_t hash) const noexcept {
        const auto idx = index(hash);
        return entries_[idx];
    }

    void add(const std::uint64_t hash, const T &t) noexcept {
        const auto idx = index(hash);
        filled_ += (entries_[idx].hash == 0 ? 1 : 0);
        entries_[idx] = t;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return max_entries_;
    }

    void clear() noexcept {
        filled_ = 0;
        for (std::size_t i = 0; i < max_entries_; ++i) {
            entries_[i] = T();
        }
    }

    [[nodiscard]] int hashfull() const noexcept {
        return 1000 * (static_cast<double>(filled_) / max_entries_);
    }

    void prefetch(const std::uint64_t hash) const noexcept {
        const auto idx = index(hash);
        __builtin_prefetch(&entries_[idx]);
    }

   private:
    [[nodiscard]] std::size_t index(const std::uint64_t hash) const noexcept {
        return hash % max_entries_;
    }

    std::size_t max_entries_;
    std::size_t filled_;
    T *entries_;
};

}  // namespace search

#endif
