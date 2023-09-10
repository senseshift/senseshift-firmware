#pragma once

#include <algorithm>
#include <stddef.h>
#include <string.h>

namespace SenseShift {
    template<typename _Tp>
    struct IBuffer {
        using ValueType = _Tp;

        IBuffer() = default;
        virtual ~IBuffer() = default;

        virtual bool push(const ValueType value) = 0;
        virtual bool push(const ValueType* values, size_t length) = 0;
        virtual void clear() = 0;
        virtual ValueType pop() = 0;
        virtual ValueType get(size_t index) const = 0;
        virtual size_t getLength() const = 0;

        virtual ValueType operator[](size_t index) const { return this->get(index); }
    };

    template<typename _Tp, size_t N>
    class RingBuffer : public IBuffer<_Tp> {
      public:
        using ValueType = _Tp;

        bool push(const ValueType value) override
        {
            this->mRingBuffer[this->mNewestIndex] = value;
            this->mNewestIndex = (this->mNewestIndex + 1) % N;
            this->mLength = std::min(this->mLength + 1, N);

            return true;
        }

        bool push(const ValueType* values, size_t length) override
        {
            if (this->mLength + length > N) {
                return false;
            }

            for (size_t i = 0; i < length; i++) {
                this->push(values[i]);
            }

            return true;
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
         * @example `get(0)` is the oldest value, `get(this.getLength() - 1)` is the newest value
         */
        ValueType get(size_t index) const override
        {
            if (index < 0 || index >= this->mLength) {
                return -1;
            }
            return this->mRingBuffer[(N + this->mNewestIndex - this->mLength + index) % N];
        }

        size_t getLength() const override { return this->mLength; }

      private:
        ValueType mRingBuffer[N];
        size_t mNewestIndex = 0;
        size_t mLength = 0;
    };

    template<typename _Tp, size_t N>
    class FixedSizeBuffer : public IBuffer<_Tp> {
      public:
        using ValueType = _Tp;

        FixedSizeBuffer() = default;
        virtual ~FixedSizeBuffer() = default;

        bool push(const ValueType value) override
        {
            if (this->mLength == N) {
                return false;
            }
            this->mBuffer[this->mLength] = value;
            this->mLength += 1;

            return true;
        }

        bool push(const ValueType* values, size_t length) override
        {
            if (this->mLength + length > N) {
                return false;
            }

            memcpy(this->mBuffer.data() + this->mLength, values, length);
            this->mLength += length;

            return true;
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
         * @example `get(0)` is the oldest value, `get(this.getLength() - 1)` is the newest value
         */
        ValueType get(size_t index) const override
        {
            if (index < 0 || index >= this->mLength) {
                return -1;
            }
            return this->mBuffer[index];
        }

        ValueType* getData() { return this->mBuffer.data(); }

        size_t getLength() const override { return this->mLength; }

      private:
        std::array<ValueType, N> mBuffer;
        size_t mLength = 0;
    };
} // namespace SenseShift
