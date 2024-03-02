#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>

#include "../default.h"
#include "../shared.h"

#include "calculateHash.h"

char *generateMD5(char *string_p) {
    unsigned char mdValue[EVP_MAX_MD_SIZE];
    unsigned int mdLength;
    size_t i;
    char *formattedHash_p = (char *) calloc(MD5_STR_SIZE, sizeof(char));
    if (formattedHash_p == NULL) {
        showError(NULL);
        return NULL;
    }

    EVP_MD_CTX *MDctx_p = EVP_MD_CTX_new();

    EVP_DigestInit_ex(MDctx_p, EVP_md5(), NULL);
    EVP_DigestUpdate(MDctx_p, string_p, strlen(string_p));
    EVP_DigestFinal_ex(MDctx_p, mdValue, &mdLength);

    for (i = 0; i < mdLength; i++) {
        snprintf(&formattedHash_p[i * 2], 3, "%02x", mdValue[i]); // Transform int to correct format
    }

    EVP_MD_CTX_free(MDctx_p);
    return formattedHash_p;
}
