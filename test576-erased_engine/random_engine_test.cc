#include <random>
#include <type_traits>

#include "catch.hpp"
#include "random_engine.hpp"


TEST_CASE("random_engine - generates uniform real numbers")
{
    cxx::random_engine engine{std::default_random_engine{}};

    float xs[] = {-1, -1};
    engine.uniform(xs, 2);
    CHECK(xs[0] >= 0);
    CHECK(xs[1] >= 0);
    CHECK(xs[0] < 1);
    CHECK(xs[1] < 1);

    double ys[] = {-1, -1};
    engine.uniform(ys, 2);
    CHECK(ys[0] >= 0);
    CHECK(ys[1] >= 0);
    CHECK(ys[0] < 1);
    CHECK(ys[1] < 1);
}

TEST_CASE("random_engine - generates normal real numbers")
{
    cxx::random_engine engine{std::default_random_engine{}};

    float xs[3] = {0, 0, 0};
    engine.normal(xs, 3);
    CHECK(xs[0] != xs[1]);
    CHECK(xs[1] != xs[2]);
    CHECK(xs[2] != xs[0]);

    double ys[3] = {0, 0, 0};
    engine.normal(ys, 3);
    CHECK(ys[0] != ys[1]);
    CHECK(ys[1] != ys[2]);
    CHECK(ys[2] != ys[0]);
}

TEST_CASE("random_engine - does not alias passed-in engine")
{
    constexpr std::minstd_rand::result_type first_number = 48271;
    constexpr std::minstd_rand::result_type second_number = 182605794;

    std::minstd_rand source;
    CHECK(source() == first_number);

    // random_engine owns a copy of source, not a reference to source
    cxx::random_engine engine{source};
    double buf[10];
    engine.uniform(buf, 10);

    // So source's state must not be changed by engine
    CHECK(source() == second_number);
}

TEST_CASE("random_engine - is copy constructible (checkpoint)")
{
    cxx::random_engine engine{std::default_random_engine{}};
    cxx::random_engine copy{engine};

    double first[3] = {-1, -1, -1};
    engine.uniform(first, 3);

    double second[3] = {-1, -1, -1};
    copy.uniform(second, 3);

    CHECK(first[0] == second[0]);
    CHECK(first[1] == second[1]);
    CHECK(first[2] == second[2]);
}

TEST_CASE("random_engine - is copy assignable (rollback)")
{
    cxx::random_engine engine{std::default_random_engine{}};
    cxx::random_engine checkpoint{engine};

    double first[3] = {-1, -1, -1};
    engine.uniform(first, 3);

    // Roll back to the checkpoint
    engine = checkpoint;

    double second[3] = {-1, -1, -1};
    engine.uniform(second, 3);

    CHECK(first[0] == second[0]);
    CHECK(first[1] == second[1]);
    CHECK(first[2] == second[2]);
}

TEST_CASE("random_engine - is nothrow move constructible")
{
    CHECK(std::is_nothrow_move_constructible<cxx::random_engine>::value);

    cxx::random_engine reference_engine = std::default_random_engine{};
    double reference_sequence[2];
    reference_engine.uniform(reference_sequence, 2);

    // Original engine generates the first number
    cxx::random_engine engine = std::default_random_engine{};
    double first;
    engine.uniform(&first, 1);

    // State is transfered to an other engine, which generates the second number
    cxx::random_engine move_to = std::move(engine);
    double second;
    move_to.uniform(&second, 1);

    CHECK(first == reference_sequence[0]);
    CHECK(second == reference_sequence[1]);
}

TEST_CASE("random_engine - is nothrow move assignable")
{
    CHECK(std::is_nothrow_move_assignable<cxx::random_engine>::value);

    cxx::random_engine reference_engine = std::default_random_engine{};
    double reference_sequence[2];
    reference_engine.uniform(reference_sequence, 2);

    // Original engine generates the first number
    cxx::random_engine engine = std::default_random_engine{};
    double first;
    engine.uniform(&first, 1);

    // State is transfered to an other engine, which generates the second number
    cxx::random_engine move_to = std::default_random_engine{};
    move_to = std::move(engine);
    double second;
    move_to.uniform(&second, 1);

    CHECK(first == reference_sequence[0]);
    CHECK(second == reference_sequence[1]);
}

TEST_CASE("random_engine - is swappable")
{
    cxx::random_engine engine_a = std::minstd_rand{1234};
    cxx::random_engine engine_b = std::minstd_rand{5678};

    cxx::random_engine engine_a_copy = engine_a;
    cxx::random_engine engine_b_copy = engine_b;

    engine_a.swap(engine_b);
    // Now engine_a == engine_b_copy and engine_b == engine_a_copy

    double a_value;
    double b_copy_value;
    engine_a.uniform(&a_value, 1);
    engine_b_copy.uniform(&b_copy_value, 1);
    CHECK(a_value == b_copy_value);

    double b_value;
    double a_copy_value;
    engine_b.uniform(&b_value, 1);
    engine_a_copy.uniform(&a_copy_value, 1);
    CHECK(b_value == a_copy_value);
}
