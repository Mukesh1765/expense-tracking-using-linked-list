# 💰 Family Expense Tracking System (Doubly Linked List Version)

This is a **Family Expense Tracking System** built using **C language** with **Doubly Linked Lists** and **File Handling** for better dynamic memory management and persistence. It allows you to track individual and family expenses across multiple categories like rent, utilities, groceries, etc.

---

## 📝 Problem Statement

> Implement an expense tracking system to track family expenses. Each family consists of users, and each user can log categorized expenses. The system should support insertion, deletion, update, and reporting operations.

---

## ⚙️ Functional Requirements

1. **AddUser()**  
   Adds a user. Stores users sorted by `UserID`.

2. **AddExpense()**  
   Adds an expense for a user. Sorted by `UserID`, then `ExpenseID`.

3. **CreateFamily()**  
   Creates a family with 1 to 4 users. Computes total family income and expenses.

4. **Update_or_delete_individual_Family_details()**  
   Updates or deletes user/family. Deletes family if only one user remains.

5. **Update_delete_expense()**  
   Updates or deletes expense and updates dependent records.

6. **Get_total_expense()**  
   Displays total family expense and compares it to income.

7. **Get_categorical_expense(category)**  
   Displays category-wise total and sorted individual contributions.

8. **Get_highest_expense_day()**  
   Prints date with highest total expense.

9. **Get_individual_expense(UserID)**  
   Shows total and category-wise individual expenses in descending order.

---

## 🧾 Data Structures Used

### 👤 Individual
```c
typedef struct Individual {
    int userID;
    char userName[50];
    float income;
    struct Individual *prev, *next;
} Individual;
```

### 💸 Expense
```c
typedef struct Expense {
    int expenseID;
    int userID;
    char category[20]; // Rent, Utility, Grocery, etc.
    float amount;
    char date[11]; // YYYY-MM-DD
    struct Expense *prev, *next;
} Expense;
```

### 🏠 Family
```c
typedef struct Family {
    int familyID;
    char familyName[50];
    int individualIDs[4]; // Max 4 members
    int memberCount;
    float totalIncome;
    float totalExpense;
    struct Family *prev, *next;
} Family;
```

---

## 💾 File Handling

Data persistence is handled using basic C file Inputs. All user, expense, and family information is read from`.txt` or `.dat` files so your data remains safe across program runs.

---

## 📈 Features

- CLI-based user interface
- Sorted insertion for all lists
- Dynamic memory with doubly linked lists
- Auto-updating family totals
- Full CRUD operations
- Category and date-based reporting
- Individual contribution breakdowns
- Input validations in every function

---

## 📦 Future Scope

- GUI with GTK or Qt
- Monthly budget targets
- Expense charts and graphs
- Export reports as CSV or PDF

---

## 🤝 Contributions

Contributions are welcome! Fork the repo, make your changes, and open a pull request. You can also open issues for feature requests or bugs.

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

