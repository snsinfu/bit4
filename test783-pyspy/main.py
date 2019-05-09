def main():
    print(is_prime(219174609154151789))


def is_prime(n):
    if n in [2, 3]:
        return True
    if n % 6 not in [1, 5]:
        return False
    for k in range(6, int(n ** 0.5) + 1, 6):
        a = k - 1
        b = k + 1
        if n % a == 0 or n % b == 0:
            return False
    return True


if __name__ == "__main__":
    main()
