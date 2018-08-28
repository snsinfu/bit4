#include <memory>

class fooer
{
protected: // DON'T: destructor needs to be public
    virtual ~fooer() = default;

public:
    virtual void foo() = 0;
};

class noop_fooer : public fooer
{
public:
    void foo() override
    {
    }
};

void run(fooer& f)
{
    f.foo();
}

int main()
{
    // error: calling a protected destructor of class 'fooer'
    std::unique_ptr<fooer> f = std::make_unique<noop_fooer>();
    run(*f);
}
