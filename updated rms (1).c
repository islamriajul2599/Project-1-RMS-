/****
This code is created by Md Riajul Islam
Student of Hamm-Lippstadt University of Applied Sciences.
****/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node structure
struct node {
    char foodname[50];
    int quantity;
    float price;
    int data;
    struct node *prev;
    struct node *next;
};

// Global pointers for different lists
struct node *headc = NULL, *tailc = NULL; // Customer order list
struct node *heada = NULL, *taila = NULL; // Admin menu list
struct node *head_s = NULL; // Total sales list

struct node *newnode; // Temporary node

// File for storing menu items
const char *menuFile = "menu.txt";

// Admin menu function
void adminmenu() {
    printf("\n\t\t\t\t1. View total sales\n");
    printf("\t\t\t\t2. Add new items to the order menu\n");
    printf("\t\t\t\t3. Delete items from the order menu\n");
    printf("\t\t\t\t4. Display order menu\n");
    printf("\t\t\t\t5. Back to Main Menu\n\n");
    printf("\t\t\t\tEnter Your Choice ---> ");
}

// Customer menu function
void customermenu() {
    printf("\n\t\t\t\t1. Place your order\n");
    printf("\t\t\t\t2. View your ordered items\n");
    printf("\t\t\t\t3. Delete an item from order\n");
    printf("\t\t\t\t4. Display final bill\n");
    printf("\t\t\t\t5. Back to Main Menu\n\n");
    printf("\t\t\t\tEnter Your Choice ---> ");
}

// Function to create admin menu items
struct node* createadmin(struct node *head, int data, char foodname[50], float price) {
    newnode = (struct node*)malloc(sizeof(struct node));
    newnode->data = data;
    newnode->price = price;
    newnode->quantity = 0;
    strcpy(newnode->foodname, foodname);
    newnode->next = NULL;
    newnode->prev = NULL;

    if (head == NULL) {
        heada = taila = newnode;
    } else {
        taila->next = newnode;
        newnode->prev = taila;
        taila = newnode;
    }
    return heada;
}

// Function to create customer orders
struct node* createcustomer(struct node *head, int data, int quantity) {
    struct node *temp1 = heada;
    while (temp1 != NULL && temp1->data != data) {
        temp1 = temp1->next;
    }

    if (temp1 != NULL) {
        newnode = (struct node*)malloc(sizeof(struct node));
        newnode->data = data;
        newnode->price = quantity * temp1->price;
        newnode->quantity = quantity;
        strcpy(newnode->foodname, temp1->foodname);
        newnode->next = NULL;
        newnode->prev = NULL;

        if (head == NULL) {
            headc = tailc = newnode;
        } else {
            tailc->next = newnode;
            newnode->prev = tailc;
            tailc = newnode;
        }
    } else {
        printf("\n\t\t\t\tThis item is not present in the menu!\n");
    }
    return headc;
}

// Function to display the list of nodes
void displayList(struct node *head) {
    struct node *temp = head;
    if (temp == NULL) {
        printf("\n\t\t\t\tList is empty!!\n\n");
    } else {
        printf("\n");
        while (temp != NULL) {
            if (temp->quantity == 0) {
                printf("\t\t\t\t%d\t%s\t%.2f €\n", temp->data, temp->foodname, temp->price);
            } else {
                printf("\t\t\t\t%d\t%s\t%d\t%.2f €\n", temp->data, temp->foodname, temp->quantity, temp->price);
            }
            temp = temp->next;
        }
        printf("\n");
    }
}

// Function to calculate total sales
struct node* totalsales(int data, int quantity) {
    struct node *temp1 = heada;
    while (temp1->data != data) {
        temp1 = temp1->next;
    }

    newnode = (struct node*)malloc(sizeof(struct node));
    newnode->data = data;
    newnode->price = quantity * temp1->price;
    newnode->quantity = quantity;
    strcpy(newnode->foodname, temp1->foodname);
    newnode->next = NULL;
    newnode->prev = NULL;

    struct node *temp = head_s;
    int flag = 0;
    if (temp == NULL) {
        head_s = newnode;
    } else {
        while (temp->next != NULL) {
            if (temp->data == data) {
                flag = 1;
                break;
            }
            temp = temp->next;
        }
        if (flag == 1) {
            temp->quantity += newnode->quantity;
            temp->price += newnode->price;
        } else {
            temp->next = newnode;
        }
    }
    return head_s;
}

// Function to calculate total sales for all items in customer order list
void calculatetotsales() {
    struct node *temp = headc;
    while (temp != NULL) {
        head_s = totalsales(temp->data, temp->quantity);
        temp = temp->next;
    }
}

// Function to delete a node from the list
struct node* delete(int data, struct node *head, struct node *tail) {
    if (head == NULL) {
        printf("\n\t\t\t\tList is empty\n");
    } else {
        struct node *temp = head;
        if (data == head->data) {
            head = head->next;
            if (head != NULL) head->prev = NULL;
            free(temp);
        } else if (data == tail->data) {
            temp = tail;
            tail = tail->prev;
            tail->next = NULL;
            free(temp);
        } else {
            while (temp != NULL && temp->data != data) {
                temp = temp->next;
            }
            if (temp != NULL) {
                temp->prev->next = temp->next;
                if (temp->next != NULL) temp->next->prev = temp->prev;
                free(temp);
            }
        }
    }
    return head;
}

// Function to delete an item from admin menu
int deleteadmin() {
    printf("\n\t\t\tEnter serial no. of the food item which is to be deleted: ");
    int num;
    if (scanf("%d", &num) != 1) {
        printf("\n\t\t\tInvalid input! Please enter a valid number.\n");
        while (getchar() != '\n'); // Clear input buffer
        return 0;
    }

    struct node *temp = heada;
    while (temp != NULL) {
        if (temp->data == num) {
            heada = delete(num, heada, taila);
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

// Function to delete an item from customer order
int deletecustomer() {
    printf("\n\t\t\tEnter serial no. of the food item which is to be deleted: ");
    int num;
    if (scanf("%d", &num) != 1) {
        printf("\n\t\t\tInvalid input! Please enter a valid number.\n");
        while (getchar() != '\n'); // Clear input buffer
        return 0;
    }

    struct node *temp = headc;
    while (temp != NULL) {
        if (temp->data == num) {
            headc = delete(num, headc, tailc);
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

// Function to display final bill
void displaybill() {
    displayList(headc);
    struct node *temp = headc;
    float total_price = 0;
    while (temp != NULL) {
        total_price += temp->price;
        temp = temp->next;
    }
    printf("\t\t\t\tTotal price: %.2f €\n", total_price);
}

// Function to delete all nodes in a list
struct node* deleteList(struct node *head) {
    while (head != NULL) {
        struct node *temp = head;
        head = head->next;
        free(temp);
    }
    return NULL;
}

// Function to load menu from file
void loadMenu() {
    FILE *file = fopen(menuFile, "r");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening menu file.\n");
        return;
    }

    int data;
    char foodname[50];
    float price;

    while (fscanf(file, "%d %[^\t] %f", &data, foodname, &price) != EOF) {
        heada = createadmin(heada, data, foodname, price);
    }

    fclose(file);
}

// Function to save menu to file
void saveMenu() {
    FILE *file = fopen(menuFile, "w");
    if (file == NULL) {
        printf("\n\t\t\t\tError opening menu file.\n");
        return;
    }

    struct node *temp = heada;
    while (temp != NULL) {
        fprintf(file, "%d\t%s\t%.2f\n", temp->data, temp->foodname, temp->price);
        temp = temp->next;
    }

    fclose(file);
}

// Admin section function
void admin() {
    printf("\n\t\t\t\t----------------------------------------------\n");
    printf("\t\t\t\t\t\tADMIN SECTION\n");
    printf("\n\t\t\t\t----------------------------------------------\n");
    while (1) {
        adminmenu();
        int option;
        if (scanf("%d", &option) != 1) {
            printf("\n\t\t\tInvalid input! Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        if (option == 5) break;

        switch (option) {
            case 1:
                printf("\n\t\t\t\t### Total sales ###\n");
                displayList(head_s);
                break;
            case 2: {
                printf("\n\t\t\tEnter number of the food item: ");
                int num;
                if (scanf("%d", &num) != 1) {
                    printf("\n\t\t\tInvalid input! Please enter a valid number.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    continue;
                }

                printf("\t\t\t\tEnter food item name: ");
                char name[50];
                scanf(" %[^\n]", name);
                printf("\t\t\t\tEnter price: ");
                float price;
                if (scanf("%f", &price) != 1) {
                    printf("\n\t\t\tInvalid input! Please enter a valid price.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    continue;
                }

                heada = createadmin(heada, num, name, price);
                saveMenu();
                printf("\n\t\t\t\tNew food item added to the menu!\n");
                break;
            }
            case 3:
                if (deleteadmin()) {
                    printf("\n\t\t\t\tUpdated list of food items\n");
                    displayList(heada);
                    saveMenu();
                } else {
                    printf("\n\t\t\t\tFood item with given serial number doesn't exist!\n");
                }
                break;
            case 4:
                displayList(heada);
                break;
            default:
                printf("\n\t\t\t\tWrong Input!! Please choose a valid option.\n");
        }
    }
}

// Customer section function
void customer() {
    printf("\n\t\t\t\t----------------------------------------------\n");
    printf("\t\t\t\t\t\tCUSTOMER SECTION\n");
    printf("\n\t\t\t\t----------------------------------------------\n");
    while (1) {
        customermenu();
        int opt;
        if (scanf("%d", &opt) != 1) {
            printf("\n\t\t\tInvalid input! Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        if (opt == 5) break;

        switch (opt) {
            case 1: {
                printf("\n\t\t\t\t### Order menu ###\n");
                displayList(heada);
                printf("\t\t\t\tEnter serial number of food item you want to order: ");
                int num, quantity;
                if (scanf("%d", &num) != 1) {
                    printf("\n\t\t\tInvalid input! Please enter a valid number.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    continue;
                }

                printf("\t\t\t\tEnter the quantity: ");
                if (scanf("%d", &quantity) != 1) {
                    printf("\n\t\t\tInvalid input! Please enter a valid number.\n");
                    while (getchar() != '\n'); // Clear input buffer
                    continue;
                }

                headc = createcustomer(headc, num, quantity);
                break;
            }
            case 2:
                printf("\n\t\t\t\t### List of ordered items ###\n");
                displayList(headc);
                break;
            case 3:
                if (deletecustomer()) {
                    printf("\n\t\t\t\t### Updated list of your ordered food items ###\n");
                    displayList(headc);
                } else {
                    printf("\n\t\t\t\tFood item with given serial number doesn't exist in your order!\n\n");
                }
                break;
            case 4:
                printf("\n\t\t\t\t### Final bill ###\n");
                displaybill();
                calculatetotsales();
                headc = deleteList(headc);
                printf("\n\t\t\t\tPress any key to return to main menu: ");
                getchar();
                getchar();
                break;
            default:
                printf("\n\t\t\t\tWrong Input!! Please choose a valid option.\n");
        }
    }
}

// Main menu function
void mainmenu() {
    printf("\n\t\t\t\t*************************************************\n");
    printf("\t\t\t\t\t\tWELCOME TO MY RESTAURANT  SYSTEM\n");
    printf("\t\t\t\t*************************************************\n\n");
    printf("\t\t\t\t1. ADMIN SECTION\n");
    printf("\t\t\t\t2. CUSTOMER SECTION\n");
    printf("\t\t\t\t3. Exit\n\n");
    printf("\t\t\t\tEnter Your Choice ---> ");
}

// Main function
int main() {
    loadMenu();

    while (1) {
        mainmenu();
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("\n\t\t\tInvalid input! Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        if (choice == 3) {
            printf("\n\n\t\t\t\t\t\t\t********** Thank you!! **********\n");
            break;
        }

        switch (choice) {
            case 1:
                admin();
                break;
            case 2:
                customer();
                break;
            default:
                printf("\n\t\t\t\t\t\tWrong Input!! Please choose a valid option.\n");
        }
    }

    return 0;
}

