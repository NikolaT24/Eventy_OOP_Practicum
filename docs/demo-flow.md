# 🧪 Eventy Demo Flow

Този файл съдържа примерен сценарий за тестване на основните функционалности в **Eventy**.

---

# 🛡️ Admin Account

Системата има предварително създаден администраторски акаунт:

```txt
username: admin
password: admin123
```

---

# 1. Register And Login As Client

Създаваме нов потребител и влизаме в системата.

```bash
register nikola secret123
login nikola secret123
```

Очакван резултат:

```txt
Successfully registered user.
Welcome back, nikola.
```

---

# 2. Add Balance

Добавяме средства към потребителския баланс.

```bash
add-balance 100
wallet
```

Очакван резултат:

```txt
Balance updated successfully.
Balance: 100
```

---

# 3. Create A Seated Ticketed Event

Създаваме платено събитие със схема от редове и колони.

```bash
create-seated-event CppSummit 2026-05-21 Sofia 25 3 5
```

Очакван резултат:

```txt
Event created. Publish request sent to admin.
```

Проверяваме собствените събития:

```bash
list-my-events
```

Очакван резултат:

```txt
[1] CppSummit | 2026-05-21 | Ticketed | Pending
```

Излизаме от client акаунта:

```bash
logout
```

---

# 4. Approve Event As Admin

Влизаме като администратор.

```bash
login admin admin123
```

Преглеждаме заявките:

```bash
list-requests
```

Очакван резултат:

```txt
[1] Publish request for event: CppSummit
```

Одобряваме заявката:

```bash
approve-request 1
```

Очакван резултат:

```txt
Request approved. Event is now public.
```

Излизаме от admin акаунта:

```bash
logout
```

---

# 5. View Published Event

Влизаме отново като client.

```bash
login nikola secret123
```

Преглеждаме публичните събития:

```bash
list-upcoming-events
```

Очакван резултат:

```txt
[1] CppSummit | 2026-05-21 | Sofia | Ticketed
```

Преглеждаме детайли за събитието:

```bash
event-info 1
```

---

# 6. Show Seating

Показваме схемата на местата.

```bash
show-seating 1
```

Очакван примерен резултат:

```txt
      1   2   3   4   5
A   [ ] [ ] [ ] [ ] [ ]
B   [ ] [ ] [ ] [ ] [ ]
C   [ ] [ ] [ ] [ ] [ ]
```

---

# 7. Buy Tickets

Купуваме 2 билета за конкретни места.

```bash
buy-ticket 1 2 A1 A2
```

Очакван резултат:

```txt
Successfully bought 2 tickets.
```

Проверяваме билетите:

```bash
list-tickets
```

Очакван резултат:

```txt
Ticket #1 | CppSummit | Seat A1
Ticket #2 | CppSummit | Seat A2
```

Проверяваме баланса:

```bash
wallet
```

Очакван резултат:

```txt
Balance: 50
```

---

# 8. Check Notifications

Показваме известията.

```bash
list-notifications
```

Очакван резултат:

```txt
[Unread] Your event "CppSummit" has been approved.
[Unread] You bought 2 tickets for "CppSummit".
```

---

# 9. Create Volunteer Event

Създаваме доброволческо събитие.

```bash
create-volunteer-event RiverCleanup 2026-06-03 Plovdiv Cleaning_the_river_area
logout
```

Влизаме като admin и го одобряваме:

```bash
login admin admin123
list-requests
approve-request 2
logout
```

---

# 10. Apply For Volunteer Event

Влизаме като друг client.

```bash
register maria pass123
login maria pass123
volunteer-application 2 I_want_to_help_with_the_cleanup
logout
```

---

# 11. Manage Volunteer Applications

Влизаме като създателя на събитието.

```bash
login nikola secret123
enter-event 2
list-volunteer-applications
approve-application 3
list-participants
exit-event
```

Очакван резултат:

```txt
Application approved.
maria
```

---

# 12. Exit And Save

Затваряме приложението.

```bash
exit
```

Очакван резултат:

```txt
Data saved.
Goodbye.
```

---

# ✅ Covered Features

Този demo flow покрива:

- регистрация;
- вход и изход;
- баланс;
- създаване на събитие;
- admin approval;
- публични събития;
- схема на места;
- закупуване на билети;
- известия;
- доброволчески събития;
- кандидатури;
- event management режим;
- запис на данни при изход.
