#include <gpgme.h>
#include <iostream>

int main() {
    gpgme_check_version(NULL);
    gpgme_ctx_t ctx;
    gpgme_error_t err;

    // GPGME bağlamını başlat
    err = gpgme_new(&ctx);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "GPGME bağlamı başlatılamadı." << std::endl;
        return 1;
    }

      
     // İmza doğrulama için gerekli verileri ayarla
    gpgme_data_t signed_data;
    gpgme_data_t signature;
    gpgme_verify_result_t verify_result;

    // Doğrulanacak veriyi ve imzayı ayarla
    gpgme_data_new_from_file(&signed_data, "deneme.txt.asc", 1);
    gpgme_data_new_from_file(&signature, "pubkey.asc", 1);


    // İmzayı doğrula
    gpgme_verify_result_t result;
  err = gpgme_op_verify(ctx, signature, signed_data, NULL);
  /*    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "İmza doğrulanamadı." << std::endl;
        gpgme_release(ctx);
        return 1;
    }
*/
    // Doğrulama sonucunu al
    result = gpgme_op_verify_result(ctx);
    if (result->signatures && result->signatures->status == GPGME_SIG_STAT_GOOD) {
        std::cout << "İmza geçerli." << std::endl;
    } else {
        std::cout << "İmza geçersiz." << std::endl;
    }

    // GPGME bağlamını serbest bırak
    gpgme_release(ctx);

    return 0;
}

