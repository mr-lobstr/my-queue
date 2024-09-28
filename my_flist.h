#ifndef MY_FORWARD_LIST_GUARD_H
#define MY_FORWARD_LIST_GUARD_H

#include <utility>

namespace data_struct
{
    struct FListHead {
        FListHead* next = nullptr;
    };


    template <typename T>
    class FList {
        using Head = FListHead;

    public:
        FList() noexcept = default;

        FList (FList&& rhs) noexcept
            : prevFirst (std::exchange (rhs.prevFirst, Head{}))
        {}
    
        FList (FList const& rhs) {
            copy_and_return_ptr_last (rhs);
        }

        FList& operator= (FList&& rhs) noexcept {
            if (this != &rhs) {
                auto tmp = std::move (rhs);
                swap (tmp);
            }
        
            return *this;
        }

        FList& operator= (FList const& rhs) {
            if (this != &rhs) {
                auto tmp = rhs;
                swap (tmp);
            }
        
            return *this;
        }

        ~FList() noexcept {
            for_each_head (
                [] (auto* pHead) {
                    delete (get_ptr_node (pHead));
                }
            );
        }

        bool empty() const noexcept {
            return prevFirst.next == nullptr;
        }

        void swap (FList& rhs) noexcept {
            std::swap(*this, rhs);
        }

        void insert_after (Head* pos, T const& value) {
            auto oldNext = pos->next;
            pos->next = new Node {oldNext, value};
        }

        void push_front (T const& value) {
            insert_after (&prevFirst, value);
        }

        void pop_front() noexcept {
            auto oldFirst = get_ptr_node (begin());
            prevFirst.next = oldFirst->next;
            delete (oldFirst);
        }

        Head* begin() noexcept {
            return prevFirst.next;
        }

        Head const* begin() const noexcept {
            return prevFirst.next;
        }

        Head* end() noexcept {
            return nullptr;
        }

        Head const* end() const noexcept {
            return nullptr;
        }

        T& front() noexcept {
            return get_value (begin());
        }

        T const& front() const noexcept {
            return get_value (begin());
        }

        static
        T& get_value (Head* pHead) noexcept {
            return get_ptr_node (pHead)->value;
        }

        static
        T const& get_value (Head const* pHead) noexcept {
            return get_ptr_node (pHead)->value;
        }

        template <typename Action>
        void read_by_range (Action read) const {
            for_each_head (
                [&] (Head const* ptr) {
                    read (get_value (ptr));
                }
            );
        }

    protected:
        template <typename Action>
        void for_each_head (Action action) const {
            auto beg = prevFirst.next;

            while (beg != nullptr) {
                auto pNext = beg->next;
                action(beg);
                beg = pNext;
            }
        }

        static
        auto* get_ptr_node (Head* pHead) noexcept {
            return static_cast<Node*> (pHead);
        }

        static
        auto* get_ptr_node (Head const* pHead) noexcept {
            return static_cast<Node const*> (pHead);
        }

        Head* copy_and_return_ptr_last (FList const& orig) {
            auto ptrLast = &prevFirst;

            orig.read_by_range (
                [&] (auto& value) {
                    insert_after (ptrLast, value);
                    ptrLast = ptrLast->next;
                }
            );

            return ptrLast;
        }

    protected:
        struct Node: public Head {
            T value;
        };

        Head prevFirst{};
    };


    template <typename T>
    void swap (FList<T>& lhs, FList<T>& rhs) noexcept {
        lhs.swap (rhs);
    }
}
#endif