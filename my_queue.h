#ifndef MY_QUEUE_H_GUARD
#define MY_QUEUE_H_GUARD

#include "my_flist.h"

namespace data_struct
{
    template <typename T>
    class Queue: private FList<T> {
        using Base = FList<T>;
        using Head = FListHead;

        using Base::copy_and_return_ptr_last;
        using Base::get_value;
        using Base::insert_after;

    public:
        using Base::front;
        using Base::empty;
        using Base::read_by_range;

    public:
        Queue() noexcept = default;
        ~Queue() noexcept = default;

        Queue (Queue&& rhs) noexcept
            : Base (std::move (rhs))
            , size_ (std::exchange (rhs.size_, 0))
        {
            if (not rhs.empty()) {
                ptrLast = std::exchange (rhs.ptrLast, rhs.ptr_prev());
            }
        }

        Queue (Queue const& rhs)
            : Base ()
            , size_ (rhs.size_)
            , ptrLast (copy_and_return_ptr_last (rhs))
        {}

        Queue& operator= (Queue&& rhs) noexcept {
            if (this != &rhs) {
                auto tmp = std::move (rhs);
                swap (tmp);
            }
        
            return *this;
        }

        Queue& operator= (Queue const& rhs) {
            if (this != &rhs) {
                auto tmp = rhs;
                swap (tmp);
            }
        
            return *this;
        }

        std::size_t size() const noexcept {
            return size_;
        }

        void swap (Queue& rhs) noexcept {
            std::swap (*this, rhs);
        }

        void push_back (T const& value) {
            insert_after (ptrLast, value);
            ptrLast = ptrLast->next;
            ++size_;
        }

        void pop_front() noexcept {
            Base::pop_front();
            --size_;
        }

        T& back() noexcept {
            return get_value (ptrLast);
        }

        T const& back() const noexcept {
            return get_value (ptrLast);
        }

    private:
        using Base::prevFirst;

        Head* ptr_prev() const noexcept {
            return const_cast<Head*> (&prevFirst);
        }

    private:
        std::size_t size_ = 0;
        Head* ptrLast = ptr_prev();
    };


    template <typename T>
    void swap (Queue<T>& lhs, Queue<T>& rhs) noexcept {
        lhs.swap (rhs);
    }
}
#endif