#include <gpgme.h>
#include <iostream>
#include <string.h>
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

    // İmzalama için kullanılacak anahtarın kimliğini ayarla
    gpgme_key_t key;
    err = gpgme_get_key(ctx, "14F96F24331C6A88BEB35AAB7C73638BCDB196AF", &key, 0);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "Anahtar bulunamadı." << std::endl;
        gpgme_release(ctx);
        return 1;
    }

    // İmzalanacak belgenin içeriğini ayarla
    const char* document = "merhaba dostum..";
    gpgme_data_t in;
    err = gpgme_data_new_from_mem(&in, document, strlen(document), 1);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "Belge içeriği ayarlanamadı." << std::endl;
        gpgme_key_unref(key);
        gpgme_release(ctx);
        return 1;
    }

    // İmzalama işlemini gerçekleştir
    gpgme_data_t out;
    err = gpgme_data_new(&out);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "İmzalama işlemi gerçekleştirilemedi." << std::endl;
        gpgme_data_release(in);
        gpgme_key_unref(key);
        gpgme_release(ctx);
        return 1;
    }

    err = gpgme_op_sign(ctx, in, out, GPGME_SIG_MODE_DETACH);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "İmzalama işlemi başarısız oldu." << std::endl;
        gpgme_data_release(in);
        gpgme_data_release(out);
        gpgme_key_unref(key);
        gpgme_release(ctx);
        return 1;
    }

    // İmzalı belgeyi al
    const char* signed_document = gpgme_data_release_and_get_mem(out, NULL);
    std::cout << "imzasız belge: " << document << std::endl;
  std::cout << "İmzalı belge: " << signed_document << std::endl;
  printf("deneme:%s",signed_document);

    // Kaynakları temizle
    gpgme_data_release(in);
    gpgme_key_unref(key);
    gpgme_release(ctx);

    return 0;
}

