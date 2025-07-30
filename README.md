# MyLogger

MyLogger - это кроссплатформенная библиотека логирования для C++, поддерживающая многоуровневое логирование с потокобезопасностью. Библиотека работает как на Windows, так и на Linux системах.

## Особенности

- **Кроссплатформенность**: Поддержка Windows и Linux
- **Многоуровневое логирование**: INFO, SECRET_INFO, TOP_SECRET_INFO
- **Потокобезопасность**: Использование мьютексов для защиты от гонки потоков
- **Гибкая сборка**: Поддержка статической и динамической линковки
- **API экспорт**: Корректный экспорт DLL на Windows

## Структура проекта

```
MyLogger/
├── include/           # Заголовочные файлы библиотеки
│   └── logger.h
├── src/              # Исходный код библиотеки
│   └── logger.cpp
├── app/              # Тестовое приложение
│   ├── include/      # Заголовки приложения
│   └── src/          # Исходники приложения
├── tests/            # Модульные тесты
└── CMakeLists.txt    # Конфигурация сборки
```

## Сборка

### Требования

- CMake 3.30 или выше
- GCC компилятор (рекомендуется)
- C++17 стандарт

### Параметры сборки

- `BUILD_TESTS=ON/OFF` - Собирать тесты (по умолчанию ON)
- `BUILD_APP=ON/OFF` - Собирать тестовое приложение (по умолчанию ON)

Пример:
```bash
cmake -DBUILD_TESTS=OFF -DBUILD_APP=ON ..
```

## Использование библиотеки


```cpp
Logger logger;

// Инициализация с файлом и уровнем по умолчанию
LogResult result = logger.init("app", LogLevel::INFO);
if (result != LogResult::SUCCESS) {
    // Обработка ошибки
}

// Логирование
logger.log("Информационное сообщение", LogLevel::INFO);
logger.log("Секретная информация", LogLevel::SECRET_INFO);
logger.log("Совершенно секретно", LogLevel::TOP_SECRET_INFO);

// Изменение уровня логирования
logger.setLogLevel(LogLevel::SECRET_INFO);
```

### Уровни логирования

- `LogLevel::INFO` - Базовая информация
- `LogLevel::SECRET_INFO` - Конфиденциальная информация  
- `LogLevel::TOP_SECRET_INFO` - Совершенно секретная информация

Уровни работают иерархически: если установлен `SECRET_INFO`, то будут записываться только сообщения уровня `SECRET_INFO` и `TOP_SECRET_INFO`.

### Линковка

#### Статическая библиотека
```cmake
target_link_libraries(your_target mylogger)
```

#### Динамическая библиотека (Windows)
```cmake
target_link_libraries(your_target mylogger)
target_compile_definitions(your_target PRIVATE MYLOGGER_SHARED)
```

## Тестовое приложение

Проект включает интерактивное тестовое приложение, демонстрирующее работу с библиотекой.

### Запуск

```bash
# Linux
./logger_app logs.log [уровень]

# Windows
logger_app.exe logs.log [уровень]
```

### Параметры

- `logs` - путь к файлу логов
- `уровень` - уровень логирования по умолчанию (INFO, SECRET_INFO, TOP_SECRET_INFO)

### Пример использования

```bash
./logger_app app.log SECRET_INFO
```

### Команды в приложении

- Ввод сообщения с автоматическим определением уровня
- Команды для изменения уровня логирования
- Интерактивный ввод сообщений

## API Reference

### Класс Logger

#### Методы

- `LogResult init(const std::string& fileName, LogLevel defaultLevel)` - Инициализация логгера
- `LogResult log(const std::string& message, LogLevel level = LogLevel::INFO)` - Запись сообщения
- `void setLogLevel(LogLevel level)` - Установка уровня логирования
- `LogLevel getLogLevel() const` - Получение текущего уровня
- `bool isInitialized() const` - Проверка инициализации

#### Коды возврата (LogResult)

- `SUCCESS` - Операция выполнена успешно
- `FILE_OPEN_ERROR` - Ошибка открытия файла
- `WRITE_ERROR` - Ошибка записи в файл
- `INVALID_LEVEL` - Некорректный уровень логирования