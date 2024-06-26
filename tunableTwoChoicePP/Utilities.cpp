#include "Utilities.h"
#include <iostream>
#include <sstream>
#include <map>
#include <openssl/sha.h>
#include <fstream>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

std::map<int, std::chrono::time_point<std::chrono::high_resolution_clock>> Utilities::m_begs;
std::map<std::string, std::ofstream*> Utilities::handlers;
std::map<int, double> timehist;
unsigned char Utilities::key[16];
unsigned char Utilities::iv[16];
std::string Utilities::rootAddress ="/tmp/";

Utilities::Utilities() {
    memset(key, 0x00, 16);
    memset(iv, 0x00, 16);
}

Utilities::~Utilities() {
}

void Utilities::startTimer(int id) {
    std::chrono::time_point<std::chrono::high_resolution_clock> m_beg = std::chrono::high_resolution_clock::now();
    m_begs[id] = m_beg;

}

double Utilities::stopTimer(int id) {
    double t = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_begs[id]).count();
    timehist.erase(id);
    timehist[id] = t;
    return t;
}

//std::string Utilities::getSHA256(std::string input) {
//    CryptoPP::SHA256 hash;
//    unsigned char digest[ CryptoPP::SHA256::DIGESTSIZE ];
//    hash.CalculateDigest(digest, (unsigned char*) input.c_str(), input.length());
//    CryptoPP::HexEncoder encoder;
//    std::string output;
//    encoder.Attach(new CryptoPP::StringSink(output));
//    encoder.Put(digest, sizeof (digest));
//    encoder.MessageEnd();
//    return output;
//}

unsigned char* Utilities::sha256(char* input, int size) 
{

    unsigned char* hash = new unsigned char[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, size);
    SHA256_Final(hash, &sha256);
    return hash;
}

//std::string Utilities::encryptAndEncode(std::string plaintext, unsigned char* key, unsigned char* iv) {
//    std::string ciphertext;
//    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
//    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
//    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
//    stfEncryptor.Put(reinterpret_cast<const unsigned char*> (plaintext.c_str()), plaintext.length() + 1);
//    stfEncryptor.MessageEnd();
//    std::string encodedCiphertext = base64_encode(ciphertext.c_str(), ciphertext.size());
//    return encodedCiphertext;
//    //    return ciphertext;
//}

//std::string Utilities::decodeAndDecrypt(std::string encodedCiphertext, unsigned char* key, unsigned char* iv) {
//    std::string decryptedtext;
//    //    std::string ciphertext = encodedCiphertext;
//    std::string ciphertext = base64_decode(encodedCiphertext);
//    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
//    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);
//    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedtext));
//    stfDecryptor.Put(reinterpret_cast<const unsigned char*> (ciphertext.c_str()), ciphertext.size());
//    stfDecryptor.MessageEnd();
//    return decryptedtext;
//}


static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string Utilities::base64_encode(const char* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';

    }

    return ret;

}

std::string Utilities::base64_decode(std::string const& encoded_string) {
    size_t in_len = encoded_string.size();
    size_t i = 0;
    size_t j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = static_cast<unsigned char> (base64_chars.find(char_array_4[i]));

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = static_cast<unsigned char> (base64_chars.find(char_array_4[j]));

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

std::string Utilities::XOR(std::string value, std::string key) {
    std::string retval(value);

    short unsigned int klen = key.length();
    short unsigned int vlen = value.length();
    short unsigned int k = 0;
    if (klen < vlen) {
        for (int i = klen; i < vlen; i++) {
            key += " ";
        }
    } else {
        for (int i = vlen; i < klen; i++) {
            value += " ";
        }
    }
    klen = vlen;

    for (short unsigned int v = 0; v < vlen; v++) {
        retval[v] = value[v]^key[k];
        k = (++k < klen ? k : 0);
    }

    return retval;
}

void Utilities::logTime(std::string filename, std::string content) {
    (*handlers[filename]) << content << std::endl;
}

void Utilities::finalizeLogging(std::string filename) {
    handlers[filename]->close();
}

void Utilities::initializeLogging(std::string filename) {
    std::ofstream* outfile = new std::ofstream();
    outfile->open(filename, std::ios_base::app);
    handlers[filename] = outfile;
    //    Utilities::handlers.insert(std::pair<std::string, ofstream>(filename,outfile));
}

int Utilities::getMem() { //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

int Utilities::parseLine(char* line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p < '0' || *p > '9') p++;
    line[i - 3] = '\0';
    i = atoi(p);
    return i;
}

std::array<uint8_t, 16> Utilities::convertToArray(std::string addr) {
    std::array<uint8_t, 16> res;
    for (int i = 0; i < 16; i++) {
        res[i] = addr[i];
    }
    return res;
}

double Utilities::getTimeFromHist(int id) {
    if (timehist.count(id) > 0) {
        return timehist[id];
    }
    return 0;
}

int Utilities::getBid(std::string srchIndex) {
    return 0;
}

std::array<uint8_t, 16> Utilities::encode(std::string keyword) {
    unsigned char plaintext[16];
    for (unsigned int i = 0; i < keyword.length(); i++) {
        plaintext[i] = keyword.at(i);
    }
    for (uint i = keyword.length(); i < 16 - 4; i++) {
        plaintext[i] = '\0';
    }

    unsigned char ciphertext[16];
    encrypt(plaintext, strlen((char *) plaintext), key, iv, ciphertext);
    std::array<uint8_t, 16> result;
    for (uint i = 0; i < 16; i++) {
        result[i] = ciphertext[i];
    }
    return result;
}

std::array<uint8_t, 16> Utilities::encode(std::string keyword, unsigned char* curkey) {
    unsigned char plaintext[16];
    for (unsigned int i = 0; i < keyword.length(); i++) {
        plaintext[i] = keyword.at(i);
    }
    for (uint i = keyword.length(); i < 16; i++) {
        plaintext[i] = '\0';
    }
    if (curkey == NULL) {
        curkey = key;
    }
    unsigned char ciphertext[16];
    encrypt(plaintext, 15, curkey, iv, ciphertext);
    std::array<uint8_t, 16> result;
    for (uint i = 0; i < 16; i++) {
        result[i] = ciphertext[i];
    }
    return result;
}

std::array<uint8_t, 16> Utilities::encode(unsigned char* plaintext, unsigned char* curkey) {
    if (curkey == NULL) {
        curkey = key;
    }
    unsigned char ciphertext[16];
    encrypt(plaintext, 15, curkey, iv, ciphertext);
    std::array<uint8_t, 16> result;
    for (uint i = 0; i < 16; i++) {
        result[i] = ciphertext[i];
    }
    return result;
}

std::string Utilities::decode(std::array<uint8_t, 16> ciphertext, unsigned char* curkey) {
    unsigned char plaintext[16];
    unsigned char cipher[16];
    for (uint i = 0; i < 16; i++) {
        cipher[i] = ciphertext[i];
    }
    if (curkey == NULL) {
        curkey = key;
    }
    decrypt(cipher, 16, curkey, iv, plaintext);
    std::string result;
    for (uint i = 0; i < 16 && plaintext[i] != '\0'; i++) {
        result += (char) plaintext[i];
    }
    return result;
}

void Utilities::decode(std::array<uint8_t, 16> ciphertext, std::array<uint8_t, 16>& plaintext, unsigned char* curkey) {
    unsigned char plain[16];
    unsigned char cipher[16];
    for (uint i = 0; i < 16; i++) {
        cipher[i] = ciphertext[i];
    }
    if (curkey == NULL) {
        curkey = key;
    }
    decrypt(cipher, 16, curkey, iv, plain);
    mempcpy(plaintext.data(), plain, 16);
}

int Utilities::encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

void Utilities::handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

int Utilities::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

std::vector<std::string> Utilities::splitData(const std::string& str, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

std::array<uint8_t, 16> Utilities::generatePRF(unsigned char* input, unsigned char* prfkey) {
    unsigned char result[16];
    encrypt(input, 15, prfkey, iv, result);
    std::array<uint8_t, 16> res;
    mempcpy(res.data(), result, 16);
    return res;
}
