#include <stdio.h>
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

    // Имя функции, которую хотим импортировать
    const char *func_name = "VOS_CheckFileImgCRC";
    // Получаем указатель на функцию
    typedef int (*my_function_type)(const char* filename);
    my_function_type my_function = (my_function_type) dlsym(handle, func_name);

    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        fprintf(stderr, "Ошибка поиска функции: %s\n", dlsym_error);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    // Пример имени файла для проверки
    const char *filename = "firmware.original";
    //const char *filename = "firmware-fixed2.img";

    // Вызов импортированной функции с передачей имени файла и получение результата
    int result = my_function(filename);
    printf("Результат вызова функции проверки: %d\n", result);

    // Закрываем библиотеку
    dlclose(handle);
	
	
	
	// ------------------------------
	
	
	FILE *file = fopen("cramfs.img", "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    // Перемещение указателя в конец файла, чтобы узнать его размер
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);  // Возвращаем указатель в начало файла

    // Выделение памяти для буфера
    char *buffer = (char *)malloc(file_size);
    if (buffer == NULL) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return 1;
    }

    // Чтение данных файла в буфер
    size_t read_size = fread(buffer, 1, file_size, file);
    if (read_size != file_size) {
        perror("Ошибка чтения файла");
        free(buffer);
        fclose(file);
        return 1;
    }







   
    // Освобождение ресурсов
    free(buffer);
    fclose(file);
    return 0;
}