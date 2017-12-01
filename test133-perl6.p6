# https://perl6advent.wordpress.com/

multi sub identify(Int $num) {
    say "(Int $num)";
}

multi sub identify(Str $str) {
    say "(Str $str)";
}

multi sub identify(@arr) {
    for @arr -> $val {
        identify($val);
    }
}

my @values = 1..10;
my $sum = [+] @values;

identify($sum);
identify(@values);
