#include <iostream>
#include <string>
#include <variant>

#include <msgpack.hpp>


namespace msgpack::adaptor
{
    template<typename... Ts>
    struct pack<std::variant<Ts...>>
    {
        template<typename Stream>
        msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, std::variant<Ts...> const& v) const
        {
            std::visit([&](auto const& val) { o.pack(val); }, v);
            return o;
        }
    };
}


int main()
{
    std::variant<int, double, bool, std::string> var = 3.14;

    msgpack::sbuffer buf;
    msgpack::pack(buf, var);
    std::cout << buf.size() << '\n';

    auto h = msgpack::unpack(buf.data(), buf.size());
    std::cout << h->as<double>() << '\n';
}
