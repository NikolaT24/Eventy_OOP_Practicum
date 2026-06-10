<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=0:0f2027,50:203a43,100:2c5364&height=230&section=header&text=Eventy&fontSize=70&fontAlignY=35&fontColor=ffffff&animation=fadeIn&desc=CLI%20система%20за%20събития,%20билети%20и%20доброволчество&descAlignY=57&descSize=17" />
</p>

<div align="center">

  <img src="https://readme-typing-svg.herokuapp.com?font=JetBrains+Mono&size=22&duration=2500&pause=800&color=6A11CB&center=true&vCenter=true&width=780&lines=Управление+на+събития+през+CLI;Билети,+места+и+баланс;Доброволчество+и+кандидатури;Админ+одобрения+и+заявки;C%2B%2B23+OOP+архитектура;Файлово+съхранение+и+валидация" />

  <br><br>

  <img src="https://img.shields.io/badge/C++23-OOP%20Project-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
  <img src="https://img.shields.io/badge/CLI-Event%20Management-6A11CB?style=for-the-badge&logo=gnubash&logoColor=white">
  <img src="https://img.shields.io/badge/FMI-Sofia%20University-2C5364?style=for-the-badge">
  <img src="https://img.shields.io/badge/Status-Completed-2ECC71?style=for-the-badge">

</div>

<br>

<p align="center">
  <b>Eventy</b> е конзолно C++23 приложение за управление на събития, закупуване на билети, кандидатстване за доброволчество и обработка на заявки чрез администраторски контрол.
</p>

<p align="center">
  <i>Една CLI сцена. Много събития. Нула хаос.</i> 🧙‍♂️✨
</p>

---

# 📜 Съдържание

- [✨ За проекта](#-за-проекта)
- [🧭 Основна идея](#-основна-идея)
- [🧙 Роли в системата](#-роли-в-системата)
- [🏛️ Видове събития](#️-видове-събития)
- [⚙️ Функционалности](#️-функционалности)
- [🧬 Основни абстракции](#-основни-абстракции)
- [🛠️ CLI команди](#-cli-команди)
- [🔔 Известия](#-известия)
- [💾 Файлово съхранение](#-файлово-съхранение)
- [🧪 Валидация](#-валидация)
- [🚀 Стартиране](#-стартиране)
- [🗂️ Структура](#️-структура)
- [🏰 Архитектура](#-архитектура)
- [🧩 Примерна сесия](#-примерна-сесия)
- [🌌 Бъдещи подобрения](#-бъдещи-подобрения)
- [🧾 Автор](#-автор)

---

# ✨ За проекта

**Eventy** е курсова задача по **Обектно-ориентирано програмиране**, реализирана като **C++23 CLI приложение**.

Проектът моделира система за:

- 📅 създаване и управление на събития;
- 🎟️ закупуване на билети;
- 🪑 избор на конкретни места при събития със схема;
- 🤝 кандидатстване за доброволчески дейности;
- 🧾 обработка на заявки;
- 🔔 изпращане и преглед на известия;
- 💰 управление на потребителски баланс;
- 💾 запис и зареждане на данни от файлове.

Системата поддържа различни типове потребители, различни видове събития и специален режим за управление на събитие от неговия създател.

---

# 🧭 Основна идея

```txt
Guest
  │
  ├── register
  ├── login
  └── view public events
          │
          ▼
Client
  │
  ├── create event
  │       │
  │       ▼
  │   publish request
  │       │
  │       ▼
  │     Admin
  │   approve / reject
  │
  ├── buy tickets
  ├── volunteer application
  ├── manage own events
  ├── receive notifications
  └── view history
```

---

# 🧙 Роли в системата

## 👻 Guest

Невписан потребител с ограничен достъп.

Може да:

- 📝 се регистрира;
- 🔐 влезе в акаунт;
- 📅 разглежда публикувани предстоящи събития;
- 🔎 преглежда детайли за публично събитие;
- 🪑 вижда схема на местата при платено събитие.

---

## 🧑 Client

Регистриран потребител.

Може да:

- 💰 управлява баланс;
- 🎟️ купува билети;
- 🪑 избира конкретни места;
- 🤝 кандидатства за доброволчески събития;
- 📅 създава събития;
- 🛠️ управлява собствените си събития;
- 🔔 получава известия;
- 📜 преглежда история на участия.

---

## 🛡️ Admin

Единственият администратор в системата.

Може да:

- 🧾 преглежда необработени заявки;
- ✅ одобрява заявки за публикуване;
- ❌ отхвърля заявки с причина;
- 🔔 уведомява потребители чрез системни известия.

Админ акаунт по подразбиране:

```txt
username: admin
password: admin123
```

---

# 🏛️ Видове събития

## 🎟️ Платени събития

Платените събития позволяват закупуване на билети.

Поддържат се два модела за капацитет.

---

## 🪑 Платено събитие с конкретни места

```txt
      1   2   3   4   5
A   [ ] [X] [ ] [ ] [X]
B   [ ] [ ] [ ] [X] [ ]
C   [X] [ ] [ ] [ ] [ ]
```

Легенда:

```txt
[ ] свободно място
[X] заето място
```

Примерна покупка:

```bash
buy-ticket 1 2 A1 A2
```

---

## 📦 Платено събитие с общ капацитет

```txt
Event: CppSummit
Capacity: 120
Sold:      37
Free:      83
```

Примерна покупка:

```bash
buy-ticket 1 3
```

---

## 🤝 Доброволчески събития

Доброволческите събития съдържат:

- 📝 описание на доброволческата дейност;
- 📬 списък с кандидатури;
- ✅ списък с одобрени участници;
- ❌ възможност за отказване на кандидатури;
- 🔒 възможност за затваряне на кандидатстването.

Пример:

```bash
volunteer-application 3 I_want_to_help_with_the_organization
```

---

# ⚙️ Функционалности

| Модул | Описание |
|---|---|
| 👤 Потребители | Регистрация, вход, изход, роли |
| 📅 Събития | Създаване, преглед, публикуване, отмяна |
| 🎟️ Билети | Покупка, капацитет, места |
| 🪑 Seating | Схема с редове и колони |
| 💰 Wallet | Баланс и плащания |
| 🤝 Доброволчество | Кандидатури, одобрение, отказ |
| 🧾 Заявки | Pending, Approved, Rejected |
| 🔔 Известия | Системни съобщения към потребителите |
| 📜 История | Минали участия и закупени билети |
| 💾 Storage | Запис и зареждане от файлове |
| 🧪 Validation | Проверка на входа преди промени |

---

# 🧬 Основни абстракции

```txt
User
├── Client
│   ├── Balance
│   ├── Tickets
│   ├── Event History
│   └── Created Events
│
└── Admin
    └── Publish Requests


Event
├── TicketedEvent
│   ├── General Capacity
│   ├── SeatingPlan
│   └── Tickets
│
└── VolunteerEvent
    ├── Activity Description
    ├── Applications
    ├── Approved Volunteers
    └── Closed / Open Applications


Request
├── EventPublishRequest
└── VolunteerApplicationRequest


Notification
└── Text + Receiver + Status
```

---

# 🛠️ CLI команди

## 👻 Guest команди

```bash
register <username> <password>
login <username> <password>
list-upcoming-events
event-info <event_id>
show-seating <event_id>
exit
```

---

## 🧑 Client команди

```bash
logout
wallet
add-balance <amount>

list-upcoming-events
event-info <event_id>
show-seating <event_id>

buy-ticket <event_id> <ticket_count> [seats...]
list-tickets
list-history

volunteer-application <event_id> <motivation>

create-ticketed-event <title> <date> <address> <price> <capacity>
create-seated-event <title> <date> <address> <price> <rows> <cols>
create-volunteer-event <title> <date> <address> <description>

list-my-events
enter-event <event_id>

list-notifications
exit
```

---

## 🛠️ Event Management режим

След избор на собствено събитие чрез:

```bash
enter-event <event_id>
```

потребителят влиза в специален режим за управление.

Команди:

```bash
event-info
cancel-event <reason>
list-volunteer-applications
approve-application <request_id>
reject-application <request_id>
close-volunteer-applications
list-participants
exit-event
```

---

## 🛡️ Admin команди

```bash
list-requests
approve-request <request_id>
reject-request <request_id> <reason>
logout
exit
```

---

# 🔔 Известия

Системата генерира известия при действия, които засягат потребител.

Примери:

```txt
[Unread] Your event "CppSummit" has been approved.
[Unread] Your volunteer application was rejected.
[Unread] You bought 2 tickets for "HackNight".
[Read]   Event "RiverCleanup" was cancelled.
```

Известия се създават при:

- ✅ одобрение на събитие;
- ❌ отказ на събитие;
- 🎟️ успешна покупка на билет;
- 🤝 резултат от доброволческа кандидатура;
- 🛑 отмяна на събитие;
- 💰 възстановяване на сума.

---

# 💾 Файлово съхранение

Eventy записва състоянието си във файлове и го възстановява при следващо стартиране.

Примерна структура:

```txt
data/
├── users.db
├── events.db
├── tickets.db
├── requests.db
├── notifications.db
└── metadata.db
```

Съхраняват се:

- 👤 потребители;
- 💰 баланси;
- 📅 събития;
- 🎟️ билети;
- 🧾 заявки;
- 🔔 известия;
- 📜 история на участията;
- 🪑 информация за заети места.

---

# 🧪 Валидация

Преди всяка промяна системата проверява входните данни.

При невалиден вход:

```txt
State is not changed.
No partial execution.
Clear error message is shown.
```

Покриват се случаи като:

- 🚫 дублирано потребителско име;
- 🔐 грешна парола;
- 🕳️ несъществуващо събитие;
- 🧑‍⚖️ опит за управление на чуждо събитие;
- 💸 недостатъчен баланс;
- 🪑 вече заето място;
- 📅 невалидна дата;
- 🎟️ невалиден брой билети;
- 🔒 кандидатстване след затворен прием;
- 🛑 отмяна на вече отменено събитие;
- 🧾 обработка на несъществуваща заявка.

---

# 🚀 Стартиране

## 1. Клониране

```bash
git clone https://github.com/<your-username>/<your-repository>.git
cd <your-repository>
```

---

## 2. Компилация

Пример с `make`:

```bash
make
```

Или директно с `g++`:

```bash
g++ -std=c++23 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o eventy
```

---

## 3. Стартиране

За Linux / macOS:

```bash
./eventy
```

За Windows:

```bash
eventy.exe
```

---

# 🗂️ Структура

```txt
Eventy/
├── include/
│   ├── Admin.h
│   ├── AppState.h
│   ├── Client.h
│   ├── CommandParser.h
│   ├── DateUtils.h
│   ├── Enums.h
│   ├── Event.h
│   ├── EventPrinter.h
│   ├── EventySystem.h
│   ├── IdGenerator.h
│   ├── Notification.h
│   ├── Request.h
│   ├── SeatingPlan.h
│   ├── StorageService.h
│   ├── StringUtils.h
│   ├── Ticket.h
│   ├── TicketedEvent.h
│   └── VolunteerEvent.h
│
├── src/
│   ├── Admin.cpp
│   ├── AppState.cpp
│   ├── Client.cpp
│   ├── CommandParser.cpp
│   ├── DateUtils.cpp
│   ├── Enums.cpp
│   ├── Event.cpp
│   ├── EventPrinter.cpp
│   ├── EventySystem.cpp
│   ├── IdGenerator.cpp
│   ├── Notification.cpp
│   ├── Request.cpp
│   ├── SeatingPlan.cpp
│   ├── StorageService.cpp
│   ├── StringUtils.cpp
│   ├── Ticket.cpp
│   ├── TicketedEvent.cpp
│   ├── VolunteerEvent.cpp
│   └── main.cpp
│
├── data/
│   └── .gitkeep
│
├── README.md
├── Makefile
└── .gitignore
```

---

# 🧩 Примерна сесия

```bash
> register nikola secret123
Successfully registered user: nikola.

> login nikola secret123
Welcome back, nikola.

> add-balance 100
Balance updated successfully.

> create-seated-event CppSummit 2026-05-21 Sofia 25 3 5
Event created. Publish request sent to admin.

> logout
Logged out.

> login admin admin123
Welcome back, admin.

> list-requests
[1] Publish request for event: CppSummit

> approve-request 1
Request approved. Event is now public.

> logout
Logged out.

> login nikola secret123
Welcome back, nikola.

> show-seating 1
      1   2   3   4   5
A   [ ] [ ] [ ] [ ] [ ]
B   [ ] [ ] [ ] [ ] [ ]
C   [ ] [ ] [ ] [ ] [ ]

> buy-ticket 1 2 A1 A2
Successfully bought 2 tickets.

> list-tickets
Ticket #1 | CppSummit | Seat A1
Ticket #2 | CppSummit | Seat A2

> wallet
Balance: 50
```

---

# 🏰 Архитектура

Проектът следва OOP принципи:

- 🧱 **инкапсулация** на данните;
- 🧬 **наследяване** при потребители и събития;
- 🎭 **полиморфизъм** при различните типове събития;
- 🧼 **валидация преди промяна**;
- 🧭 **ясно разделяне на отговорности**;
- 🪄 **лесна разширяемост**;
- 💾 **отделен storage layer**;
- 🧰 **helper класове за parsing, дати и id-та**.

```txt
Command Input
     │
     ▼
CommandParser
     │
     ▼
EventySystem
     │
     ▼
Validation Layer
     │
     ▼
Domain Objects
     │
     ▼
StorageService
     │
     ▼
File Storage
```

---

# 🧠 C++23 използвани идеи

Проектът използва стандартен C++23 без външни библиотеки.

Включени са:

- `std::string`
- `std::vector`
- `std::optional`
- `std::expected`
- `std::stringstream`
- файлови потоци
- класове и енкапсулация
- наследяване
- полиморфизъм
- разделна компилация

---

# 🌌 Бъдещи подобрения

- 🌈 цветен terminal output;
- 🧾 по-богат storage формат;
- 🔍 търсене и филтриране на събития;
- 🏷️ категории за събития;
- 📊 статистики за организаторите;
- 🎫 export на билети;
- 🧪 unit tests;
- 🧠 command aliases;
- 🪟 интерактивно меню върху CLI командите.

---

# 🧾 Автор

<div align="center">

## Никола Топалов

<img src="https://img.shields.io/badge/GitHub-@nikolatopalov10-181717?style=for-the-badge&logo=github">
<img src="https://img.shields.io/badge/Project-Eventy-6A11CB?style=for-the-badge">

</div>

---

<p align="center">
  <b>Eventy</b>
  <br>
  <i>Where events enter the terminal and leave as organized reality.</i>
</p>

<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=0:2c5364,50:203a43,100:0f2027&height=140&section=footer" />
</p>
