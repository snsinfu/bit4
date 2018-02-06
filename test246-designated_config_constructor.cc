// Let us use C++20 degignated initializers with the Config object idiom often
// seen in golang projects. To emulate named parameters!

#include <string>

namespace
{
    class database_client
    {
      public:
        struct config
        {
            std::string host     = "localhost";
            int         port     = 1111;
            std::string user     = "root";
            std::string password = "";
        };

        explicit database_client(config const& config)
        {
            static_cast<void>(config);
        }

        void query(std::string query) const
        {
            static_cast<void>(query);
        }
    };
}

int main()
{
    // The double bracket is ugly but it looks super clear!
    auto client = database_client{{
        .user     = "alice",
        .password = "correct horse battery staple",
    }};

    client.query("");

    // It might be clearer to explicitly specify the config type name.
    auto another_client = database_client{database_client::config{
        .user     = "bob",
        .password = "Tr0ub4dor&3",
    }};

    another_client.query("");

    // C++ <= 17 users still can do this.
    database_client::config config;
    config.host = "db.example.com";
    config.user = "charlie";
    auto yet_another_client = database_client{config};

    yet_another_client.query("");
}
