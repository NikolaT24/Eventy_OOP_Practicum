# 🪄 Eventy CLI Commands

Този файл описва основните команди в **Eventy** и режимите, в които могат да се използват.

---

# 👻 Guest Commands

Команди, достъпни за невписан потребител.

```bash
register <username> <password>
```

Регистрира нов потребител.

```bash
login <username> <password>
```

Вписва потребител в системата.

```bash
list-upcoming-events
```

Показва всички публикувани предстоящи събития.

```bash
event-info <event_id>
```

Показва детайлна информация за конкретно събитие.

```bash
show-seating <event_id>
```

Показва схема на местата за събитие със seating модел.

```bash
exit
```

Записва текущите данни и затваря приложението.

---

# 🧑 Client Commands

Команди, достъпни за регистриран потребител.

```bash
logout
```

Излиза от текущия акаунт.

```bash
wallet
```

Показва текущия баланс на потребителя.

```bash
add-balance <amount>
```

Добавя средства към баланса.

```bash
list-upcoming-events
```

Показва всички публикувани предстоящи събития.

```bash
event-info <event_id>
```

Показва детайлна информация за конкретно събитие.

```bash
show-seating <event_id>
```

Показва схема на местата за платено събитие с конкретни места.

```bash
buy-ticket <event_id> <ticket_count> [seats...]
```

Купува билети за платено събитие.

Примери:

```bash
buy-ticket 1 2
buy-ticket 1 2 A1 A2
```

```bash
list-tickets
```

Показва всички билети на текущия потребител.

```bash
list-history
```

Показва история на участията.

```bash
volunteer-application <event_id> <motivation>
```

Изпраща кандидатура за доброволческо събитие.

Пример:

```bash
volunteer-application 3 I_want_to_help_with_the_organization
```

```bash
create-ticketed-event <title> <date> <address> <price> <capacity>
```

Създава платено събитие с общ капацитет.

Пример:

```bash
create-ticketed-event CppSummit 2026-05-21 Sofia 25 100
```

```bash
create-seated-event <title> <date> <address> <price> <rows> <cols>
```

Създава платено събитие със схема от редове и колони.

Пример:

```bash
create-seated-event CppSummit 2026-05-21 Sofia 25 3 5
```

```bash
create-volunteer-event <title> <date> <address> <description>
```

Създава доброволческо събитие.

Пример:

```bash
create-volunteer-event RiverCleanup 2026-06-03 Plovdiv Cleaning_the_river_area
```

```bash
list-my-events
```

Показва събитията, създадени от текущия потребител.

```bash
enter-event <event_id>
```

Влиза в режим за управление на конкретно събитие.

```bash
list-notifications
```

Показва известията на текущия потребител.

```bash
exit
```

Записва текущите данни и затваря приложението.

---

# 🛠️ Event Management Mode

Този режим се активира след:

```bash
enter-event <event_id>
```

В него потребителят управлява конкретно събитие, което е създал.

```bash
event-info
```

Показва информация за текущо избраното събитие.

```bash
cancel-event <reason>
```

Отменя текущото събитие.

Пример:

```bash
cancel-event Organizer_unavailable
```

```bash
list-volunteer-applications
```

Показва кандидатурите за доброволчество към текущото събитие.

```bash
approve-application <request_id>
```

Одобрява кандидатура за доброволчество.

```bash
reject-application <request_id>
```

Отхвърля кандидатура за доброволчество.

```bash
close-volunteer-applications
```

Затваря приемането на нови доброволчески кандидатури.

```bash
list-participants
```

Показва участниците в текущото събитие.

```bash
exit-event
```

Излиза от режима за управление на събитие.

---

# 🛡️ Admin Commands

Команди, достъпни само за администратора.

Админ акаунт по подразбиране:

```txt
username: admin
password: admin123
```

```bash
list-requests
```

Показва необработените заявки за публикуване.

```bash
approve-request <request_id>
```

Одобрява заявка за публикуване на събитие.

```bash
reject-request <request_id> <reason>
```

Отхвърля заявка за публикуване на събитие с причина.

Пример:

```bash
reject-request 2 Missing_event_information
```

```bash
logout
```

Излиза от администраторския акаунт.

```bash
exit
```

Записва текущите данни и затваря приложението.

---

# 🧪 Notes

- При невалидна команда системата показва ясно съобщение за грешка.
- При невалидни параметри състоянието на системата не се променя.
- Събитията стават публични само след одобрение от администратор.
- Само създателят на събитие може да го управлява.
- Само администраторът може да обработва заявки за публикуване.
