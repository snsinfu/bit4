fun main(args: Array<String>) {
    val y = apply(3) { x -> x * x }
    println("${y}")
}

fun <A, R> apply(arg: A, func: (A) -> R) : R = func(arg)
