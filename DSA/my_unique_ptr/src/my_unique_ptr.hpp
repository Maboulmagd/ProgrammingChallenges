#pragma once

#include <memory>

namespace DSA
{
    template <typename T, typename Deleter = std::default_delete<T>>
    class unique_ptr final
    {
        public:
            constexpr unique_ptr() : rawPtr_(nullptr)
            {  
            }

            constexpr explicit unique_ptr(T* rawPtr)
             : rawPtr_(rawPtr)
            {
            }

            constexpr explicit unique_ptr(T* rawPtr, Deleter deleter)
             : rawPtr_(rawPtr), deleter_(deleter)
            {
            } 

            constexpr unique_ptr(const unique_ptr& other) = delete;
            constexpr unique_ptr& operator=(const unique_ptr& other) = delete;

            ~unique_ptr()
            {
                if (this->rawPtr_ != nullptr)
                {
                    this->deleter_(this->rawPtr_);// Invoke deleter explicitly;
                }
            }

            constexpr T* release() noexcept
            {
                //assert(rawPtr_ != nullptr);
                this->rawPtr_ = nullptr;
            }

            constexpr void reset(T* newRawPtr) noexcept
            {
                //assert(newRawPtr != nullptr);
                if (this->rawPtr_ != nullptr)
                {
                    this->deleter_(this->rawPtr_);// Invoke deleter explicitly
                    this->rawPtr_ = nullptr;
                }

                this->rawPtr_ = newRawPtr;
            }

            constexpr void swap(unique_ptr& other) noexcept
            {
                T* otherRawPtr = other.get();
                //assert(otherRawPtr != nullptr);
                Deleter otherDeleter = other.get_deleter();

                other.rawPtr_ = this->rawPtr_;
                other.deleter_ = this->deleter_;

                this->rawPtr_ = otherRawPtr;
                this->deleter_ = otherDeleter;
            }

            constexpr T* get() const noexcept
            {
                return this->rawPtr_;
            }

            constexpr Deleter& get_deleter() const noexcept
            {
                return this->deleter_;
            }

            explicit constexpr operator bool() const noexcept
            {
                return this->get() != nullptr;
            }

            constexpr typename std::add_lvalue_reference<T>::type operator*() const 
            noexcept(noexcept(*std::declval<T*>()))
            {
                return *this->get();
            }

            constexpr T* operator->() const noexcept
            {
                return this->get();
            }

            constexpr bool operator==(const unique_ptr& other) const noexcept
            {
                return this->get() == other.get();
            }

            constexpr bool operator!=(const unique_ptr& other) const noexcept
            {
                return this->get() != other.get(); 
            }

        private:
            T* rawPtr_;
            [[no_unique_address]] Deleter deleter_;//Empty base optimization
    };

    // May throw bad alloc
    template <typename T, typename... Args>
    constexpr unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}