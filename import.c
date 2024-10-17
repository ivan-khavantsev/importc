#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    // Имя динамической библиотеки
    const char *lib_name = "/usr/lib/libvos.so";  // Пример имени библиотеки

    // Открываем библиотеку
    void *handle = dlopen(lib_name, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // Очищаем возможные ошибки
    dlerror(); 

    // Получаем указатель на функцию
    typedef int (*my_function_type)(int crc, const uint8_t *buf, size_t len);
    my_function_type my_function = (my_function_type) dlsym(handle, "crc_Calculate32");

    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        fprintf(stderr, "Ошибка поиска функции: %s\n", dlsym_error);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    // Пример имени файла для проверки
    //const char *filename = "firmware.original";
    //const char *filename = "firmware-fixed2.img";

   
	
	
	
	// ------------------------------
	
	
	FILE *file = fopen("cramfs.img", "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

	int crc = 0x00000000;
	//int crc = 0x464C457F;
	
	uint8_t buffer[1024];
	size_t bytesRead;
	
	// Чтение файла блоками
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        crc = my_function(crc, buffer, bytesRead);
    }

    // Закрытие файла
    fclose(file);
	printf("CRC: %x\n", crc);
   
	
	
	
    return 0;
}