//
//  ServerConn.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/18.
//
//

#include "ServerConn.h"
#include "openssl/aes.h"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <stdint.h>
#include "base64.h"
#include <string>
namespace sco {

    void dispByte(unsigned char * p, size_t n){
        std::stringstream is;
        for (size_t i = 0; i < n; i++) {
            is << (0xf & (p[i] >> 4)) << (p[i]&0xf);
        }
        CCLOG("%s", is.str().c_str());
    }

    void httpTest2()
    {
        AES_KEY aesKey;
        size_t len = 0;

        unsigned char pin[16] = {0x05, 0xC9, 0xAB, 0x00, 0x1A, 0x99, 0xC4, 0x4B, 0x26, 0xE8, 0xDC, 0xD1, 0x51, 0x10, 0x8E, 0x9B};
        unsigned char * pout;
        unsigned char * pdst;

        len = 16;

        pout = static_cast<unsigned char*>(malloc(len* sizeof(unsigned char)));
        pdst = static_cast<unsigned char*>(malloc(len * sizeof(unsigned char)));

        dispByte(pin, 16);
        /*
        salt=F4C3184497E50F0F
        key=05C9AB001A99C44B26E8DCD151108E9B
        iv =6924EC1E391AE4C5778585F24093BC05
        */
        unsigned char keyRaw[16] = {0x05, 0xC9, 0xAB, 0x00, 0x1A, 0x99, 0xC4, 0x4B, 0x26, 0xE8, 0xDC, 0xD1, 0x51, 0x10, 0x8E, 0x9B};

        unsigned char ivRaw[16] = {0x69, 0x24, 0xEC, 0x1E, 0x39, 0x1A, 0xE4, 0xC5, 0x77, 0x85, 0x85, 0xF2, 0x40, 0x93, 0xBC, 0x05};
        //for (size_t i = 0; i < 16; i++) {
          //  ivRaw[i] = 0;
        //}

        // set key
        AES_set_encrypt_key(keyRaw, 128, &aesKey);
        AES_cbc_encrypt(pin, pout, len, &aesKey, ivRaw, AES_ENCRYPT);
        dispByte(pout, 16);

        AES_set_encrypt_key(keyRaw, 128, &aesKey);
        AES_cbc_encrypt(pout, pdst, len, &aesKey, ivRaw, AES_DECRYPT);
        dispByte(pdst, 16);
        free(pout);
        free(pdst);
    }
    void handleErrors(void)
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
                unsigned char *iv, unsigned char *ciphertext)
    {
        EVP_CIPHER_CTX *ctx;

        int len;

        int ciphertext_len;

        /* Create and initialise the context */
        if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

        /* Initialise the encryption operation. IMPORTANT - ensure you use a key
         * and IV size appropriate for your cipher
         * In this example we are using 256 bit AES (i.e. a 256 bit key). The
         * IV size for *most* modes is the same as the block size. For AES this
         * is 128 bits */
        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
            handleErrors();

        /* Provide the message to be encrypted, and obtain the encrypted output.
         * EVP_EncryptUpdate can be called multiple times if necessary
         */
        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
            handleErrors();
        ciphertext_len = len;

        /* Finalise the encryption. Further ciphertext bytes may be written at
         * this stage.
         */
        //if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
        //else ciphertext_len += len;
        
        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);
        
        return ciphertext_len;
    }

    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
                unsigned char *iv, unsigned char *plaintext)
    {
        EVP_CIPHER_CTX *ctx;

        int len;

        int plaintext_len;

        /* Create and initialise the context */
        if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

        /* Initialise the decryption operation. IMPORTANT - ensure you use a key
         * and IV size appropriate for your cipher
         * In this example we are using 256 bit AES (i.e. a 256 bit key). The
         * IV size for *most* modes is the same as the block size. For AES this
         * is 128 bits */
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
            handleErrors();

        /* Provide the message to be decrypted, and obtain the plaintext output.
         * EVP_DecryptUpdate can be called multiple times if necessary
         */
        if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
            handleErrors();
        plaintext_len = len;

        /* Finalise the decryption. Further plaintext bytes may be written at
         * this stage.
         */
        //if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
        //plaintext_len += len;
        
        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);
        
        return plaintext_len;
    }

    void httpTest3(){

        /* Set up the key and iv. Do I need to say to not hard code these in a
         * real application? :-)
         */

        /* A 256 bit key */
        unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

        /* A 128 bit IV */
        unsigned char *iv = (unsigned char *)"0123456789012345";

        /* Message to be encrypted */
        unsigned char *plaintext = (unsigned char *)"hello";

        /* Buffer for ciphertext. Ensure the buffer is long enough for the
         * ciphertext which may be longer than the plaintext, dependant on the
         * algorithm and mode
         */
        unsigned char ciphertext[128];

        /* Buffer for the decrypted text */
        unsigned char decryptedtext[128];

        int decryptedtext_len, ciphertext_len;

        /* Initialise the library */
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();
        OPENSSL_config(NULL);

        auto srcLen = strlen((char*)plaintext);
        /* Encrypt the plaintext */
        ciphertext_len = encrypt (plaintext, srcLen, key, iv,
                                  ciphertext);

        /* Do something useful with the ciphertext here */
        BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

        AESB64 gen;

        auto b64text = gen.encry("gggg");//base64_encode(ciphertext, ciphertext_len);
        CCLOG("%s", b64text.c_str());

        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt((unsigned char*)base64_decode(b64text).c_str(), ciphertext_len, key, iv,
                                    decryptedtext);
        
        /* Add a NULL terminator. We are expecting printable text */
        decryptedtext[decryptedtext_len] = '\0';
        
        /* Show the decrypted text */
        CCLOG("Decrypted text is:\n");
        CCLOG("%s\n", decryptedtext);
        
        /* Clean up */
        EVP_cleanup();
        ERR_free_strings();

    }

    void httpTest(){
        AESB64 gen;
        auto b64 = gen.encry("Apple Google Trunro Game");
        CCLOG("%s", b64.c_str());
        auto det = gen.decry("SM+5zuRpsdRHzPy1CTOCNTs4lJ4rPwLJB92C8LNiGnA=");
        CCLOG("%s", det.c_str());
    }

    std::string AESB64::encry(std::string datastring)
    {
        // 256 bits key
        unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

        // 128 bits IV
        unsigned char *iv = (unsigned char *)"0123456789012345";


        // Align to block size
        // Make the python server easy, which is nervous about not aligned data
        auto tmplen = datastring.length();
        if (tmplen % AES_BLOCK_SIZE != 0) {
            for (int i = 0; i < AES_BLOCK_SIZE - (tmplen % AES_BLOCK_SIZE); i++) {
                datastring.append(" ");
            }
        }
        assert(datastring.length() % AES_BLOCK_SIZE == 0);

        // The extra BLOCK size is for safty of EVP
        unsigned char *ciphertext = (unsigned char*)malloc((datastring.length() + AES_BLOCK_SIZE) * sizeof(unsigned char));
        assert(ciphertext != nullptr);

        int ciphertext_len;

        // EVP init
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();
        OPENSSL_config(NULL);

        // Encrypt
        ciphertext_len = encrypt ((unsigned char *)datastring.c_str(), (int)datastring.length(), key, iv, ciphertext);

        // Clean up
        EVP_cleanup();
        ERR_free_strings();

        auto res = base64_encode(ciphertext, ciphertext_len);

        free(ciphertext);
        return res;
    }

    std::string AESB64::decry(const std::string& b64string)
    {
        unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

        unsigned char *iv = (unsigned char *)"0123456789012345";

        auto data = base64_decode(b64string);

        unsigned char * decryptedtext = (unsigned char *)malloc((data.length() + AES_BLOCK_SIZE)*sizeof(unsigned char));
        assert(decryptedtext != nullptr);
        int decryptedtext_len;

        /* Initialise the library */
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();
        OPENSSL_config(NULL);

        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt((unsigned char *)data.c_str(), 48, key, iv, decryptedtext);

        /* Add a NULL terminator. We are expecting printable text */
        decryptedtext[decryptedtext_len] = '\0';
        std::string res((char*)decryptedtext);

        /* Clean up */
        EVP_cleanup();
        ERR_free_strings();
        free(decryptedtext);

        return res;
    }

} // sco