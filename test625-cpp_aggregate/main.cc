// Aggregate initialization of a struct with default initializer(s) requires
// C++14.

struct parameters_a
{
    double foo;
    double bar;
    double baz;
};

struct parameters_b
{
    double foo = 1;
    double bar = 2;
    double baz = 3;
};

int main()
{
    parameters_a pa = {
        4, 5, 6
    };
    (void) pa;

    parameters_b pb = { // error: matching constructor for initialization of 'parameters_b'
        4, 5, 6
    };
    (void) pb;

    parameters_b pb2 = { // error: matching constructor for initialization of 'parameters_b'
        .foo = 4,
        .bar = 5,
        .baz = 6
    };
    (void) pb2;
}
