Proda: Минималистичный HTTP/1.1 сервер на C

Минималистичная, но полнофункциональная реализация HTTP/1.1 сервера на языке C, демонстрирующая ключевые навыки современного разработчика и DevOps-инженера.

## Особенности
Высокая производительность: Нативный код на C с эффективной обработкой соединений
Кроссплатформенность: Работает на Linux, Windows и в контейнерах Docker
Простая архитектура: Чистая и понятная кодовая база для легкого расширения
Полное логирование: Поддержка уровней debug, info, warn, error
Тестирование: Комплексные юнит-тесты для ключевых компонентов
Контейнеризация: Готовые Docker-образы для быстрого развертывания
CI/CD: Автоматизированная сборка и тестирование через GitHub Actions

## Технологический стек
Язык: C11 (стандарт с современными возможностями)
Сети: Беркли-сокеты (POSIX-compliant)
Сборка: Makefile с поддержкой кросс-компиляции
Контейнеризация: Docker с многоэтапной сборкой
CI/CD: GitHub Actions с автоматическим деплоем
Тестирование: Unity Framework для unit-тестов
Конфигурация: YAML для гибких настроек сервера

## Структура проекта
``` text
proda/
├── scripts/
│   ├── headers/     # Заголовочные файлы
│   └── src/         # Исходный код на C
├── tests/           # Юнит-тесты
├── docker/          # Docker-конфигурации
├── .github/
│   └── workflows/   # CI/CD конфигурации
├── Dockerfile       # Описание Docker-образа
├── Makefile         # Система сборки
├── info.yaml        # Конфигурация сервера
└── script.py        # Утилиты для деплоя и мониторинга
```

## Быстрый старт
Сборка из исходников
``` bash
# Клонирование репозитория
git clone https://github.com/your-username/proda.git
cd proda

# Проверка работы 
curl http://localhost:8080/
curl http://localhost:8080/api/status
```

# Запуск в Docker
``` bash
# Сборка Docker-образа
docker build -t proda-server .

# Запуск контейнера
docker run -p 8080:8080 -p 8081:8081 proda-server

# Или использование готового образа
docker run -p 8080:8080 your-username/proda-server:latest
```

Тестирование
``` bash
# Запуск unit-тестов
make test

# Проверка покрытия кода
make coverage

# Статический анализ кода
make analyze
Конфигурация
Настройки сервера задаются в файле info.yaml:
```

``` yaml
server:
  host: "0.0.0.0"
  port: 8080
  max_connections: 100
  timeout: 30

logging:
  level: "info"
  file: "/var/log/proda/server.log"

routes:
  - path: "/"
    handler: "root_handler"
  - path: "/api/status"
    handler: "status_handler"
  - path: "/api/metrics"
    handler: "metrics_handler"
```

## CI/CD Pipeline
Проект включает готовый workflow для GitHub Actions:
Сборка: Автоматическая компиляция для разных платформ
Тестирование: Запуск unit-тестов и проверка покрытия
Анализ: Статический анализ кода и проверка стиля
Docker: Сборка и публикация Docker-образов
Деплой: Автоматическое развертывание на тестовые среды

Мониторинг и управление
``` bash
# Деплой сервера
python script.py --deploy --config production.yaml

# Мониторинг работы
python script.py --monitor --interval 5

# Статистика производительности
python script.py --stats
```
