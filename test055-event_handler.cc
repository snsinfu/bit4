#include <functional>
#include <memory>
#include <unordered_set>
#include <utility>

#include <cstddef>

namespace
{
    template<typename T>
    std::size_t compute_hash(T const& object)
    {
        std::hash<T> hash;
        return hash(object);
    }

    template<typename T>
    std::shared_ptr<T> wrap_shared(T object)
    {
        return std::make_shared<T>(std::move(object));
    }

    template<typename... Args>
    struct Subscription
    {
        explicit
        Subscription(std::function<void(Args...)> callback)
            : callback_{wrap_shared(std::move(callback))}
        {
        }

        void call(Args... args) const
        {
            (*callback_)(args...);
        }

        std::size_t hash() const
        {
            return compute_hash(callback_);
        }

        bool operator==(Subscription const& other) const
        {
            return callback_ == other.callback_;
        }

        bool operator!=(Subscription const& other) const
        {
            return !(*this == other);
        }

      private:
        std::shared_ptr<std::function<void(Args...)>> callback_;
    };

    template<typename... Args>
    struct Emitter
    {
        using Subscription = ::Subscription<Args...>;

        Subscription subscribe(std::function<void(Args...)> callback)
        {
            Subscription subsc{std::move(callback)};
            subscriptions_.insert(subsc);
            return subsc;
        }

        void unsubscribe(Subscription const& subsc)
        {
            subscriptions_.erase(subsc);
        }

        void emit(Args... args) const
        {
            for (Subscription const& subsc : subscriptions_) {
                subsc.call(args...);
            }
        }

      private:
        std::unordered_set<Subscription> subscriptions_;
    };
}

template<typename... Args>
struct std::hash<Subscription<Args...>>
{
    std::size_t operator()(Subscription<Args...> const& subsc) const
    {
        return subsc.hash();
    }
};

//------------------------------------------------------------------------------

#include <iostream>

int main()
{
    Emitter<int> emitter;

    Subscription<int> subsc = emitter.subscribe([](int x) {
        std::cout << "event " << x << '\n';
    });

    emitter.emit(1);
    emitter.emit(2);
    emitter.unsubscribe(subsc);
    emitter.emit(3);
}
