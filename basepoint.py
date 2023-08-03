import random

def derive_private_key(public_key):
    base_point = (0x03, 0x63, 0x3c, 0xbe, 0x3e, 0xc0, 0x2b, 0x94, 0x01, 0xc5, 0xef, 0xfa, 0x14, 0x4c, 0x5b, 0x4d, 0x22, 0xf8, 0x79, 0x40, 0x25, 0x96, 0x34, 0x85, 0x8f, 0xc7, 0xe5, 0x9b, 0x1c, 0x09, 0x93, 0x78, 0x52)
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
