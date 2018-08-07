#include <cstdint>
#include <cstdio>
#include <cstring>

#include <wmmintrin.h>


struct vec
{
    std::uint32_t x {};
    std::uint32_t y {};
    std::uint32_t z {};

    std::uint32_t hash() const
    {
        static char const key[176 + 1] =
            "\x85\x07\x22\xe2\x58\xfa\x15\x86\x65\x61\x3c\xf4\x24\xed\x68\xe4"
            "\xd1\x42\x4b\xd4\x89\xb8\x5e\x52\xec\xd9\x62\xa6\xc8\x34\x0a\x42"
            "\xcb\x25\x67\x3c\x42\x9d\x39\x6e\xae\x44\x5b\xc8\x66\x70\x51\x8a"
            "\x9e\xf4\x19\x0f\xdc\x69\x20\x61\x72\x2d\x7b\xa9\x14\x5d\x2a\x23"
            "\xda\x11\x3f\xf5\x06\x78\x1f\x94\x74\x55\x64\x3d\x60\x08\x4e\x1e"
            "\xfa\x3e\x4d\x25\xfc\x46\x52\xb1\x88\x13\x36\x8c\xe8\x1b\x78\x92"
            "\x75\x82\x02\xbe\x89\xc4\x50\x0f\x01\xd7\x66\x83\xe9\xcc\x1e\x11"
            "\x7e\xf0\x80\xa0\xf7\x34\xd0\xaf\xf6\xe3\xb6\x2c\x1f\x2f\xa8\x3d"
            "\xeb\x32\xa7\x60\x1c\x06\x77\xcf\xea\xe5\xc1\xe3\xf5\xca\x69\xde"
            "\x84\xcb\xba\x86\x98\xcd\xcd\x49\x72\x28\x0c\xaa\x87\xe2\x65\x74"
            "\x2a\x86\x28\x91\xb2\x4b\xe5\xd8\xc0\x63\xe9\x72\x47\x81\x8c\x06";

        __m128i kb = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key));
        __m128i k1 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 16));
        __m128i k2 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 32));
        //__m128i k3 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 48));
        //__m128i k4 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 64));
        //__m128i k5 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 80));
        //__m128i k6 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 96));
        //__m128i k7 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 112));
        //__m128i k8 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 128));
        //__m128i k9 = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 144));
        __m128i ke = _mm_loadu_si128(reinterpret_cast<__m128i const*>(key + 160));

        std::uint32_t const plaintext[4] = {
            x, y, z, 0
        };

        __m128i m = _mm_loadu_si128(reinterpret_cast<__m128i const*>(plaintext));
        m = _mm_xor_si128(m, kb);
        m = _mm_aesenc_si128(m, k1);
        m = _mm_aesenc_si128(m, k2);
        //m = _mm_aesenc_si128(m, k3);
        //m = _mm_aesenc_si128(m, k4);
        //m = _mm_aesenc_si128(m, k5);
        //m = _mm_aesenc_si128(m, k6);
        //m = _mm_aesenc_si128(m, k7);
        //m = _mm_aesenc_si128(m, k8);
        //m = _mm_aesenc_si128(m, k9);
        m = _mm_aesenclast_si128(m, ke);

        std::uint32_t ciphertext[4];
         _mm_storeu_si128(reinterpret_cast<__m128i*>(ciphertext), m);
         return ciphertext[0];
    }
};

int main()
{
    vec v;

    for (std::uint32_t x = 0; x < 10; x++) {
        for (std::uint32_t y = 0; y < 10; y++) {
            for (std::uint32_t z = 0; z < 10; z++) {
                v.x = x;
                v.y = y;
                v.z = z;

                std::printf("%u %u %u %d\n", x, y, z, v.hash() % 2000);
            }
        }
    }
}
