import random


def generate_worst_case():
    W, H, N, Q = 100, 100, 20, 20

    # Header: W H N
    print(f"{W} {H} {N}")

    products = []
    # Place products such that their pickup location is valid
    # Entry: (1, 0), Exit: (W-2, 0)
    # Walls at y=0 from x=2 to W-3
    for i in range(N):
        name = f"item{i}"
        products.append(name)
        # x=2, y=i+5. Direction 'E' means pickup at (3, i+5)
        # This is safely away from the bottom wall and boundaries
        x, y = 2, i + 5
        direction = "E"
        print(f"{x} {y} {name} {direction}")

    # Q: Number of queries
    print(Q)

    # Generate Q queries, each with M=N products
    for _ in range(Q):
        # All N products in each query
        # Shuffling to make it slightly different for DP (though complexity is same)
        shuffled_products = list(products)
        random.shuffle(shuffled_products)
        print(f"{N} {' '.join(shuffled_products)}")


if __name__ == "__main__":
    import sys

    sys.stdout = open("input_task5_worst.txt", "w")
    generate_worst_case()