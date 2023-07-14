#include <iostream>
#include <string>
#include <vector>
#include <gmp.h>

using namespace std;
typedef unsigned char byte;
typedef vector<byte> PublicKey;

int calculateYCoord(const mpz_t xCoord, bool isEven) {
    mpz_t p, ySquared, y;
    mpz_inits(p, ySquared, y, NULL);

    mpz_set_str(p, "115792089237316195423570985008687907853269984665640564039457584007908834671663", 10); // Field prime for secp256k1
    mpz_pow_ui(ySquared, xCoord, 3);
    mpz_add_ui(ySquared, ySquared, 7);
    mpz_mod(ySquared, ySquared, p);
    mpz_powm_ui(y, ySquared, (mpz_get_ui(p) + 1) / 4, p);

    int yCoord;
    if (mpz_even_p(y))
        yCoord = mpz_get_ui(y);
    else {
        mpz_sub(y, p, y);
        yCoord = mpz_get_ui(y);
    }

    mpz_clears(p, ySquared, y, NULL);

    return yCoord;
}

PublicKey derivePrivateKey(const string& publicKey) {
    PublicKey pubKey;

    if (publicKey.substr(0, 2) == "04") {  // Uncompressed public key
        for (size_t i = 2; i < publicKey.length(); i += 2) {
            byte byteValue = static_cast<byte>(stoi(publicKey.substr(i, 2), nullptr, 16));
            pubKey.push_back(byteValue);
        }
    } else if (publicKey.substr(0, 2) == "02" || publicKey.substr(0, 2) == "03") {  // Compressed public key
        string prefix = publicKey.substr(0, 2);
        string xCoordStr = publicKey.substr(2);
        mpz_t xCoord;
        mpz_init(xCoord);
        mpz_set_str(xCoord, xCoordStr.c_str(), 16);

        int yCoord;
        if (prefix == "02")
            yCoord = calculateYCoord(xCoord, true);
        else if (prefix == "03")
            yCoord = calculateYCoord(xCoord, false);
        else
            throw invalid_argument("Invalid public key format");

        pubKey.push_back(static_cast<byte>(mpz_get_ui(xCoord)));
        pubKey.push_back(static_cast<byte>(yCoord));

        mpz_clear(xCoord);
    } else {
        throw invalid_argument("Invalid public key format");
    }

    PublicKey privateKey;
    PublicKey basePoint = {
        0x02, 0x79, 0xBE, 0x66, 0x7E, 0xF4, 0x8A, 0x03, 0xBB, 0xAC, 0x99, 0x9B, 0x5A, 0xCE, 0x6E, 0xEF,
        0x48, 0x56, 0x1A, 0x14, 0x9E, 0x38, 0x77, 0x63, 0x74, 0x8F, 0xAE, 0x6A, 0x7E, 0x84, 0xF3, 0x42
    };

    mpz_t p;
    mpz_init_set_str(p, "115792089237316195423570985008687907853269984665640564039457584007908834671663", 10); // Field prime for secp256k1
    size_t maxIterations = 100000000000ULL;  // Maximum number of iterations
    size_t iterations = 0;

    while (pubKey != basePoint && iterations < maxIterations) {
        for (size_t i = 0; i < pubKey.size(); ++i) {
            pubKey[i] = static_cast<byte>((pubKey[i] - basePoint[i]) % 256);  // Perform element-wise subtraction
        }
        privateKey.push_back(1);
        iterations++;
    }

    if (iterations >= maxIterations) {
        throw runtime_error("Maximum iterations reached. Solution not found.");
    }

    mpz_clear(p);
    return privateKey;
}

int main() {
    PublicKey basePoint = {
        0x02, 0x79, 0xBE, 0x66, 0x7E, 0xF4, 0x8A, 0x03, 0xBB, 0xAC, 0x99, 0x9B, 0x5A, 0xCE, 0x6E, 0xEF,
        0x48, 0x56, 0x1A, 0x14, 0x9E, 0x38, 0x77, 0x63, 0x74, 0x8F, 0xAE, 0x6A, 0x7E, 0x84, 0xF3, 0x42
    };

    string public_key;
    cout << "Enter public key: ";
    cin >> public_key;

    PublicKey private_key = derivePrivateKey(public_key);

    cout << "Public key: " << public_key << endl;
    cout << "Private key: ";
    for (const auto& byteValue : private_key) {
        cout << hex << static_cast<int>(byteValue);
    }
    cout << endl;

    return 0;
}
