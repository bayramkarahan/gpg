#include <iostream>
#include <cstdlib>

int main() {
    int result = system("gpg --verify belge.txt.asc");
    if (result == 0) {
        std::cout << "Dijital imza doğrulandı pozitif" << std::endl;
    } else {
        std::cout << "Dijital imza doğrulanamadı negatif " << std::endl;
    }
    return 0;
}
