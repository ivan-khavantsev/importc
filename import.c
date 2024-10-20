#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    // Открываем библиотеку
    void *handle = dlopen("/usr/lib/libvos.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // Очищаем возможные ошибки
    dlerror(); 

    // Получаем указатель на функцию
    typedef int (*dlFunctionType)(int crc, const uint8_t *buf, size_t len);
    dlFunctionType dlFunction = (dlFunctionType) dlsym(handle, "crc_Calculate32");

    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        fprintf(stderr, "Ошибка поиска функции: %s\n", dlsym_error);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

	// Файл, для которого хотим посчитать контрольную сумму
	FILE *file = fopen("cramfs.img", "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    // Изначальное значение CRC
	int crc = 0x00000000;

    // Чтение файла блоками
    uint8_t buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        crc = dlFunction(crc, buffer, bytesRead);
    }

    fclose(file);
	printf("CRC: %x\n", crc);
    return 0;
}