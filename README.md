<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=0:0f2027,50:203a43,100:2c5364&height=230&section=header&text=Eventy&fontSize=70&fontAlignY=35&fontColor=ffffff&animation=fadeIn&desc=CLI%20система%20за%20събития,%20билети%20и%20доброволчество&descAlignY=57&descSize=17" />
</p>

<div align="center">

  <img src="https://readme-typing-svg.herokuapp.com?font=JetBrains+Mono&size=22&duration=2500&pause=800&color=6A11CB&center=true&vCenter=true&width=760&lines=Управление+на+събития+през+CLI;Билети,+места+и+баланс;Доброволчество+и+кандидатури;Админ+одобрения+и+заявки;OOP+архитектура+с+магически+ред" />

  <br><br>

  <img src="https://img.shields.io/badge/C++-OOP%20Project-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
  <img src="https://img.shields.io/badge/CLI-Event%20Management-6A11CB?style=for-the-badge&logo=gnubash&logoColor=white">
  <img src="https://img.shields.io/badge/FMI-Sofia%20University-2C5364?style=for-the-badge">
  <img src="https://img.shields.io/badge/Status-In%20Development-FF9F1C?style=for-the-badge">

</div>

<br>

<p align="center">
  <b>Eventy</b> е конзолно приложение за управление на събития, закупуване на билети, кандидатстване за доброволчество и обработка на заявки чрез администраторски контрол.
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
- [🪄 CLI команди](#-cli-команди)
- [🔔 Известия](#-известия)
- [💾 Файлово съхранение](#-файлово-съхранение)
- [🧪 Валидация](#-валидация)
- [🚀 Стартиране](#-стартиране)
- [🗂️ Структура](#️-структура)
- [🏰 Архитектура](#-архитектура)
- [🌌 Бъдещи подобрения](#-бъдещи-подобрения)
- [🧾 Автор](#-автор)

---

# ✨ За проекта

**Eventy** е курсова задача по **Обектно-ориентирано програмиране**, реализирана като **C++ CLI приложение**.

Проектът моделира система за:

- 📅 създаване и управление на събития;
- 🎟️ закупуване на билети;
- 🤝 кандидатстване за доброволчески дейности;
- 🧾 обработка на заявки;
- 🔔 изпращане на известия;
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
  └── receive notifications
```

---

# 🧙 Роли в системата

## 👻 Guest

Невписан потребител с ограничен достъп.

Може да:

- 📝 се регистрира;
- 🔐 влезе в акаунт;
- 📅 разглежда публикувани предстоящи събития;
- 🔎 преглежда детайли за събитие;
- 🪑 вижда схема на местата при платено събитие.

---

## 🧑 Client

Регистриран потребител.

Може да:

- 💰 управлява баланс;
- 🎟️ купува билети;
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
- 🔔 изпраща системни известия чрез действията си.

---

# 🏛️ Видове събития

## 🎟️ Платени събития

Платените събития позволяват закупуване на билети.

Те могат да използват два модела за капацитет.

### 🪑 Модел с конкретни места

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

Потребителят избира конкретни координати, например:

```bash
buy-ticket 1 2 <A,1> <A,2>
```

---

### 📦 Модел с общ капацитет

```txt
Event: C++ Summit
Capacity: 120
Sold:      37
Free:      83
```

Потребителят избира само брой билети:

```bash
buy-ticket 1 3
```

---

## 🤝 Доброволчески събития

Доброволческите събития съдържат:

- 📝 описание на доброволческата дейност;
- 📬 списък с кандидатури;
- ✅ списък с одобрени участници;
- 🔒 възможност за затваряне на кандидатстването.

Пример:

```bash
volunteer-application 3 "Искам да помогна с организацията и посрещането на участниците."
```

---

# ⚙️ Функционалности

| Модул | Описание |
|---|---|
| 👤 Потребители | Регистрация, вход, изход, роли |
| 📅 Събития | Създаване, преглед, публикуване, отмяна |
| 🎟️ Билети | Покупка, места, капацитет |
| 💰 Wallet | Баланс и плащания |
| 🤝 Доброволчество | Кандидатури, одобрение, отказ |
| 🧾 Заявки | Pending, Approved, Rejected |
| 🔔 Известия | Системни съобщения към потребителите |
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
│   ├── SeatingModel
│   └── Tickets
│
└── VolunteerEvent
    ├── Activity Description
    ├── Applications
    └── Volunteers


Request
├── EventPublishRequest
└── VolunteerApplicationRequest


Notification
└── Text + DateTime + Status
```

---

# 🪄 CLI команди

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
buy-ticket <event_id> <ticket_count> [<r1, c1>, …, <rn, cn>]
volunteer-application <event_id> <motivation>
list-tickets
list-history
create-event <type> <title> <date> <address> [options]
list-my-events
enter-event <event_id>
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
[Unread] Your event "C++ Summit" has been approved.
[Unread] Your volunteer application was rejected.
[Unread] You bought 2 tickets for "Hack Night".
[Read]   Event "River Cleanup" was cancelled.
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
├── users.txt
├── events.txt
├── tickets.txt
├── requests.txt
├── notifications.txt
└── metadata.txt
```

Съхраняват се:

- 👤 потребители;
- 💰 баланси;
- 📅 събития;
- 🎟️ билети;
- 🧾 заявки;
- 🔔 известия;
- 📜 история на участията.

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
git clone https://github.com/<your-username>/eventy.git
cd eventy
```

## 2. Компилация

Пример с `g++`:

```bash
g++ -std=c++17 src/*.cpp -o eventy
```

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
├── src/
│   ├── main.cpp
│   ├── core/
│   ├── users/
│   ├── events/
│   ├── tickets/
│   ├── requests/
│   ├── notifications/
│   └── storage/
│
├── include/
│   ├── User.hpp
│   ├── Client.hpp
│   ├── Admin.hpp
│   ├── Event.hpp
│   ├── TicketedEvent.hpp
│   ├── VolunteerEvent.hpp
│   ├── Request.hpp
│   └── Notification.hpp
│
├── data/
├── docs/
├── README.md
└── Makefile
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

> list-upcoming-events
[1] C++ Summit | 2026-05-21 | Sofia | Ticketed
[2] River Cleanup | 2026-06-03 | Plovdiv | Volunteer

> event-info 1
Title: C++ Summit
Type: Ticketed
Price: 25
Seats: 42 available

> buy-ticket 1 2 <A,1> <A,2>
Successfully bought 2 tickets.

> list-tickets
Ticket #1 | C++ Summit | Seat A1
Ticket #2 | C++ Summit | Seat A2
```

---

# 🏰 Архитектура

Проектът следва OOP принципи:

- 🧱 **инкапсулация** на данните;
- 🧬 **наследяване** при потребители и събития;
- 🎭 **полиморфизъм** при различните типове събития;
- 🧼 **валидация преди промяна**;
- 🧭 **ясно разделяне на отговорности**;
- 🪄 **лесна разширяемост**.

```txt
Command Input
     │
     ▼
Command Parser
     │
     ▼
Validation Layer
     │
     ▼
Service / Manager Layer
     │
     ▼
Domain Objects
     │
     ▼
File Storage
```

---

# 🌌 Бъдещи подобрения

- 🌈 цветен terminal output;
- 🧾 JSON storage;
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
