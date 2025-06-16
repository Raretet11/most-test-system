[![CI](https://github.com/Raretet11/most-test-system/actions/workflows/ci.yml/badge.svg)](https://github.com/Raretet11/most-test-system/actions/workflows/ci.yml)

<div align="center">
	<code><img width="50" src="https://raw.githubusercontent.com/marwin1991/profile-technology-icons/refs/heads/main/icons/vue_js.png" alt="Vue.js" title="Vue.js"/></code>
	<code><img width="50" src="https://raw.githubusercontent.com/marwin1991/profile-technology-icons/refs/heads/main/icons/c++.png" alt="C++" title="C++"/></code>
	<code><img width="50" src="https://raw.githubusercontent.com/marwin1991/profile-technology-icons/refs/heads/main/icons/postgresql.png" alt="PostgreSQL" title="PostgreSQL"/></code>
</div>

# most - mvp

**most-test sys.** — это архив задач с встроенной тестирующей системой, предназначенной для проверки решений пользователей, для проверки задач используется Isolate - система для проверки задач используемая на международной олимпиаде по информатике (IOI), для хранения данных используется база данных Postgres. Сервис представлен бекенд и фронтенд составляющими, для взаимодействия между фронтендом и бекендом используется REST API, подробнее о нем ниже. Бекенд написан на C++ с ипользованием фреймворвка Userver, фронтенд написан на Vue.js

## Технологии и языки

1) `C++`, `python`, `JavaScript` - языки используемые при разработке
2) `Userver`, `Vue.js` - бекенд//фронтенд фреймворк
4) `Postgresql` - база данных
5) `CMake` - система сборки бекенда

## Возможности

1) Загрузка собственных задач (они сохраняются в базе данных)
2) Отправка решений и их тестирование (решение после также сохраняются в базе данных)
3) Веб-интерфейс для удобного взаимодействия с системой
4) Открытое API для использования внешними сервисами (например можно скачать все решения для работы бан-машины)

## Makefile

Запуск и развертываение бекенд сервиса происходит через make

`PRESET` - это `debug`, `release`

* `make cmake-PRESET` - запуск настройки cmake, обновление параметров cmake и списков исходных файлов
* `make build-PRESET` - построить сервис
* `make test-PRESET` - построить сервис и запустить все тесты
* `make dist-clean` - почистить кэш и удалить созданные файлы
