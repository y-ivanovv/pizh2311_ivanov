#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace collection_adapters {

    template<typename T>
    using iterator_t = decltype(std::begin(std::declval<T&>()));

    template<typename Collection, typename Condition>
    class filtered_view {
        Collection collection_;
        Condition condition_;
    public:
        filtered_view(Collection col, Condition cond)
            : collection_(std::move(col)), condition_(std::move(cond)) {
        }

        class iterator {
            iterator_t<Collection> current_, end_;
            Condition* condition_;
            void advance_to_valid() {
                while (current_ != end_ && !(*condition_)(*current_)) ++current_;
            }
        public:
            iterator(iterator_t<Collection> curr, iterator_t<Collection> end, Condition* cond)
                : current_(curr), end_(end), condition_(cond) {
                advance_to_valid();
            }
            auto operator*() const { return *current_; }
            iterator& operator++() { ++current_; advance_to_valid(); return *this; }
            bool operator!=(const iterator& other) const { return current_ != other.current_; }
        };

        iterator begin() { return { std::begin(collection_), std::end(collection_), &condition_ }; }
        iterator end() { return { std::end(collection_), std::end(collection_), &condition_ }; }
    };

    template<typename Condition>
    inline auto where(Condition cond) {
        return [cond](auto&& col) {
            using C = std::remove_cvref_t<decltype(col)>;
            return filtered_view<C, Condition>(std::forward<decltype(col)>(col), cond);
            };
    }

    template<typename Collection, typename Operation>
    class mapped_view {
        Collection collection_;
        Operation operation_;
    public:
        mapped_view(Collection col, Operation op)
            : collection_(std::move(col)), operation_(std::move(op)) {
        }

        class iterator {
            iterator_t<Collection> current_;
            Operation* operation_;
        public:
            iterator(iterator_t<Collection> curr, Operation* op)
                : current_(curr), operation_(op) {
            }
            auto operator*() const { return (*operation_)(*current_); }
            iterator& operator++() { ++current_; return *this; }
            bool operator!=(const iterator& other) const { return current_ != other.current_; }
        };

        iterator begin() { return { std::begin(collection_), &operation_ }; }
        iterator end() { return { std::end(collection_), &operation_ }; }
    };

    template<typename Operation>
    inline auto map(Operation op) {
        return [op](auto&& col) {
            using C = std::remove_cvref_t<decltype(col)>;
            return mapped_view<C, Operation>(std::forward<decltype(col)>(col), op);
            };
    }

    template<typename Collection, typename Count>
    class limited_view {
        Collection collection_;
        Count limit_;
    public:
        limited_view(Collection col, Count cnt)
            : collection_(std::move(col)), limit_(cnt) {
        }

        class iterator {
            iterator_t<Collection> current_;
            Count remaining_;
        public:
            iterator(iterator_t<Collection> curr, Count rem)
                : current_(curr), remaining_(rem) {
            }
            auto operator*() const { return *current_; }
            iterator& operator++() { ++current_; if (remaining_ > 0) --remaining_; return *this; }
            bool operator!=(const iterator& other) const { return remaining_ > 0 && current_ != other.current_; }
        };

        iterator begin() { return { std::begin(collection_), limit_ }; }
        iterator end() { return { std::end(collection_), 0 }; }
    };

    template<typename Count>
    inline auto limit(Count cnt) {
        return [cnt](auto&& col) {
            using C = std::remove_cvref_t<decltype(col)>;
            return limited_view<C, Count>(std::forward<decltype(col)>(col), cnt);
            };
    }

    template<typename Collection, typename Count>
    class skipped_view {
        Collection collection_;
        Count skip_count_;
    public:
        skipped_view(Collection col, Count cnt)
            : collection_(std::move(col)), skip_count_(cnt) {
        }

        class iterator {
            iterator_t<Collection> current_, end_;
        public:
            iterator(iterator_t<Collection> curr, iterator_t<Collection> end, Count to_skip)
                : current_(curr), end_(end) {
                for (Count i = 0; i < to_skip && current_ != end_; ++i) ++current_;
            }
            auto operator*() const { return *current_; }
            iterator& operator++() { ++current_; return *this; }
            bool operator!=(const iterator& other) const { return current_ != other.current_; }
        };

        iterator begin() { return { std::begin(collection_), std::end(collection_), skip_count_ }; }
        iterator end() { return { std::end(collection_), std::end(collection_), 0 }; }
    };

    template<typename Count>
    inline auto skip(Count cnt) {
        return [cnt](auto&& col) {
            using C = std::remove_cvref_t<decltype(col)>;
            return skipped_view<C, Count>(std::forward<decltype(col)>(col), cnt);
            };
    }

    template<typename Collection>
    class reversed_view {
        Collection collection_;
    public:
        reversed_view(Collection col) : collection_(std::move(col)) {}

        class iterator {
            iterator_t<Collection> current_;
        public:
            iterator(iterator_t<Collection> curr) : current_(curr) {}
            auto operator*() const { auto tmp = current_; return *--tmp; }
            iterator& operator++() { --current_; return *this; }
            bool operator!=(const iterator& other) const { return current_ != other.current_; }
        };

        iterator begin() { return { std::end(collection_) }; }
        iterator end() { return { std::begin(collection_) }; }
    };

    inline auto reversed = [](auto&& col) {
        using C = std::remove_cvref_t<decltype(col)>;
        return reversed_view<C>(std::forward<decltype(col)>(col));
        };

    template<typename Collection>
    class keys_view {
        Collection collection_;
    public:
        keys_view(Collection col) : collection_(std::move(col)) {}

        class iterator {
            iterator_t<Collection> current_;
        public:
            iterator(iterator_t<Collection> curr) : current_(curr) {}
            auto operator*() const { return current_->first; }
            iterator& operator++() { ++current_; return *this; }
            bool operator!=(const iterator& other) const { return current_ != other.current_; }
        };

        iterator begin() { return { std::begin(collection_) }; }
        iterator end() { return { std::end(collection_) }; }
    };

    inline auto get_keys = [](auto&& col) {
        using C = std::remove_cvref_t<decltype(col)>;
        return keys_view<C>(std::forward<decltype(col)>(col));
        };

    template<typename Collection>
    class values_view {
        Collection collection_;
    public:
        values_view(Collection col) : collection_(std::move(col)) {}

        class iterator {
            iterator_t<Collection> current_;
        public:
            iterator(iterator_t<Collection> curr) : current_(curr) {}
            auto operator*() const { return current_->second; }
            iterator& operator++() { ++current_; return *this; }
            bool operator!=(const iterator& other) const { return current_ != other.current_; }
        };

        iterator begin() { return { std::begin(collection_) }; }
        iterator end() { return { std::end(collection_) }; }
    };

    inline auto get_values = [](auto&& col) {
        using C = std::remove_cvref_t<decltype(col)>;
        return values_view<C>(std::forward<decltype(col)>(col));
        };

    template<typename Collection, typename Adapter>
    auto operator|(Collection&& col, Adapter adapter) {
        return adapter(std::forward<Collection>(col));
    }
}