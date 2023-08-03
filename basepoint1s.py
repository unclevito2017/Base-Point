import random

def derive_private_key(public_key):
    base_point = (0x03, 0x63, 0x3c, 0xbe, 0x3e, 0xc0, 0x2b, 0x94, 0x01, 0xc5, 0xef, 0xfa, 0x14, 0x4c, 0x5b, 0x4d, 0x22, 0xf8, 0x79, 0x40, 0x25, 0x96, 0x34, 0x85, 0x8f, 0xc7, 0xe5, 0x9b, 0x1c, 0x09, 0x93, 0x78, 0x52)
    
    if public_key.startswith("04"):  # Uncompressed public key
        public_key = tuple(int(public_key[i:i+2], 16) for i in range(2, len(public_key), 2))
    elif public_key.startswith(("02", "03")):  # Compressed public key
        prefix = public_key[:2]
        x_coord = int(public_key[2:], 16)
        
        if prefix == "02":
            y_coord = calculate_y_coord(x_coord, is_even=True)
        elif prefix == "03":
            y_coord = calculate_y_coord(x_coord, is_even=False)
            
        public_key = (x_coord, y_coord)
    else:
        raise ValueError("Invalid public key format")
        
    private_key = 0
    max_iterations = 100000000000  # Maximum number of iterations
    iterations = 0
    while public_key != base_point and iterations < max_iterations:
        public_key = tuple((x - y) % (2**256) for x, y in zip(public_key, base_point))  # Perform element-wise subtraction
        private_key += 1
        iterations += 1
    
    if iterations >= max_iterations:
        raise ValueError("Maximum iterations reached. Solution not found.")
    
    return private_key


def calculate_y_coord(x_coord, is_even):
    p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F  # Field prime for secp256k1
    y_squared = (x_coord ** 3 + 7) % p
    y = pow(y_squared, (p + 1) // 4, p)
    
    if y % 2 == 0:
        even_y = y
        odd_y = p - y
    else:
        even_y = p - y
        odd_y = y
    
    if is_even:
        return even_y
    else:
        return odd_y


if __name__ == "__main__":
    public_key = input("Enter public key: ")
    private_key = derive_private_key(public_key)
    print("Public key:", public_key)
    print("Private key:", private_key)
