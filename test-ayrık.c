#include <iostream>
#include <cstdlib>

int main() {
    int result = system("gpg --verify belge.txt.sig belge.txt");
    if (result == 0) {
        std::cout << "Dijital imza doğrulandı." << std::endl;
    } else {
        std::cout << "Dijital imza doğrulanamadı." << std::endl;
    }
    return 0;
}
