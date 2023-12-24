#include <gpgme.h>
#include <cstdlib>
//c++ -o imza-kontrol imza-kontrol.c -lgpgme


int main() {
    gpgme_check_version(NULL);
    gpgme_ctx_t ctx;
    gpgme_error_t err;

    // GPGME bağlamı oluştur
    err = gpgme_new(&ctx);
    if (err != GPG_ERR_NO_ERROR) {
        // Hata durumunda işlemler yapılabilir
        return 1;
    }

    // İmza doğrulama için gerekli verileri ayarla
    gpgme_data_t signed_data;
    gpgme_data_t signature;
    gpgme_verify_result_t verify_result;

    // Doğrulanacak veriyi ve imzayı ayarla
    gpgme_data_new_from_file(&signed_data, "belge.txt", 1);
    gpgme_data_new_from_file(&signature, "belge.txt.sig", 1);

//printf("Data: %c",signed_data);
    // İmza doğrulama işlemini gerçekleştir
    err = gpgme_op_verify(ctx, signature, signed_data, NULL);
    if (err != GPG_ERR_NO_ERROR) {
        // Hata durumunda işlemler yapılabilir
        printf("hata var.%d\n",err);
           printf("signed_data var.%s\n",signed_data);
              printf("signature var.%s\n",signature);
        return 1;
    }

    // İmza doğrulama sonucunu al
    verify_result = gpgme_op_verify_result(ctx);

    // İmza doğrulama sonucunu kontrol et
    if (verify_result->signatures && verify_result->signatures->status == GPGME_SIG_STAT_GOOD) {
        // İmza doğrulandı
        printf("İmza doğrulandı.\n");
    } else {
        // İmza doğrulanamadı
        printf("İmza doğrulanamadı.\n");
    }

    // Bellek temizleme
    gpgme_data_release(signed_data);
    gpgme_data_release(signature);
    gpgme_release(ctx);

    return 0;
}

