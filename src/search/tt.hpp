#ifndef SEARCH_TT_HPP
#define SEARCH_TT_HPP

#include <cstdint>
#include <cstring>

namespace search {

template <class T>
class TT {
   public:
    TT(unsigned int mb) {
        if (mb < 0) {
            mb = 1;
        }
        max_entries_ = (mb * 1024 * 1024) / sizeof(T);
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
        entries_[idx] = t;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return max_entries_;
    }

    void clear() noexcept {
        std::memset(entries_, 0, max_entries_ * sizeof(T));
    }

   private:
    [[nodiscard]] std::size_t index(const std::uint64_t hash) const noexcept {
        return hash % max_entries_;
    }

    unsigned int max_entries_;
    T *entries_;
};

}  // namespace search

#endif
