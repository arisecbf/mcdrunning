//
//  ServerConn.cpp
//  mcdrunning
//
//  Created by chenbingfeng on 15/6/18.
//
//

#include "ServerConn.h"
#include <openssl/aes.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <openssl/bio.h>
#include <openssl/buffer.h>

#include <stdint.h>
#include "base64.h"
#include <string>
namespace tr {

    void handleErrors(void)
    {
        CCLOG("openssl error");
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

        EVP_CIPHER_CTX_set_padding(ctx, 0);

        /* Provide the message to be encrypted, and obtain the encrypted output.
         * EVP_EncryptUpdate can be called multiple times if necessary
         */
        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
            handleErrors();
        ciphertext_len = len;

        /* Finalise the encryption. Further ciphertext bytes may be written at
         * this stage.
         */
        if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
        ciphertext_len += len;

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

        // set not padding
        EVP_CIPHER_CTX_set_padding(ctx, 0); // must after init

        /* Provide the message to be decrypted, and obtain the plaintext output.
         * EVP_DecryptUpdate can be called multiple times if necessary
         */
        if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
            handleErrors();
        plaintext_len = len;

        /* Finalise the decryption. Further plaintext bytes may be written at
         * this stage.
         */
        if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
        plaintext_len += len;

        /* Clean up */
        EVP_CIPHER_CTX_free(ctx);
        
        return plaintext_len;
    }

    // 256 bits key
    static unsigned char *KEY = (unsigned char *)"01234567890123456789012345678901";

    // 128 bits IV
    static unsigned char *IV = (unsigned char *)"0123456789012345";


    std::string encry(std::string datastring)
    {
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
        ciphertext_len = encrypt ((unsigned char *)datastring.c_str(), (int)datastring.size(), KEY, IV, ciphertext);

        // Clean up
        EVP_cleanup();
        ERR_free_strings();

        auto res = base64_encode(ciphertext, ciphertext_len);

        free(ciphertext);
        return res;
    }

    std::string decry(const std::string& b64string)
    {
        auto data = base64_decode(b64string);

        unsigned char * decryptedtext = (unsigned char *)malloc((data.length() + AES_BLOCK_SIZE)*sizeof(unsigned char));
        assert(decryptedtext != nullptr);
        int decryptedtext_len;

        /* Initialise the library */
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();
        OPENSSL_config(NULL);

        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt((unsigned char *)data.c_str(), data.size(), KEY, IV, decryptedtext);

        /* Add a NULL terminator. We are expecting printable text */
        decryptedtext[decryptedtext_len] = '\0';
        std::string res((char*)decryptedtext);

        /* Clean up */
        EVP_cleanup();
        ERR_free_strings();
        free(decryptedtext);

        return res;
    }

    ///////////test code//////////////
    void test(){
        std::string in = "{\"err_code\":1,\"err_msg\":\"id_string exists\"} ";
        auto enc = encry(in);
        CCLOG("%s", enc.c_str());
        auto dec = decry(enc);
        CCLOG("%s", dec.c_str());
    }

    void test2(){
        /* Set up the key and iv. Do I need to say to not hard code these in a
         * real application? :-)
         */

        /* A 256 bit key */
        unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

        /* A 128 bit IV */
        unsigned char *iv = (unsigned char *)"01234567890123456";

        /* Message to be encrypted */
        unsigned char *plaintext =
        (unsigned char *)"{\"err_code\":1,\"err_msg\":\"id_string exists\"}     ";

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

        /* Encrypt the plaintext */
        ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,
                                  ciphertext);

        /* Do something useful with the ciphertext here */
        printf("Ciphertext is:\n");
        BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
        auto enc = base64_encode(ciphertext, ciphertext_len);
        CCLOG("%s", enc.c_str());
        /* Decrypt the ciphertext */
        decryptedtext_len = decrypt((unsigned char*)base64_decode(enc).c_str(), ciphertext_len, key, iv,
                                    decryptedtext);
        
        /* Add a NULL terminator. We are expecting printable text */
        decryptedtext[decryptedtext_len] = '\0';
        
        /* Show the decrypted text */
        printf("Decrypted text is:\n");
        printf("%s\n", decryptedtext);
        
        /* Clean up */
        EVP_cleanup();
        ERR_free_strings();
        test2();
    }
} // tr