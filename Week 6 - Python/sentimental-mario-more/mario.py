from cs50 import get_int


def main():

    n = height()
    full_pyramid(n)


def height():
    """Ask pyramid height until it matchs requirements"""
    while True:
        n = get_int("How tall the pyramid should be ? ")
        if n >= 1 and n <= 8:
            break
    return n


def full_pyramid(n):
    """FULL PYRAMID printing"""
    for i in range(1, n + 1):
        # Left pyramid side
        for j in range(n - i):
            # Print space of left pyramid
            print(" ", end='')
        for j in range(i):
            # Print bricks of left side pyramid
            print("#", end='')
        # Print space between pyramids
        print("  ", end='')
        # Right pyramid side
        for j in range(i):
            # Print bricks of right side pyramid
            print("#", end='')
        # Move to next line
        print("")


if __name__ == "__main__":
    main()
