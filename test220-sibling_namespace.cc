namespace snsinfu
{
    namespace foo
    {
        using int_type = int;
    }

    namespace bar
    {
        foo::int_type baz()
        {
            return 0;
        }
    }
}

int main()
{
    snsinfu::bar::baz();
}
