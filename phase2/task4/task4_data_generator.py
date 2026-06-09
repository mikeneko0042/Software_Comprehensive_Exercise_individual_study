import random
import string
import sys

def generate_random_string(min_len=1, max_len=10):
    length = random.randint(min_len, max_len)
    return ''.join(random.choices(string.ascii_lowercase, k=length))

def generate_test_data(N, M_max, Q, num_unique_products):
    #全顧客で共有する商品
    product_pool = list({generate_random_string() for _ in range(num_unique_products)})
    
    print(N)
    for _ in range(N):
        M = random.randint(1, M_max)
        purchased = random.sample(product_pool, M)
        print(f"{M} " + " ".join(purchased))
    
    print(Q)
    max_pairs = (N * M_max * (M_max - 1)) // 2
    for _ in range(Q):
        a = random.randint(1, min(max_pairs, 100))
        b = random.randint(a, a + 10)
        print(f"{a} {b}")

if __name__ == "__main__":
    generate_test_data(N=10000, M_max=20, Q=10, num_unique_products=5000)