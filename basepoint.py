import random

def derive_private_key(public_key):
    base_point = (0x02, 0x79, 0xBE, 0x66, 0x7E, 0xF4, 0x8A, 0x03, 0xBB, 0xAC, 0x99, 0x9B, 0x5A, 0xCE, 0x6E, 0xEF, 0x48, 0x56, 0x1A, 0x14, 0x9E, 0x38, 0x77, 0x63, 0x74, 0x8F, 0xAE, 0x6A, 0x7E, 0x84, 0xF3, 0x42)
    public_key = tuple(int(x, 16) for x in public_key.split())  # Convert string to tuple of integers
    private_key = 0
    while public_key != base_point:
        public_key = tuple((x - y) % (2**256) for x, y in zip(public_key, base_point))  # Perform element-wise subtraction
        private_key += 1
    return private_key

if __name__ == "__main__":
    public_key = input("Enter public key: ")
    private_key = derive_private_key(public_key)
    print("Public key:", public_key)
    print("Private key:", private_key)
