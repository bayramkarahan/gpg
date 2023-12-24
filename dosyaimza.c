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

    // İmzalanacak dosyanın adını belirle
    const char* dosyaAdi = "belge.txt";

    // İmzalama işlemini gerçekleştir
    gpgme_data_t input, output;
    err = gpgme_data_new_from_file(&input, dosyaAdi, 1);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "Dosya açılamadı." << std::endl;
        gpgme_key_unref(key);
        gpgme_release(ctx);
        return 1;
    }
    err = gpgme_data_new(&output);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "Çıktı verisi oluşturulamadı." << std::endl;
        gpgme_data_release(input);
        gpgme_key_unref(key);
        gpgme_release(ctx);
        return 1;
    }
    err = gpgme_op_sign(ctx, input, output, GPGME_SIG_MODE_DETACH);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "İmzalama işlemi başarısız oldu." << std::endl;
        gpgme_data_release(input);
        gpgme_data_release(output);
        gpgme_key_unref(key);
        gpgme_release(ctx);
        return 1;
    }

    // İmzalı dosyayı kaydet
    const char* imzaliDosyaAdi = "belgeimzali.txt";
    err = gpgme_data_seek(output, 0, SEEK_SET);
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "Çıktı verisi konumu ayarlanamadı." << std::endl;
        gpgme_data_release(input);
        gpgme_data_release(output);
        gpgme_key_unref(key);
        gpgme_release(ctx);
        return 1;
    }
    err = gpgme_data_write(output, imzaliDosyaAdi, strlen(imzaliDosyaAdi));
    if (err != GPG_ERR_NO_ERROR) {
        std::cerr << "İmzalı dosya kaydedilemedi."<<err << std::endl;
        gpgme_data_release(input);
        gpgme_data_release(output);
        gpgme_key_unref(key);
        gpgme_release(ctx);
        return 1;
    }

    // Bellek temizleme
    gpgme_data_release(input);
    gpgme_data_release(output);
    gpgme_key_unref(key);
    gpgme_release(ctx);

    std::cout << "Dosya başarıyla imzalandı: " << imzaliDosyaAdi << std::endl;

    return 0;
}

