#ifndef INCLUDED_GZBUFFER_HPP
#define INCLUDED_GZBUFFER_HPP

#include <cstddef>
#include <cstdlib>
#include <memory>

#include <zlib.h>


namespace schema
{
    class gzbuffer
    {
    public:
        static constexpr std::size_t default_init_size = 8192;
        static constexpr std::size_t reservation_size = 512;

        gzbuffer(
            int level = Z_DEFAULT_COMPRESSION,
            std::size_t init_size = default_init_size
        )
            : stream_{}, data_{nullptr}, init_size_{init_size}
        {
            int const method = Z_DEFLATED;
            int const win_bits = 31; // gzip with default window
            int const mem_level = 8; // default
            int const strategy = Z_DEFAULT_STRATEGY;

            if (deflateInit2(&stream_, level, method, win_bits, mem_level, strategy) != Z_OK) {
                throw std::bad_alloc();
            }
        }

        gzbuffer(gzbuffer const&) = delete;
        gzbuffer& operator=(gzbuffer const&) = delete;

        ~gzbuffer()
        {
            deflateEnd(&stream_);
            std::free(data_);
        }

        void write(char const* buf, std::size_t len)
        {
            stream_.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(buf));
            stream_.avail_in = static_cast<unsigned>(len);

            while (stream_.avail_in > 0) {
                if (stream_.avail_out < reservation_size) {
                    if (!expand()) {
                        throw std::bad_alloc();
                    }
                }

                if (deflate(&stream_, Z_NO_FLUSH) != Z_OK) {
                    throw std::bad_alloc();
                }
            }
        }

        char* flush()
        {
            for (;;) {
                switch (deflate(&stream_, Z_FINISH)) {
                case Z_STREAM_END:
                    return data_;

                case Z_OK:
                    if(!expand()) {
                        throw std::bad_alloc();
                    }
                    break;

                default:
                    throw std::bad_alloc();
                }
            }
        }

        char* data()
        {
            return data_;
        }

        const char* data() const
        {
            return data_;
        }

        std::size_t size() const
        {
            return static_cast<std::size_t>(reinterpret_cast<char*>(stream_.next_out) - data_);
        }

        void reset()
        {
            if (deflateReset(&stream_) != Z_OK) {
                throw std::bad_alloc();
            }
            reset_buffer();
        }

        void reset_buffer()
        {
            stream_.avail_out += reinterpret_cast<char*>(stream_.next_out) - data_;
            stream_.next_out = reinterpret_cast<Bytef*>(data_);
        }

    private:
        bool expand()
        {
            std::size_t used = static_cast<std::size_t>(
                reinterpret_cast<char*>(stream_.next_out) - data_
            );
            std::size_t csize = used + stream_.avail_out;
            std::size_t nsize = (csize == 0) ? init_size_ : csize * 2;

            char* tmp = static_cast<char*>(std::realloc(data_, nsize));
            if (tmp == nullptr) {
                return false;
            }

            data_ = tmp;
            stream_.next_out  = reinterpret_cast<Bytef*>(tmp + used);
            stream_.avail_out = static_cast<unsigned>(nsize - used);

            return true;
        }

    private:
        z_stream stream_;
        char* data_;
        size_t init_size_;
    };
}

#endif
