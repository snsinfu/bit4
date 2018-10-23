// Recursive template argument?
// -> Does not work.

template<void(&func)(unique<func>)>
struct unique
{
};

void fun(unique<fun>)
{
}

int main()
{
}
