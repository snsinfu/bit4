#include <memory>
#include <string>
#include <unordered_map>


class attribute_base
{
  public:
    virtual ~attribute_base() = default;
};

template<typename T>
class attribute : public attribute_base
{
  public:
    void set(T value)
    {
        value_ = value;
    }

  private:
    T value_;
};

class attributes_store
{
  public:
    template<typename T>
    void create(std::string name)
    {
        attributes_[name] = std::make_unique<attribute<T>>();
    }

    template<typename T>
    attribute<T>& get(std::string name)
    {
        return dynamic_cast<attribute<T>&>(*attributes_.at(name));
    }

  private:
    std::unordered_map<std::string, std::unique_ptr<attribute_base>> attributes_;
};

int main()
{
    attributes_store store;

    store.create<double>("charge");
    store.create<double>("mass");

    attribute<double>& charge = store.get<double>("charge");
    attribute<double>& mass = store.get<double>("mass");

    charge.set(1.602176634e-19);
    mass.set(9.10938356e-31);
}
