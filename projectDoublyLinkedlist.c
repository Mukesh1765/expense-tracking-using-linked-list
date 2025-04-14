#include<stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX_USERS 1000
#define MAX_EXPENSES 1000
#define MAX_NO_OF_FAMILIES 100
#define MAX_NO_OF_USERS 4
#define MAX_EXPENSE_CATEGORY_SIZE 15
#define TOTAL_DAYS 10
#define MAX_NAME_LENGTH 100

typedef enum {SUCCESS, FAILURE} statusCode;
typedef enum {false, true} bool;

typedef struct {
    unsigned int date;
    unsigned int month;
    unsigned int year;
} Date;

typedef struct users {
    //data
    int user_ID;//ranges from 1 to 1000
    char user_name[MAX_NAME_LENGTH];
    float income;
    //link
    struct users *prev;
    struct users *next;
} userNode;

typedef struct expense {
    //data
    int expense_ID;
    int user_ID;
    char expense_category[MAX_EXPENSE_CATEGORY_SIZE]; // rent, utility, grocery, stationary, leisure;
    float expense_amount;
    Date date_of_expense;
    //link
    struct expense *prev;
    struct expense *next;
} expenseNode;

typedef struct family {
    //data
    int family_ID;
    char family_name[MAX_NAME_LENGTH];
    int total_members;
    int family_members_user_id[MAX_NO_OF_USERS];
    float total_family_income;
    float total_family_expense;
    //link
    struct family *prev;
    struct family *next;
} familyNode;

typedef struct daily_expense { 
    Date date;
    float total_expense;
} dailyExpense;

typedef struct {
    int user_id;
    float expense;
} familyUserExpense;

typedef struct category_expense {
    char category_name[15];
    float expense;
} cat_exp;

expenseNode* deleteExpenseNode (expenseNode *head, expenseNode *ntd);

userNode* createUserNode (int user_ID, char user_name[], float income) {
    userNode* newNode = (userNode*) malloc(sizeof(userNode));
    if (newNode != NULL) {
        newNode->user_ID = user_ID;
        strcpy(newNode->user_name, user_name);
        newNode->income = income;
        newNode->next = NULL;
        newNode->prev = NULL;
    }
    return newNode;
}

expenseNode* createExpenseNode (int expense_ID, int user_ID, char category[], float amount, Date date_of_expense) {
    expenseNode* newNode = (expenseNode*) malloc(sizeof(expenseNode));
    if (newNode != NULL) {
        newNode->expense_ID = expense_ID;
        newNode->user_ID = user_ID;
        strcpy(newNode->expense_category, category);
        newNode->expense_amount = amount;
        newNode -> date_of_expense = date_of_expense;

        newNode->next = NULL;
        newNode->prev = NULL;
    }
    return newNode;
}

familyNode* createFamilyNode (int family_ID, char family_name[], int members, int member_IDs[]) {
    familyNode* newNode = (familyNode*) malloc(sizeof(familyNode));
    if (newNode != NULL) {
        newNode -> family_ID = family_ID;
        strcpy(newNode -> family_name, family_name);
        newNode -> total_members = (members > 4) ? 4 : members; // Max 4 members per family
        int i;
        for (i = 0; i < newNode -> total_members; i++) {
            newNode -> family_members_user_id[i] = member_IDs[i];
        }
        for (; i < MAX_NO_OF_USERS; i++) {
            newNode -> family_members_user_id[i] = -1;
        }
        newNode -> next = NULL;
        newNode -> prev = NULL;
    }
    return newNode;
}

Date createDate (unsigned int date, unsigned int month, unsigned int year) {
    Date result = {date, month, year};
    return result;
}

int userNodelength (userNode *head) {
    userNode *temp = head;
    int count = 0;
    if (temp != NULL) {
        while (temp != NULL) {
            count = count + 1;
            temp = temp -> next;
        }
    }
    return count;
}

int expenseNodelength (expenseNode *head) {
    expenseNode *temp = head;
    int count = 0;
    if (temp != NULL) {
        while (temp != NULL) {
            count = count + 1;
            temp = temp -> next;
        }
    }
    return count;
}

int familyNodelength (familyNode *head) {
    familyNode *temp = head;
    int count = 0;
    if (temp != NULL) {
        while (temp != NULL) {
            count = count + 1;
            temp = temp -> next;
        }
    }
    return count;
}

void capitalize(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - ('a' - 'A');
        }
    }
}

//users
bool doesUserExist (userNode *head, int user_id) {
    bool found = false;
    userNode *user_temp = head;
    while ((user_temp != NULL) && (!found)) {
        if (user_temp -> user_ID == user_id) {
            found = true;
        } else {
            user_temp = user_temp -> next;
        }
    }
    return found;
}

userNode* mergeUsers (userNode* head1, userNode* head2) {
    userNode* result = NULL;
    userNode* temp = NULL;
    while (head1 != NULL && head2 != NULL) {
        if (head1->user_ID <= head2->user_ID) {
            if (result == NULL) {
                result = temp = head1;
            } else {
                temp->next = head1;
                head1->prev = temp;
                temp = temp->next;
            }
            head1 = head1->next;
        } else {
            if (result == NULL) {
                result = temp = head2;
            } else {
                temp->next = head2;
                head2->prev = temp;
                temp = temp->next;
            }
            head2 = head2->next;
        }
    }
    if (head1 != NULL) {
        temp->next = head1;
        head1->prev = temp;
    }
    if (head2 != NULL) {
        temp->next = head2;
        head2->prev = temp;
    }

    return result;
}

userNode* divideUserList (userNode* head) {
    userNode* slow = head;
    userNode* fast = head;
    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        if (fast) {
            slow = slow->next;
        }
    }
    userNode* middleNext = slow->next;
    slow->next = NULL;
    if (middleNext) {
        middleNext->prev = NULL;
    }
    return middleNext;
}

userNode* mergeSortUser (userNode* head) {
    if (head != NULL && head -> next != NULL) {
        userNode* middle = divideUserList(head);
        userNode* left = mergeSortUser(head);
        userNode* right = mergeSortUser(middle);
        head = mergeUsers(left, right);
    }
    return head;
}

userNode* insertBeforeUser(userNode *head, userNode *before, userNode *newNode) {
    userNode *temp, *prevNode;
    if (before == NULL) {
        if (head == NULL) {
            head = newNode;
        } else {
            temp = head;
            while (temp -> next != NULL) {
                temp = temp -> next;
            }
            temp -> next = newNode;
            newNode -> prev = temp;
            newNode -> next = NULL;
        }
    } else {
        newNode -> next = before;
        newNode -> prev = before -> prev;
        before -> prev = newNode;
        prevNode = newNode -> prev;
        if (prevNode != NULL) {
            prevNode -> next = newNode;
        } else {
            head = newNode;
        }
    }
    return head;
}

userNode* insertCorrectPlaceUser(userNode *user_head, userNode *newNode) {
    userNode *curr = user_head;
    while ((curr != NULL) && (curr -> user_ID <= newNode -> user_ID)) {
        curr = curr -> next;
    }
    user_head = insertBeforeUser(user_head, curr, newNode);
    return user_head;
}

bool doesFamilyIdExist (familyNode *head, int famid) {
    bool found = false;
    familyNode *temp = head;
    while (temp != NULL && !found) {
        if (temp -> family_ID == famid) {
            found = true;
        } else {
            temp = temp -> next;
        }
    }
    return found;
}

familyNode* getFamilyNode (familyNode *head, int fam_id) {
    familyNode *temp = head;
    while (temp != NULL && temp -> family_ID != fam_id) {
        temp = temp -> next;
    }
    return temp;
}

void addUserToFamily(familyNode *fhead, int user_id, float income) {
    int fam_id;
    printf("enter family id to which you want to add this user : ");
    scanf("%d", &fam_id);
    bool exist = doesFamilyIdExist(fhead, fam_id);
    if (exist) {
        familyNode *temp = getFamilyNode(fhead, fam_id);
        if (temp -> total_members == 4) {
            printf("Max number of users are in family\n");
        } else {
            temp -> total_members += 1;
            int i = 0;
            while (temp -> family_members_user_id[i] != -1) {
                i++;
            }
            temp -> family_members_user_id[i] = user_id;
            temp -> total_family_income += income;
        }
    } else {
        printf("family id %d does not exist\n");
    }
}

statusCode AddUser (userNode **user_head, familyNode **fhead) {
    statusCode sc = SUCCESS;
    userNode *user_temp = *user_head;
    userNode *new_user;
    int users_count = userNodelength(*user_head);
    if (users_count >= MAX_USERS || users_count < 0) {
        printf("User list is out of bounds.\n");
        sc = FAILURE;
    } else {
        printf("enter the details\n");
        int user_id;
        float income;
        char name[100];
        printf("enter user_id : ");
        scanf("%d",&user_id);
        while(user_id > 1000 || user_id < 1){
            printf("wrong input. user id cannot exceed 1000 and cannot be less than 1.\n");
            printf("enter user_id : ");
            scanf("%d",&user_id);
        }
        bool does_exist = doesUserExist(*user_head, user_id);
        if (does_exist) {
            printf("user id already exists.\n");
        } else {
            printf("enter income of user : ");
            scanf("%f", &income);
            while(income < 0){
                printf("wrong input. Income is never negative.\n");
                printf("enter income of user : ");
                scanf("%f", &income);
            }
            printf("enter name of individual user : ");
            scanf("%s", name);
            while(name[0] == '\0'){
                printf("wrong input. please enter your name.\n");
                printf("enter name of individual user : ");
                fgets(name, 100, stdin);
            }
            new_user = createUserNode(user_id, name, income);
            if (new_user == NULL) {
                sc = FAILURE;
            } else {
                *user_head = insertCorrectPlaceUser(*user_head, new_user);
            }
            if (sc == SUCCESS) {
                char c;
                printf("do you want to add this user to a family? enter y to add, n to continue : ");
                do {
                    scanf("%c", &c);
                } while (c != 'y' && c != 'n' && c != 'Y' && c != 'N');
                if (c == 'y' || c == 'Y') {
                    addUserToFamily(*fhead, user_id, income);
                }
            }
        }
    }
    return sc;
}

//expenses
bool doesExpenseIdExist (expenseNode *head, int eId) {
    bool found = false;
    expenseNode *temp = head;
    while (temp && !found) {
        if (temp -> expense_ID == eId) {
            found = true;
        } else {
            temp = temp -> next;
        }
    }
    return found;
}

expenseNode* mergeExpenses (expenseNode* left, expenseNode* right) {
    expenseNode* head = NULL;
    expenseNode* tail = NULL;
    
    while (left && right) {
        expenseNode* selected = NULL;
        if (left -> user_ID < right -> user_ID || 
           (left -> user_ID == right -> user_ID && left -> expense_ID < right -> expense_ID)) {
            selected = left;
            left = left -> next;
        } else {
            selected = right;
            right = right->next;
        }
        if (!head) {
            head = selected;
            tail = selected;
        } else {
            tail -> next = selected;
            selected -> prev = tail;
            tail = selected;
        }
    }
    if (left) {
        if (!head) {
            head = left;
        } else { 
            tail -> next = left;
        }
        left -> prev = tail;
    } else if (right) {
        if (!head) {
            head = right;
        } else {
            tail -> next = right;
        }
        right -> prev = tail;
    }
    if (head) {
        head -> prev = NULL;
    }
    return head;
}

expenseNode* divideExpenseList (expenseNode* head) {
    expenseNode* secondHalf = NULL;
    if (head && head -> next) {
        expenseNode* slow = head;
        expenseNode* fast = head;
        while (fast -> next && fast -> next -> next) {
            slow = slow -> next;
            fast = fast -> next -> next;
        }
        secondHalf = slow -> next;
        slow -> next = NULL;
        if (secondHalf) {
            secondHalf -> prev = NULL;
        }
    }
    return secondHalf;
}

expenseNode* mergeSortExpense (expenseNode* head) {
    if (head && head -> next) {
        expenseNode* middle = divideExpenseList(head);
        head = mergeSortExpense(head);
        middle = mergeSortExpense(middle);
        head = mergeExpenses(head, middle);
    }
    return head;
}

expenseNode* insertBeforeExpense (expenseNode *head, expenseNode *before, expenseNode *newNode) {
    expenseNode *temp, *prevNode;
    if (before == NULL) {
        if (head == NULL) {
            head = newNode;
        } else {
            temp = head;
            while (temp -> next != NULL) {
                temp = temp -> next;
            }
            temp -> next = newNode;
            newNode -> prev = temp;
            newNode -> next = NULL;
        }
    } else {
        newNode -> next = before;
        newNode -> prev = before -> prev;
        before -> prev = newNode;
        prevNode = newNode -> prev;
        if (prevNode != NULL) {
            prevNode -> next = newNode;
        } else {
            head = newNode;
        }
    }
    return head;
}

expenseNode* insertAfterExpense (expenseNode *head, expenseNode *after, expenseNode *newNode) {
    if (head == NULL) {
        if (after == NULL) {
            head = newNode;
        }
    } else {
        if (after == NULL) {
            newNode -> next = head;
            head -> prev = newNode;
            newNode -> prev = NULL;
            head = newNode;
        } else {
            newNode -> next = after -> next;
            newNode -> prev = after;
            after -> next = newNode;
            expenseNode *nextNode = newNode -> next;
            if (nextNode != NULL) {
                nextNode -> prev = newNode;
            }
        }
    }
    return head;
}

expenseNode* insertCorrectPlaceExpense (expenseNode *head, expenseNode *newNode) {
    expenseNode *temp = head;
    int reqUserid = newNode -> user_ID;
    bool found = false;
    expenseNode *prevn = NULL;
    while ((temp != NULL) && (temp -> user_ID <= reqUserid)) {
        if (temp -> user_ID == reqUserid) {
            if (temp -> next == NULL) {
                prevn = temp;
            }
            found = true;
        }
        temp = temp -> next;
    }
    if (found) {
        //sort according to expesneid while adding;
        int reqexpenseid = newNode -> expense_ID;
        if (temp) {
            prevn = temp -> prev;
        }
        while (prevn != NULL && prevn -> expense_ID > reqexpenseid && prevn -> user_ID == reqUserid) {
            prevn = prevn -> prev;
        }
        head = insertAfterExpense(head, prevn, newNode);
    } else {
        //sort based on userids
        head = insertBeforeExpense(head, temp, newNode);
    }
    return head;
}

statusCode AddExpense (expenseNode **expenseHead, userNode **uhead) {
    statusCode sc = SUCCESS;
    int size = expenseNodelength(*expenseHead);
    if (size < 0 || size > 1000) {
        printf("expense limit reached.\n");
        sc = FAILURE;
    } else {
        int user_id, expense_id;
        float expense_amount;
        char expense_category[MAX_EXPENSE_CATEGORY_SIZE];
        Date date;
        printf("Enter expense ID for expense: ");
        scanf("%d", &expense_id);
        while(expense_id > 1000 || expense_id < 1){
            printf("wrong input. expense id cannot exceed 1000 and cannot be less than 1.\n");
            printf("enter expense_id : ");
            scanf("%d",&expense_id);
        }
        bool does_exist = doesExpenseIdExist(*expenseHead, expense_id);
        if (does_exist) {
            printf("expense id already exists.\n");
            sc = FAILURE;
        } else {
            printf("Enter user ID for expense: ");
            scanf("%d", &user_id);
            bool is_exist = doesUserExist(*uhead, user_id);
            if (!is_exist) {
                printf("given user doesnot exist.\n");
            } else {
                printf("Enter expense amount: ");
                scanf("%f", &expense_amount);
                while(expense_amount < 0){
                    printf("wrong input. expense can never be negative.\n");
                    printf("enter income again : ");
                    scanf("%f", &expense_amount);
                }
                printf("Enter expense category: ");
                scanf("%14s", expense_category);
                while(strcmp(expense_category, "rent") != 0 && strcmp(expense_category, "utility") != 0 && strcmp(expense_category, "stationary") != 0 && strcmp(expense_category, "grocery") != 0 && strcmp(expense_category, "leisure") != 0){
                    printf("enter only rent or utility or stationary or grocery or leisure\n");
                    printf("Enter expense category again : ");
                    scanf("%14s", expense_category);         
                }
                capitalize(expense_category);
                printf("Enter date of expense (DD MM YYYY): ");
                scanf("%u %u %u", &date.date, &date.month, &date.year);
                while (date.date > 10 || date.date <= 0) {
                    printf("enter only 1 - 10\n");
                    printf("enter date again: ");
                    scanf("%u", &date.date);
                }
                while (date.month > 12 || date.month <= 0) {
                    printf("enter only 1 - 12\n");
                    printf("enter month again: ");
                    scanf("%u", &date.month);
                }
                while (date.year > 2025 || date.year <= 0) {
                    printf("enter only 1 - 12\n");
                    printf("enter year again: ");
                    scanf("%u", &date.year);
                }
                expenseNode *newNode = createExpenseNode(expense_id, user_id, expense_category, expense_amount, date);
                if (newNode == NULL) {
                    sc = FAILURE;
                } else {
                    *expenseHead = insertCorrectPlaceExpense(*expenseHead, newNode);
                }
            }
        }
    }
    return sc;
}

//family
bool doesFamilyUsersExist (userNode *head, int *userArray, int size) {
    bool found  = false;
    for (int i = 0; i < size && !found; i++) {
        found = doesUserExist(head, userArray[i]);
    }
    return found;
}

bool checkifUserInAnotherFamily(familyNode* fhead, int user_id) {   
    familyNode* temp = fhead;
    bool found = false;
    while(temp != NULL && !found) {
        for(int i = 0 ; i < temp->total_members && found == 0 ; i++) {
            if(temp->family_members_user_id[i] == user_id) {
                found = true;
            }
        }
        temp = temp -> next;
    }
    return found;
}

familyNode* mergeFamily (familyNode* head1, familyNode* head2) {
    familyNode* result = NULL;
    familyNode* temp = NULL;
    while (head1 != NULL && head2 != NULL) {
        if (head1 -> family_ID <= head2 -> family_ID) {
            if (result == NULL) {
                result = temp = head1;
            } else {
                temp->next = head1;
                head1->prev = temp;
                temp = temp->next;
            }
            head1 = head1->next;
        } else {
            if (result == NULL) {
                result = temp = head2;
            } else {
                temp->next = head2;
                head2->prev = temp;
                temp = temp->next;
            }
            head2 = head2->next;
        }
    }
    if (head1 != NULL) {
        temp->next = head1;
        head1->prev = temp;
    }
    if (head2 != NULL) {
        temp->next = head2;
        head2->prev = temp;
    }

    return result;
}

familyNode* divideFamilyList (familyNode* head) {
    familyNode* slow = head;
    familyNode* fast = head;
    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        if (fast) {
            slow = slow->next;
        }
    }
    familyNode* middleNext = slow->next;
    slow->next = NULL;
    if (middleNext) {
        middleNext->prev = NULL;
    }
    return middleNext;
}

familyNode* mergeSortFamily (familyNode* head) {
    if (head != NULL && head -> next != NULL) {
        familyNode* middle = divideFamilyList(head);
        familyNode* left = mergeSortFamily(head);
        familyNode* right = mergeSortFamily(middle);
        head = mergeFamily(left, right);
    }
    return head;
}

familyNode* insertBeforeFamily (familyNode *head, familyNode *before, familyNode *newNode) {
    familyNode *temp, *prevNode;
    if (before == NULL) {
        if (head == NULL) {
            head = newNode;
        } else {
            temp = head;
            while (temp -> next != NULL) {
                temp = temp -> next;
            }
            temp -> next = newNode;
            newNode -> prev = temp;
            newNode -> next = NULL;
        }
    } else {
        newNode -> next = before;
        newNode -> prev = before -> prev;
        before -> prev = newNode;
        prevNode = newNode -> prev;
        if (prevNode != NULL) {
            prevNode -> next = newNode;
        } else {
            head = newNode;
        }
    }
    return head;
}

familyNode* insertCorrectPlaceFamily (familyNode *head, familyNode *newNode) {
    familyNode *curr = head;
    while ((curr != NULL) && (curr -> family_ID <= newNode -> family_ID)) {
        curr = curr -> next;
    }
    head = insertBeforeFamily(head, curr, newNode);
    return head;
}

float getTotalFamilyExpense (expenseNode *head, familyNode *node) {
    float expense = 0;
    int size = node -> total_members;
    for (int i = 0; i < size; i++) {
        int userId = node -> family_members_user_id[i];
        expenseNode *temp = head;
        while (temp != NULL) {
            if (temp -> user_ID == userId) {
                expense += temp -> expense_amount;
            }
            temp = temp -> next;
        }
    }
    return expense;
}

float getTotalFamilyIncome (userNode *head, familyNode *node) {
    float income = 0;
    int size = node -> total_members;
    for (int i = 0; i < size; i++) {
        int userId = node -> family_members_user_id[i];
        userNode *temp = head;
        while (temp != NULL) {
            if (temp -> user_ID == userId) {
                income += temp -> income;
            }
            temp = temp -> next;
        }
    }
    return income;
}

statusCode CreateFamily (familyNode **head, expenseNode *eHead, userNode *uHead) {
    statusCode sc = SUCCESS;
    familyNode *newNode;
    int size = familyNodelength(*head);
    if (size > MAX_NO_OF_FAMILIES || size < 0) {
        printf("family list is out of bounds.\n");
        sc = FAILURE;
    } else {
        int family_id;
        int total_members; 
        printf("Enter family id: ");
        scanf("%d", &family_id);
        while (family_id > 100 || family_id < 1) {
            printf("wrong input. family id cannot exceed 100 and cannot be less than 1.\n");
            printf("enter family id : ");
            scanf("%d",&family_id);
        }
        bool does_exist = doesFamilyIdExist(*head, family_id);
        if (does_exist) {
            printf("family with id %d exists already.\n", family_id);
        } else {
            printf("Enter total members in the family: ");
            scanf("%d", &total_members);
            while (total_members > 4 || total_members < 1) {
                printf("wrong input. total members should be between 1 and 4.\n");
                printf("enter total members again :");
                scanf("%d", &total_members);
            }
            int family_user_id[total_members];
            bool can_continue = true;
            for (int i = 0; i < total_members && can_continue; i++) {
                printf("Enter user ID of family member %d: ", i + 1);
                scanf("%d", &family_user_id[i]);
                bool user_exists = doesUserExist(uHead, family_user_id[i]);
                if(user_exists) {
                    bool user_in_other_family = checkifUserInAnotherFamily(*head, family_user_id[i]);
                    if(user_in_other_family) {
                        printf("User %d exists in another family, Cant add user to this family.\n",family_user_id[i]);
                        can_continue = false;
                    } else {
                        can_continue = true;
                    }
                } else {
                    printf("User %d doesnt exist, cant add user to the family.\n", family_user_id[i]);
                    can_continue = false;
                }
            }
            if(can_continue) {
                char family_name[25]; 
                printf("Enter family name: ");
                fgets(family_name, sizeof(family_name), stdin);
                family_name[strcspn(family_name, "\n")] = 0;

                while (family_name[0] == '\0') {
                    printf("Wrong input. Please enter a valid family name.\n");
                    printf("Enter family name: ");
                    fgets(family_name, sizeof(family_name), stdin);
                    family_name[strcspn(family_name, "\n")] = 0;
                }
                newNode = createFamilyNode(family_id, family_name, total_members, family_user_id);
                if (newNode == NULL) {
                    sc = FAILURE;
                } else {
                    *head =  insertCorrectPlaceFamily(*head, newNode);
                    newNode -> total_family_expense = getTotalFamilyExpense(eHead, newNode);
                    newNode -> total_family_income = getTotalFamilyIncome(uHead, newNode);
                }
            } else {
                printf("operation failed.\n");
            }
        }
    }
    return sc;
}

void traverseUserList(userNode *head) {
    const int id_width = 10;
    const int name_width = 20;
    const int income_width = 15;
    
    printf("+-%-*s-+-%-*s-+-%-*s-+\n",
           id_width, "----------",
           name_width, "--------------------",
           income_width, "---------------");
    
    printf("| %-*s | %-*s | %-*s |\n",
           id_width, "User ID",
           name_width, "User Name",
           income_width, "Income");
    
    printf("+-%-*s-+-%-*s-+-%-*s-+\n",
           id_width, "----------",
           name_width, "--------------------",
           income_width, "---------------");

    userNode *temp = head;
    while (temp != NULL) {
        printf("| %-*d | %-*s | %-*.2f |\n",
               id_width, temp->user_ID,
               name_width, temp->user_name,
               income_width, temp->income);
        temp = temp->next;
    }
    
    printf("+-%-*s-+-%-*s-+-%-*s-+\n",
           id_width, "----------",
           name_width, "--------------------",
           income_width, "---------------");
}

void traverseExpenseList(expenseNode *head) {
    const int user_id_width = 10;
    const int expense_id_width = 12;
    const int amount_width = 15;
    const int category_width = 15;
    const int date_width = 12;
    
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           user_id_width, "----------",
           expense_id_width, "------------",
           amount_width, "---------------",
           category_width, "---------------",
           date_width, "------------");
    
    printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n",
           user_id_width, "User ID",
           expense_id_width, "Expense ID",
           amount_width, "Amount",
           category_width, "Category",
           date_width, "Date");
    
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           user_id_width, "----------",
           expense_id_width, "------------",
           amount_width, "---------------",
           category_width, "---------------",
           date_width, "------------");

    expenseNode *temp = head;
    while (temp != NULL) {
        printf("| %-*d | %-*d | %-*.2f | %-*s | %02u-%02u-%04u |\n",
               user_id_width, temp->user_ID,
               expense_id_width, temp->expense_ID,
               amount_width, temp->expense_amount,
               category_width, temp->expense_category,
               temp->date_of_expense.date,
               temp->date_of_expense.month,
               temp->date_of_expense.year);
        temp = temp->next;
    }
    
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           user_id_width, "----------",
           expense_id_width, "------------",
           amount_width, "---------------",
           category_width, "---------------",
           date_width, "------------");
}

void traverseFamilyList(familyNode *head) {
    const int id_width = 10;
    const int name_width = 15;
    const int users_width = 15;
    const int expense_width = 15;
    const int income_width = 15;
    
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           id_width, "----------",
           name_width, "---------------",
           users_width, "---------------",
           expense_width, "---------------",
           income_width, "---------------");
    
    printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n",
           id_width, "Family ID",
           name_width, "Family Name",
           users_width, "User IDs",
           expense_width, "Total Expense",
           income_width, "Total Income");
    
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           id_width, "----------",
           name_width, "---------------",
           users_width, "---------------",
           expense_width, "---------------",
           income_width, "---------------");

    familyNode *temp = head;
    while (temp != NULL) {
        printf("| %-*d | %-*s | ", 
               id_width, temp->family_ID,
               name_width, temp->family_name);
        
        char user_ids[50] = "(";
        int pos = 1;
        
        for (int i = 0; i < temp->total_members; i++) {
            if (i > 0) {
                pos += sprintf(user_ids + pos, " %d", temp->family_members_user_id[i]);
            } else {
                pos += sprintf(user_ids + pos, "%d", temp->family_members_user_id[i]);
            }
        }
        strcat(user_ids, ")");
        printf("%-*s | ", users_width, user_ids);
        
        printf("%-*.2f | %-*.2f |\n",
               expense_width, temp->total_family_expense,
               income_width, temp->total_family_income);
        
        temp = temp->next;
    }
    
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           id_width, "----------",
           name_width, "---------------",
           users_width, "---------------",
           expense_width, "---------------",
           income_width, "---------------");
}

familyNode* updateFamilyUsersIncome (familyNode *fhead, userNode* node, userNode *uhead) {
    familyNode *temp = fhead;
    int uid = node -> user_ID;
    while (temp != NULL) {
        int size = temp -> total_members;
        bool found = false;
        for (int i = 0; i < size && !found; i++) {
            if (temp -> family_members_user_id[i] == uid) {
                found = true;
            }
        }
        if (found) {
            temp -> total_family_income = getTotalFamilyIncome(uhead, temp);
        }
        temp = temp -> next;
    }
    traverseFamilyList(fhead);
    return fhead;
}

statusCode updateUserDetails (userNode **head, familyNode **fhead) {
    statusCode sc = SUCCESS;
    if (*head == NULL) {
        printf("user list is empty\n");
        sc = FAILURE;
    } else {
        int user_id;
        printf("enter which user id you want to update :");
        scanf("%d", &user_id);
        bool exist = doesUserExist(*head, user_id);
        if (!exist) {
            printf("user not found\n");
            sc = FAILURE;
        } else {
            printf("1. enter 1 to change user name.\n");
            printf("2. enter 2 to update user income.\n");
            printf("3. enter 3 to change both user name and user income.\n");
            int option;
            printf("enter your option :");
            scanf("%d", &option);
            userNode *temp = *head;
            while (user_id != temp -> user_ID) {
                temp = temp -> next;
            }
            if (option == 1) {
                char userName[100];
                printf("enter new user name :");
                scanf("%99s", userName);
                strcpy(temp->user_name, userName);
            } else if (option == 2) {
                float income;
                printf("enter your new income :");
                scanf("%f", &income);
                temp -> income = income;
            } else if (option == 3) {
                char userName[100];
                float income;
                printf("enter new user name :");
                scanf("%99s", userName);
                printf("enter your new income :");
                scanf("%f", &income);
                temp -> income = income;
                strcpy(temp->user_name, userName);
            } else {
                printf("you have entered a invalid option\n");
                sc = FAILURE;
            }
            if (option == 2 || option == 3) {
                if (*fhead == NULL) {
                    printf("family list is empty");
                    sc = FAILURE;
                } else {
                    *fhead = updateFamilyUsersIncome(*fhead, temp, *head);
                }
            }
            printf("user %d details updated successfully.\n", user_id);
        }
    }
    traverseUserList(*head);
    return sc;
}

statusCode updateFamilyDetails (familyNode **head, userNode **uhead, int *f_id) {
    statusCode sc = SUCCESS;
    if (*head == NULL) {
        printf("family list is empty\n");
        sc = FAILURE;
    } else {
        int famId;
        printf("enter family id to update :");
        scanf("%d", &famId);
        bool exist = doesFamilyIdExist(*head, famId);
        if (!exist) {
            printf("family id %d doesnot exist.\n", famId);
            sc = FAILURE;
        } else {
            *f_id = famId;
            char famName[100];
            printf("1. enter 1 to change family name :\n");
            printf("2. enter 2 to update user list.\n");
            printf("3. enter 3 to change both family name and update user list.\n");
            printf("4. enter 4 to update total monthly income.\n");
            int option;
            printf("enter your option :");
            scanf("%d", &option);
            familyNode *temp = *head;
            while (famId != temp -> family_ID) {
                temp = temp -> next;
            }
            switch (option) {
                case 1: {
                    printf("enter new family name :");
                    scanf("%99s", famName);
                    strcpy(temp->family_name, famName);
                    break;
                }
                case 2: {
                    if (temp -> total_members == 4) {
                        printf("Max number of users are in family\n");
                        sc = FAILURE;
                    } else {
                        statusCode sc1 = AddUser(uhead, head);
                        if (sc1 == FAILURE) {
                            printf("corresponding users details failed.\n");
                            sc = FAILURE;
                        }
                    }
                    break;
                }
                case 3: {
                    printf("enter new family name :");
                    scanf("%99s", famName);
                    strcpy(temp->family_name, famName);

                    if (temp -> total_members == 4) {
                        printf("Max number of users are in family\n");
                        sc = FAILURE;
                    } else {
                        statusCode sc1 = AddUser(uhead, head);
                        if (sc1 == FAILURE) {
                            printf("corresponding users details failed.\n");
                            sc = FAILURE;
                        }
                    }
                    break;
                }
                case 4 : {
                    int income;
                    printf("enter new income :");
                    scanf("%d", &income);
                    temp -> total_family_income = income;
                    break;
                }
                default: {
                    printf("you entered a wrong number\n");
                    sc = FAILURE;
                    break;
                }
            }
            traverseUserList(*uhead);
            traverseFamilyList(*head);
        }
    }
    return sc;
}

userNode* deleteUserNode (userNode *head, userNode *ntd) {
    if (head == NULL) {
        head = NULL;
    } else {
        if (ntd == NULL) {
            head = head;
        } else {
            userNode *nextNode, *prevNode;
            nextNode = ntd -> next;
            prevNode = ntd -> prev;
            if (prevNode != NULL) {
                prevNode -> next = nextNode;
            } else {
                head = nextNode;
                nextNode -> prev = NULL;
            }
            if (nextNode != NULL) {
                nextNode -> prev = prevNode;
            } else {
                prevNode -> next = NULL;
            }
            printf("user id : %d is deleted.\n", ntd -> user_ID);
            free(ntd);
            ntd = NULL;
        }
    }
    return head;
}

familyNode* deleteFamilyNode (familyNode *head, familyNode *ntd) {
    if (head == NULL) {
        head = NULL;
    } else {
        if (ntd == NULL) {
            head = head;
        } else {
            familyNode *nextNode, *prevNode;
            nextNode = ntd -> next;
            prevNode = ntd -> prev;
            if (prevNode != NULL) {
                prevNode -> next = nextNode;
            } else {
                head = nextNode;
                nextNode -> prev = NULL;
            }
            if (nextNode != NULL) {
                nextNode -> prev = prevNode;
            } else {
                prevNode -> next = NULL;
            }
            printf("family id : %d is deleted.\n", ntd -> family_ID);
            free(ntd);
            ntd = NULL;
        }
    }
    return head;
}

familyNode* deleteUserFromFamily (familyNode *head, int user_id, float income) {
    familyNode *temp = head;
    while (temp != NULL) {
        bool found = false;
        int size = temp -> total_members;
        int i = 0;
        while (i < size && !found) {
            if (temp -> family_members_user_id[i] != user_id) {
                i++;
            } else {
                found = true;
            }
        }
        familyNode *ntd = temp;
        temp = temp -> next;
        if (found) {
            if (size == 1) {
                head = deleteFamilyNode(head, ntd);
            } else {
                ntd -> family_members_user_id[i] = -1;
                ntd -> total_family_income -= income;
                int newArray[4] = {-1, -1, -1, -1};
                int index = 0;
                for (int i = 0; i < 4; i++) {
                    if (ntd -> family_members_user_id[i] != -1) {
                        newArray[index] = ntd -> family_members_user_id[i];
                        index += 1;
                    }
                }
                for (int i = 0; i < 4; i++) {
                    ntd -> family_members_user_id[i] = newArray[i];
                }
                ntd -> total_members -= 1;
            }  
        }
    }
    return head;
}

expenseNode* removeUserExpense (expenseNode *ehead, int user_id) {
    expenseNode *temp = ehead;
    while (temp != NULL && temp -> user_ID != user_id) {
        temp = temp -> next;
    }
    while (temp != NULL && temp -> user_ID == user_id) {
        expenseNode *del = temp;
        temp = temp -> next;
        ehead = deleteExpenseNode(ehead, del);
    }
    return ehead;
}

statusCode deleteUser (userNode **head, familyNode **fhead, expenseNode **ehead) {
    statusCode sc = SUCCESS;
    if (*head == NULL) {
        printf("user list is empty\n");
        sc = FAILURE;
    } else {
        int user_id;
        printf("enter user id to delete :");
        scanf("%d", &user_id);
        bool exist = doesUserExist(*head, user_id);
        if (!exist) {
            printf("user not found\n");
            sc = FAILURE;
        } else {
            userNode *temp = *head;
            while (temp -> user_ID != user_id) {
                temp = temp -> next;
            }
            float income = temp -> income;
            *head = deleteUserNode(*head, temp);
            printf("user %d deleted successfully.\n", user_id);
            if (*fhead == NULL) {
                printf("family list is empty\n");
                sc = FAILURE;
            } else {
                *fhead = deleteUserFromFamily(*fhead, user_id, income);
            }
            if (*ehead == NULL) {
                printf("expense list is empty.\n");
                sc = FAILURE;
            } else {
                *ehead = removeUserExpense(*ehead, user_id);
            }
        }
    }
    traverseUserList(*head);
    traverseFamilyList(*fhead);
    traverseExpenseList(*ehead);
    return sc;
}

userNode* deleteFamilyUsers (userNode *head, int *user_array, int size) {
    for (int i = 0; i < size; i++) {
        userNode *temp = head;
        bool found = false;
        while (temp != NULL && !found) {
            if (temp -> user_ID == user_array[i]) {
                found = true;
            } else {
                temp = temp -> next;
            }
        }
        if (found) {
            head = deleteUserNode(head, temp);
        }
    }
    return head;
}

expenseNode* Remove_Family_User_Expenses(expenseNode *ehead, int family_members_user_id[], int family_members) {
    for(int i = 0; i < family_members; i++) {
        ehead = removeUserExpense(ehead, family_members_user_id[i]);
    }
    return ehead;
}

//users are not common to families, belongs to only one family one user
statusCode deleteFamily (familyNode **head, userNode **uhead, expenseNode **ehead) {
    statusCode sc = SUCCESS;
    if (*head == NULL) {
        printf("family list is empty\n");
        sc = FAILURE;
    } else {
        int fam_id;
        printf("enter family id to delete :");
        scanf("%d", &fam_id);
        bool exist = doesFamilyIdExist(*head, fam_id);
        if (!exist) {
            printf("family id doesnot exist\n");
            sc = FAILURE;
        } else {
            familyNode *temp = *head;
            while (temp -> family_ID != fam_id) {
                temp = temp -> next;
            }
            if (*uhead == NULL) {
                sc = FAILURE;
            } else {
                *uhead = deleteFamilyUsers(*uhead, temp -> family_members_user_id, temp -> total_members);
            }
            if (*ehead == NULL) {
                printf("expense list is empty.\n");
                sc = FAILURE; 
            } else {
                *ehead = Remove_Family_User_Expenses(*ehead, temp -> family_members_user_id, temp -> total_members);
            }
            *head = deleteFamilyNode(*head, temp);
        }
    }
    traverseFamilyList(*head);
    traverseUserList(*uhead);
    traverseExpenseList(*ehead);
    return sc;
}

familyNode* updateFamilyExpense (familyNode *head, int user_id, expenseNode *ehead) {
    familyNode *temp = head;
    while (temp != NULL) {
        int size = temp -> total_members;
        bool found = false;
        int i = 0;
        while (!found && i < size) {
            if (temp -> family_members_user_id[i] == user_id) {
                found = true;
            } else {
                i++;
            }
        }
        if (found) {
            temp -> total_family_expense = getTotalFamilyExpense(ehead, temp);
        }
        temp = temp -> next;
    }
    return head;
}

statusCode updateExpense (expenseNode **head, familyNode **fhead) {
    statusCode sc = SUCCESS;
    if (*head == NULL) {
        printf("expense list is empty\n");
        sc = FAILURE;
    } else {
        int expense_id;
        printf("enter expense id to update :");
        scanf("%d", &expense_id);
        bool exist = doesExpenseIdExist(*head, expense_id);
        if (!exist) {
            printf("expense id doesnot exist\n");
            sc = FAILURE;
        } else {
            printf("1. enter 1 to update expense category.\n");
            printf("2. enter 2 to update expense amount.\n");
            printf("3. enter 3 to update date of expense.\n");
            printf("4. enter 4 to do update all.\n");
            int option;
            printf("enter your option :");
            scanf("%d", &option);
            expenseNode *temp = *head;
            while (temp -> expense_ID != expense_id) {
                temp = temp -> next;
            }
            switch (option) {
                case 1 : {
                    char expense_category[15];
                    printf("enter expense category :");
                    scanf("%14s", expense_category);
                    while(strcmp(expense_category, "rent") != 0 && strcmp(expense_category, "utility") != 0 && strcmp(expense_category, "stationary") != 0 && strcmp(expense_category, "grocery") != 0 && strcmp(expense_category, "leisure") != 0){
                        printf("enter only rent or utility or stationary or grocery or leisure\n");
                        printf("Enter expense category again : ");
                        scanf("%14s", expense_category);         
                    }
                    capitalize(expense_category);
                    strcpy(temp -> expense_category, expense_category);
                    printf("exepnse %d updated successfully.\n", expense_id);
                    break;
                }
                case 2 : {
                    float expense_amount;
                    printf("enter expense amount :");
                    scanf("%f", &expense_amount);
                    temp -> expense_amount = expense_amount;
                    printf("exepnse %d updated successfully.\n", expense_id);
                    break;
                }
                case 3 : {
                    unsigned int date;
                    unsigned int month;
                    unsigned int year;
                    printf("enter date :");
                    scanf("%u", &date);
                    while (date > 10 && date <= 0) {
                        printf("enter only 1 - 10");
                        scanf("%u", &date);
                    }
                    printf("enter month :");
                    scanf("%u", &month);
                    while (month > 12 && month <= 0) {
                        printf("enter only 1 - 12");
                        scanf("%u", &month);
                    }
                    printf("enter year :");
                    scanf("%u", &year);
                    Date d = createDate(date, month, year);
                    temp -> date_of_expense = d;
                    printf("exepnse %d updated successfully.\n", expense_id);
                    break;
                }
                case 4 : {
                    char* expense_category;
                    printf("enter expense category :");
                    scanf("%s", expense_category);
                    while(strcmp(expense_category, "rent") != 0 && strcmp(expense_category, "utility") != 0 && strcmp(expense_category, "stationary") != 0 && strcmp(expense_category, "grocery") != 0 && strcmp(expense_category, "leisure") != 0){
                        printf("enter only rent or utility or stationary or grocery or leisure\n");
                        printf("Enter expense category again : ");
                        scanf("%14s", expense_category);         
                    }
                    capitalize(expense_category);
                    strcpy(temp -> expense_category, expense_category);
                
                    float expense_amount;
                    printf("enter expense amount :");
                    scanf("%f", &expense_amount);
                    temp -> expense_amount = expense_amount;

                    unsigned int date;
                    unsigned int month;
                    unsigned int year;
                    printf("enter date :");
                    scanf("%u", &date);
                    printf("enter month :");
                    scanf("%u", &month);
                    printf("enter year :");
                    scanf("%u", &year);
                    Date d = createDate(date, month, year);
                    temp -> date_of_expense = d;
                    printf("exepnse %d updated successfully.\n", expense_id);
                    break;
                }
                default : {
                    printf("you entered a wrong number.\n");
                    sc = FAILURE;
                    break;
                }
            }
            if (option == 2 || option == 4) {
                if (fhead == NULL) {
                    printf("family list is empty\n");
                    sc = FAILURE;
                } else {
                    *fhead = updateFamilyExpense(*fhead, temp -> user_ID, *head);
                }
            }
        }
    }
    traverseExpenseList(*head);
    traverseFamilyList(*fhead);
    return sc;
}

expenseNode* deleteExpenseNode (expenseNode *head, expenseNode *ntd) {
    if (head == NULL) {
        head = NULL;
    } else {
        if (ntd == NULL) {
            head = head;
        } else {
            expenseNode *nextNode, *prevNode;
            nextNode = ntd -> next;
            prevNode = ntd -> prev;
            if (prevNode != NULL) {
                prevNode -> next = nextNode;
            } else {
                head = nextNode;
                nextNode -> prev = NULL;
            }
            if (nextNode != NULL) {
                nextNode -> prev = prevNode;
            } else {
                prevNode -> next = NULL;
            }
            printf("expense id : %d is deleted.\n", ntd -> expense_ID);
            free(ntd);
            ntd = NULL;
        }
    }
    return head;
}

statusCode deleteExpense (expenseNode **head, familyNode **fhead) {
    statusCode sc = SUCCESS;
    if (*head == NULL) {
        printf("expense list is empty\n");
        sc = FAILURE;
    } else {
        int expense_id;
        printf("enter expense id to delete :");
        scanf("%d", &expense_id);
        bool exist = doesExpenseIdExist(*head, expense_id);
        if (!exist) {
            printf("expense id doesnot exist\n");
            sc = FAILURE;
        } else {
            expenseNode *temp = *head;
            while (temp -> expense_ID != expense_id) {
                temp = temp -> next;
            }
            int user_id = temp -> user_ID;
            *head = deleteExpenseNode(*head, temp);
            if (fhead == NULL) {
                printf("family list is empty\n");
                sc = FAILURE;
            } else {
                *fhead = updateFamilyExpense(*fhead, user_id, *head);
            }
        }
    }
    traverseExpenseList(*head);
    traverseFamilyList(*fhead);
    return sc;
}

statusCode Get_Total_Expense (familyNode *fhead) {
    int fam_id;
    statusCode sc = SUCCESS;
    printf("enter your family id :");
    scanf("%d", &fam_id);
    if (!doesFamilyIdExist(fhead, fam_id)) {
        printf("family id doesnot exist.\n");
        sc = FAILURE;
    } else {
        familyNode *fnode = getFamilyNode(fhead, fam_id);
        printf("total family expense : %f.\n", fnode -> total_family_expense);
        if (fnode -> total_family_expense <= fnode -> total_family_income) {
            printf("family expense per month is within the range of income.\n");
        } else {
            printf("family expense per month surpasses your income by %.2f.\n", fnode -> total_family_expense - fnode -> total_family_income);
        }
        printf("if you want to check any particular expense enter 1.\n if not enter any number other than 1.\n");
        int option;
        printf("enter your option :");
        scanf("%d", &option);
        if (option == 1) {
            float expense_amount;
            printf("enter you expense amount :");
            scanf("%f", &expense_amount);
            if (fnode -> total_family_income >= expense_amount) {
                printf("current expense is within the range of income.\n");
            } else {
                printf("current expense surpasses your income by %.2f.\n", expense_amount - fnode -> total_family_income);
            }
        }
    }
    return sc;
}

expenseNode* getExpenseNode (expenseNode *head, int user_id) {
    expenseNode *temp = head;
    while (temp != NULL && temp -> user_ID != user_id) {
        temp = temp -> next;
    }
    return temp;
}

statusCode get_categorical_expense(familyNode *fhead, expenseNode *ehead) {
    statusCode sc = SUCCESS;
    int family_ID;
    printf("enter family id :");
    scanf("%d", &family_ID);
    float total_expense = 0;
    familyNode* temp = fhead;
    int found = 0;
    while(temp != NULL && !found) {
        if(family_ID == temp->family_ID) {
            found = 1;
        } else {
            temp = temp->next;
        }
    }
    if (found == 0) {
        printf("family %d not found.\n", family_ID);
        sc = FAILURE;
    } else {
        char category_name[15];
        printf("enter category name :");
        scanf("%14s", category_name);
        while(strcmp(category_name, "rent") != 0 && strcmp(category_name, "utility") != 0 && strcmp(category_name, "stationary") != 0 && strcmp(category_name, "grocery") != 0 && strcmp(category_name, "leisure") != 0){
            printf("enter only rent or utility or stationary or grocery or leisure\n");
            printf("Enter category name again : ");
            scanf("%14s", category_name);         
        }
        capitalize(category_name);
        found = 0;
        int family_size = temp->total_members;
        familyUserExpense user_expense[family_size];

        for (int i = 0; i < family_size; i++) {
            user_expense[i].user_id = temp -> family_members_user_id[i];
            user_expense[i].expense = 0;
        }
        for(int i = 0; i < family_size ; i++) {
            for(expenseNode* ptr = ehead; ptr != NULL; ptr = ptr->next) {
                if(temp->family_members_user_id[i] == ptr->user_ID) {
                    user_expense[i].user_id = ptr -> user_ID;
                    if(strcmp(ptr->expense_category, category_name) == 0) {
                        found = 1;
                        total_expense = total_expense + ptr->expense_amount;
                        user_expense[i].expense += ptr -> expense_amount;
                    }
                }
            }
        }
        if(found) {
            for (int i = 1; i < family_size; i++) {
                familyUserExpense key = user_expense[i];
                int j = i - 1;
                while (j >= 0 && user_expense[j].expense < key.expense) {
                    user_expense[j + 1] = user_expense[j];
                    j = j - 1;
                }
                user_expense[j + 1] = key;
            }
            printf("Family %d expense on the category %s is %.2f\n",temp->family_ID, category_name, total_expense);
            for (int i = 0; i < family_size; i++) {
                printf("user %d : %f\n", user_expense[i].user_id, user_expense[i].expense);
            }
        } else {
            printf("No expense spent by the family %d on category %s\n",temp->family_ID,category_name);
        }
    }
    return sc;
}

statusCode get_highest_expense_day(familyNode* fhead, expenseNode* ehead) {
    statusCode sc = SUCCESS;
    if (fhead == NULL || ehead == NULL) {
        printf("Empty family or expense list\n");
        sc = FAILURE;
    } else {
        int family_id;
        printf("Enter family ID: ");
        scanf("%d", &family_id);
        familyNode* current_family = fhead;
        while (current_family != NULL && current_family->family_ID != family_id) {
            current_family = current_family->next;
        }      
        if (current_family == NULL) {
            printf("Family ID %d not found\n", family_id);
            sc = FAILURE;
        } else {
            dailyExpense daily_expenses[MAX_EXPENSES] = {0};
            dailyExpense max_expense_day = {0};
            int unique_days_count = 0;
            bool found_any_expense = false;
            expenseNode* current_expense = ehead;
            while (current_expense != NULL) {
                bool is_family_member = false;
                for (int i = 0; i < current_family->total_members && !is_family_member; i++) {
                    if (current_family->family_members_user_id[i] == current_expense->user_ID) {
                        is_family_member = true;
                    }
                }               
                if (is_family_member) {
                    found_any_expense = true;
                    bool date_found = false;
                    for (int i = 0; i < unique_days_count && !date_found; i++) {
                        if (daily_expenses[i].date.date == current_expense->date_of_expense.date &&
                            daily_expenses[i].date.month == current_expense->date_of_expense.month &&
                            daily_expenses[i].date.year == current_expense->date_of_expense.year) {
                            daily_expenses[i].total_expense += current_expense->expense_amount;

                            if (daily_expenses[i].total_expense > max_expense_day.total_expense) {
                                max_expense_day = daily_expenses[i];
                            }
                            date_found = true;
                        }
                    }
                    if (!date_found) {
                        if (unique_days_count >= MAX_EXPENSES) {
                            printf("Maximum unique days limit reached\n");
                        } else {
                            daily_expenses[unique_days_count].date = current_expense->date_of_expense;
                            daily_expenses[unique_days_count].total_expense = current_expense->expense_amount;
                            if (daily_expenses[unique_days_count].total_expense > max_expense_day.total_expense) {
                                max_expense_day = daily_expenses[unique_days_count];
                            }
                            unique_days_count++;
                        }
                    }
                }
                current_expense = current_expense->next;
            }
            if (!found_any_expense) {
                printf("No expenses found for family ID %d\n", family_id);
            } else {
                printf("Highest expense day for family ID %d:\n", family_id);
                printf("Date: %u-%u-%u\n", max_expense_day.date.date, max_expense_day.date.month, max_expense_day.date.year);
                printf("Total expense: %.2f\n", max_expense_day.total_expense);
            }
        }
    }
    return sc;
}

statusCode Get_Individual_Expense (expenseNode *head, userNode *uhead) {
    statusCode sc = SUCCESS;
    int user_id;
    printf("enter user id :");
    scanf("%d", &user_id);
    if (!doesUserExist(uhead, user_id)) {
        printf("user %d doesnot exist.\n", user_id);
        sc = FAILURE;
    } else {
        float total_expense = 0;
        expenseNode *temp = getExpenseNode(head, user_id);
        if (temp == NULL) {
            sc = FAILURE;
        } else {
            cat_exp categoryExpense[5] = {
                {"RENT", 0},
                {"UTILITY", 0},
                {"GROCERY", 0},
                {"STATIONARY", 0},
                {"LEISURE", 0}
            };
            while (temp != NULL && temp -> user_ID == user_id) {
                total_expense += temp -> expense_amount;
                bool stop = false;
                for (int i = 0; i < 5 && !stop; i++) {
                    if (strcmp(temp->expense_category, categoryExpense[i].category_name) == 0) {
                        categoryExpense[i].expense += temp->expense_amount;
                        stop = true;
                    }
                }
                temp = temp -> next;
            }
            for (int i = 1; i < 5; i++) {
                cat_exp key = categoryExpense[i];
                int j = i - 1;
                while (j >= 0 && categoryExpense[j].expense < key.expense) {
                    categoryExpense[j + 1] = categoryExpense[j];
                    j = j - 1;
                }
                categoryExpense[j + 1] = key;
            }           
            printf("total expense by user %d : is %.2f.\n", user_id, total_expense);
            for (int i = 0; i < 5; i++) {
                if (categoryExpense[i].expense != 0) {
                    printf("%s: %.2f\n", categoryExpense[i].category_name, categoryExpense[i].expense);
                }
            }
        }
    }
    return sc;
}

statusCode readUsersFile (userNode **uhead) {
    statusCode sc = SUCCESS;
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        sc = FAILURE;
    } else {
        int id;
        char name[MAX_NAME_LENGTH];
        float income;
        while (fscanf(file, "%d,%99[^,],%f", &id, name, &income) == 3) {
            userNode* new_node = createUserNode(id, name, income);
            if (new_node) {
                if (*uhead == NULL) {
                    *uhead = new_node;
                } else {
                    new_node -> next = *uhead;
                    (*uhead) -> prev = new_node;
                    *uhead = new_node;
                }
            } else {
                printf("error creating user node.\n");
                sc = FAILURE;
            }
        }
        printf("users file read successfully.\n");
        fclose(file);
    }
    if (*uhead) {
        *uhead = mergeSortUser(*uhead);
        traverseUserList(*uhead);
    }
    return sc;
}

statusCode readExpensesFile (expenseNode **ehead) {
    statusCode sc = SUCCESS;
    FILE *file = fopen("expenses.txt", "r");
    if (file == NULL) {
        sc = FAILURE;
    } else {
        int exp_id;
        int user_id;
        char exp_cat[15];
        float income;
        Date d;
        while (fscanf(file, "%d,%d,%[^,],%f,%u,%u,%u", &exp_id, &user_id, exp_cat, &income, &d.date, &d.month, &d.year) == 7 && sc != FAILURE) {
            expenseNode *new_node = createExpenseNode(exp_id, user_id, exp_cat, income, d);
            if (new_node) {
                if (*ehead == NULL) {
                    *ehead = new_node;
                } else {
                    new_node -> next = *ehead;
                    (*ehead) -> prev = new_node;
                    *ehead = new_node;
                }
            } else {
                printf("error creating expense node.\n");
                sc = FAILURE;
            }
        }
        printf("expenses file read successfully.\n");
        fclose(file);
    }
    if (*ehead) {
        *ehead = mergeSortExpense(*ehead);
        traverseExpenseList(*ehead);
    }
    return sc;
}

statusCode readFamiliesFile (familyNode **fhead, expenseNode *ehead, userNode *uhead) {
    statusCode sc = SUCCESS;
    FILE *file = fopen("families.txt", "r");
    if (file == NULL) {
        sc = FAILURE;
    } else {
        int fam_id;
        char fam_name[50];
        int members;
        while (fscanf(file, "%d,%49[^,],%d", &fam_id, fam_name, &members) == 3 && sc != FAILURE) {
            int user_id_array[members];
            for (int i = 0; i < members; i++) {
                if (fscanf(file, ",%d", &user_id_array[i]) != 1) {
                    printf("Failed to read members for family %d\n", fam_id);
                    sc = FAILURE;
                }
            }
            if (sc != FAILURE) {
                familyNode *new_node = createFamilyNode(fam_id, fam_name, members, user_id_array);
                if (new_node) {
                    if (*fhead == NULL) {
                        *fhead = new_node;
                    } else {
                        new_node -> next = *fhead;
                        (*fhead) -> prev = new_node;
                        *fhead = new_node;
                    }
                } else {
                    printf("error creating expense node.\n");
                    sc = FAILURE;
                }
            }
        }
        printf("families file read successfully.\n");
        fclose(file);
    }
    if (fhead) {
        familyNode *temp = *fhead;
        while (temp != NULL) {
            float income = getTotalFamilyIncome(uhead, *fhead);
            temp -> total_family_income = income;
            float expense = getTotalFamilyExpense(ehead, *fhead);
            temp -> total_family_expense = expense;
            temp = temp -> next;
        }
        *fhead = mergeSortFamily(*fhead);
        traverseFamilyList(*fhead);
    }
    return sc;
}

void deleteUserList(userNode **uhead) {
    userNode *current = *uhead;
    userNode *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
        temp = NULL;
    }
    *uhead = NULL;
}

void deleteExpenseList(expenseNode **ehead) {
    expenseNode *current = *ehead;
    expenseNode *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
        temp = NULL;
    }
    *ehead = NULL;
}

void deleteFamilyList(familyNode **fhead) {
    familyNode *current = *fhead;
    familyNode *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
        temp = NULL;
    }
    *fhead = NULL;
}

int main() {
    userNode *uhead = NULL;
    expenseNode *ehead = NULL;
    familyNode *fhead = NULL;
    
    statusCode sc = readUsersFile(&uhead);
    sc = readExpensesFile(&ehead);
    sc = readFamiliesFile(&fhead, ehead, uhead);

    int n = 1;
    while (n < 14 && n != 13 && n > 0) {
        printf("Enter 1  : Add User\n");
        printf("Enter 2  : Add Expense\n");
        printf("Enter 3  : Create Family\n");
        printf("Enter 4  : Update or Delete individual family\n");
        printf("Enter 5  : Update or Delete Expense\n");
        printf("Enter 6  : Get Total Expense\n");
        printf("Enter 7  : Get Categorical Expense\n");
        printf("Enter 8  : Get Highest Expense Day\n");
        printf("Enter 9  : Get Individual Expense\n");
        printf("Enter 10 : print users details\n");
        printf("Enter 11 : print expenses details\n");
        printf("Enter 12 : print families details\n");
        printf("Enter 13 : Exit\n");
        printf("enter n : ");
        scanf("%d", &n);

        switch (n) {
            case 1 :{
                statusCode status = AddUser(&uhead, &fhead);
                if (status == SUCCESS) {
                    printf("user added successfully.\n");
                    if(uhead) {
                        traverseUserList(uhead);
                        traverseFamilyList(fhead);
                    }
                } else {
                    printf("user adding operation failed.\n");
                }
                break;
            }
            case 2 :{
                statusCode status = AddExpense(&ehead, &uhead);
                if (status == SUCCESS) {
                    printf("expense added successfully.\n");
                    if (ehead) traverseExpenseList(ehead);
                } else {
                    printf("expense operation failed.\n");
                }
                break;
            }  
            case 3 : {
                statusCode status = CreateFamily(&fhead, ehead, uhead);
                if(status == SUCCESS) {
                    printf("Family Created Successfully.\n");
                    if (fhead) traverseFamilyList(fhead);
                } else {
                    printf("Error creating family.\n");
                }
                break;
            }
            case 4:{
                int j;
                printf("enter 1 : update_user\n");
                printf("enter 2 : update_family\n");
                printf("enter 3 : delete user\n");
                printf("enter 4 : delete family\n");
                printf("enter j : ");
                scanf("%d", &j);
                switch(j) {
                    case 1: {
                        statusCode status = updateUserDetails(&uhead, &fhead);
                        if (status == FAILURE) {
                            printf("update user unsuccessful.\n");
                        }
                        break;
                    }
                    case 2:{
                        int fam_id;
                        statusCode status = updateFamilyDetails(&fhead, &uhead, &fam_id);
                        if (status == SUCCESS) {
                            printf("family %d details updated successfully.\n", fam_id);
                        } else {
                            printf("family %d details updated was unsuccessful.\n", fam_id);
                        }
                        break;
                    }
                    case 3:{
                        statusCode status = deleteUser(&uhead, &fhead, &ehead);
                        break;
                    }
                    case 4:{
                        statusCode status = deleteFamily(&fhead, &uhead, &ehead);
                        break;
                    }
                    default:{
                        printf("you entered wrong number.\n");
                        break;
                    }
                }
                break;
            }
            case 5:{
                int p;
                printf("enter 1 : update expenses\n");
                printf("enter 2 : delete expenses\n");
                printf("enter your number : ");
                scanf("%d", &p);
                switch(p){
                    case 1: {
                        statusCode status = updateExpense(&ehead, &fhead);
                        break;
                    }
                    case 2: {
                        statusCode status = deleteExpense(&ehead, &fhead);
                        break;
                    }
                    default: {
                        printf("you entered a wrong number.\n");
                        break;
                    }
                }
                break;
            }
            case 6: {
                if(fhead) {
                    statusCode sc = Get_Total_Expense(fhead);
                }
                break;
            }
            case 7: {
                if(fhead && ehead) {
                    statusCode sc = get_categorical_expense(fhead, ehead);
                }
                break;
            }
            case 8: {
                statusCode sc = get_highest_expense_day(fhead, ehead);
                break;
            }
            case 9: {
                if(ehead && uhead) {
                    statusCode sc = Get_Individual_Expense(ehead, uhead);
                }
                break;
            }
            case 10: {
                traverseUserList(uhead);
                break;
            }
            case 11: {
                traverseExpenseList(ehead);
                break;
            }
            case 12: {
                traverseFamilyList(fhead);
                break;
            }
            case 13: {
                printf("Exiting.....\n");
                break;
            }
            default:{
                printf("you entered a wrong number.\n");
                break;
            }
        }
    }

    deleteUserList(&uhead);
    deleteExpenseList(&ehead);
    deleteFamilyList(&fhead);
    
   return 0;
}