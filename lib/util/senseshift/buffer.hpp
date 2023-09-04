#pragma once

#include <algorithm>
#include <stddef.h>

namespace SenseShift {
    template<typename _Tp>
    struct IBuffer {
        using ValueType = _Tp;

        virtual void add(ValueType value) = 0;
        virtual void clear() = 0;
        virtual ValueType pop() = 0;
        virtual ValueType get(size_t index) = 0;
        virtual size_t getLength() = 0;
    };

    template<typename _Tp, size_t N>
    class RingBuffer : public IBuffer<_Tp> {
      public:
        using ValueType = _Tp;

        void add(ValueType value) override
        {
            this->mRingBuffer[this->mNewestIndex] = value;
            this->mNewestIndex = (this->mNewestIndex + 1) % N;
            this->mLength = std::min(this->mLength + 1, N);
        }

        void clear() override
        {
            this->mNewestIndex = 0;
            this->mLength = 0;
        }

        /**
         * @brief Remove the oldest value from the buffer and return it.
         * If the buffer is empty, `-1` is returned.
         */
        ValueType pop() override
        {
            if (this->mLength == 0) {
                return -1;
            }
            ValueType result = this->mRingBuffer[(N + this->mNewestIndex - this->mLength) % N];
            this->mLength -= 1;
            return result;
        }

        /**
         * @brief Get the value at the given index.
         *
         * `get(0)` is the oldest value, `get(this.getLength() - 1)` is the newest value
         */
        ValueType get(size_t index) override
        {
            if (index < 0 || index >= this->mLength) {
                return -1;
            }
            return this->mRingBuffer[(N + this->mNewestIndex - this->mLength + index) % N];
        }

        size_t getLength() override { return this->mLength; }

      private:
        ValueType mRingBuffer[N];
        size_t mNewestIndex = 0;
        size_t mLength = 0;
    };

    template<typename _Tp, size_t N>
    class FlatBuffer : public IBuffer<_Tp> {
      public:
        using ValueType = _Tp;

        void add(ValueType value) override
        {
            if (this->mLength == N) {
                return;
            }
            this->mBuffer[this->mLength] = value;
            this->mLength += 1;
        }

        void clear() override { this->mLength = 0; }

        /**
         * @brief Remove the oldest value from the buffer and return it.
         * If the buffer is empty, `-1` is returned.
         */
        ValueType pop() override
        {
            if (this->mLength == 0) {
                return -1;
            }
            ValueType result = this->mBuffer[0];
            for (size_t i = 1; i < this->mLength; i++) {
                this->mBuffer[i - 1] = this->mBuffer[i];
            }
            this->mLength -= 1;
            return result;
        }

        /**
         * @brief Get the value at the given index.
         *
         * `get(0)` is the oldest value, `get(this.getLength() - 1)` is the newest value
         */
        ValueType get(size_t index) override
        {
            if (index < 0 || index >= this->mLength) {
                return -1;
            }
            return this->mBuffer[index];
        }

        size_t getLength() override { return this->mLength; }

      private:
        ValueType mBuffer[N];
        size_t mLength = 0;
    };
} // namespace SenseShift
