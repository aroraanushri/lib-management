#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_USERS 50
#define MAX_STRING 50
#define MAX_ISSUED 200

// Book structure
typedef struct {
    int id;
    char name[MAX_STRING];
    char author[MAX_STRING];
    char category[MAX_STRING];
    int available;
} Book;

// User structure
typedef struct {
    int id;
    char name[MAX_STRING];
    char address[MAX_STRING];
    char phone[15];
} User;

// Issue record structure
typedef struct {
    int id;
    int book_id;
    int user_id;
    char issue_date[20];
    char return_date[20];
    int returned;
} Issue;

// Global data structures
Book books[MAX_BOOKS];
User users[MAX_USERS];
Issue issues[MAX_ISSUED];
int book_count = 0;
int user_count = 0;
int issue_count = 0;

// Function prototypes
void print_menu();
int get_valid_int();
void get_valid_string(char *str, int maxLength);
void format_string(char *str);
void add_book();
void delete_book();
void search_books();
void add_user();
void edit_user();
void issue_book();
void return_book();
void get_current_date(char *date_str);
void save_data();
void load_data();

int main() {
    int choice;
    
    // Load data from files
    load_data();
    
    while (1) {
        print_menu();
        choice = get_valid_int();
        
        switch (choice) {
            case 1:
                add_book();
                break;
            case 2:
                delete_book();
                break;
            case 3:
                search_books();
                break;
            case 4:
                add_user();
                break;
            case 5:
                edit_user();
                break;
            case 6:
                issue_book();
                break;
            case 7:
                return_book();
                break;
            case 8:
                save_data();
                printf("Exiting... Thank you!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}

// Function to display main menu
void print_menu() {
    printf("\n===== Library Management System =====\n");
    printf("1. Add Book\n");
    printf("2. Delete Book\n");
    printf("3. Search Books\n");
    printf("4. Add User\n");
    printf("5. Edit User\n");
    printf("6. Issue Book\n");
    printf("7. Return Book\n");
    printf("8. Exit\n");
    printf("=====================================\n");
    printf("Enter your choice: ");
}

// Function to get a validated integer input
int get_valid_int() {
    int num;
    while (scanf("%d", &num) != 1) {
        printf("Invalid input. Please enter a number: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    getchar(); // Prevents accidental input skipping in next scanf
    return num;
}

// Function to get a validated string input
void get_valid_string(char *str, int maxLength) {
    fgets(str, maxLength, stdin);
    str[strcspn(str, "\n")] = '\0'; // Remove newline character
}

// Function to format strings (capitalizes first letter of each word)
void format_string(char *str) {
    for (int i = 0; str[i]; i++) {
        if (i == 0 || str[i - 1] == ' ') {
            str[i] = toupper(str[i]);
        } else {
            str[i] = tolower(str[i]);
        }
    }
}

// Function to add a new book
void add_book() {
    if (book_count >= MAX_BOOKS) {
        printf("Error: Maximum book limit reached.\n");
        return;
    }
    
    Book new_book;
    new_book.id = book_count + 1;
    new_book.available = 1;
    
    printf("\n==== Add New Book ====\n");
    printf("Enter book name: ");
    get_valid_string(new_book.name, MAX_STRING);
    format_string(new_book.name);
    
    printf("Enter author name: ");
    get_valid_string(new_book.author, MAX_STRING);
    format_string(new_book.author);
    
    printf("Enter category: ");
    get_valid_string(new_book.category, MAX_STRING);
    format_string(new_book.category);
    
    books[book_count++] = new_book;
    printf("Book added successfully! ID: %d\n", new_book.id);
}

// Function to delete a book
void delete_book() {
    int id, found = 0;
    
    printf("\n==== Delete Book ====\n");
    printf("Enter book ID to delete: ");
    id = get_valid_int();
    
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) {
            // Check if book is currently issued
            int issued = 0;
            for (int j = 0; j < issue_count; j++) {
                if (issues[j].book_id == id && issues[j].returned == 0) {
                    issued = 1;
                    break;
                }
            }
            
            if (issued) {
                printf("Error: Book is currently issued and cannot be deleted.\n");
                return;
            }
            
            // Delete book by shifting array
            for (int j = i; j < book_count - 1; j++) {
                books[j] = books[j + 1];
            }
            book_count--;
            printf("Book deleted successfully!\n");
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Book with ID %d not found.\n", id);
    }
}

// Function to search for books
void search_books() {
    int choice;
    char search_term[MAX_STRING];
    int found = 0;
    
    printf("\n==== Search Books ====\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("3. Search by Author\n");
    printf("4. Search by Category\n");
    printf("5. Show All Books\n");
    printf("Enter your choice: ");
    
    choice = get_valid_int();
    
    if (choice >= 1 && choice <= 4) {
        printf("Enter search term: ");
        get_valid_string(search_term, MAX_STRING);
        if (choice != 1) { // Format string for non-ID searches
            format_string(search_term);
        }
    }
    
    printf("\n%-5s %-30s %-20s %-15s %-10s\n", "ID", "Book Name", "Author", "Category", "Status");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < book_count; i++) {
        int match = 0;
        
        switch (choice) {
            case 1: // ID
                if (books[i].id == atoi(search_term)) {
                    match = 1;
                }
                break;
            case 2: // Name
                if (strstr(books[i].name, search_term) != NULL) {
                    match = 1;
                }
                break;
            case 3: // Author
                if (strstr(books[i].author, search_term) != NULL) {
                    match = 1;
                }
                break;
            case 4: // Category
                if (strstr(books[i].category, search_term) != NULL) {
                    match = 1;
                }
                break;
            case 5: // All books
                match = 1;
                break;
            default:
                printf("Invalid choice.\n");
                return;
        }
        
        if (match) {
            printf("%-5d %-30s %-20s %-15s %-10s\n", 
                books[i].id, 
                books[i].name, 
                books[i].author, 
                books[i].category, 
                books[i].available ? "Available" : "Issued");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No matching books found.\n");
    }
}

// Function to add a new user
void add_user() {
    if (user_count >= MAX_USERS) {
        printf("Error: Maximum user limit reached.\n");
        return;
    }
    
    User new_user;
    new_user.id = user_count + 1;
    
    printf("\n==== Add New User ====\n");
    printf("Enter user name: ");
    get_valid_string(new_user.name, MAX_STRING);
    format_string(new_user.name);
    
    printf("Enter address: ");
    get_valid_string(new_user.address, MAX_STRING);
    
    printf("Enter phone number: ");
    get_valid_string(new_user.phone, 15);
    
    users[user_count++] = new_user;
    printf("User added successfully! ID: %d\n", new_user.id);
}

// Function to edit a user
void edit_user() {
    int id, found = 0;
    
    printf("\n==== Edit User ====\n");
    printf("Enter user ID to edit: ");
    id = get_valid_int();
    
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == id) {
            printf("Current User Details:\n");
            printf("Name: %s\n", users[i].name);
            printf("Address: %s\n", users[i].address);
            printf("Phone: %s\n", users[i].phone);
            
            printf("\nEnter new name (press enter to keep current): ");
            char temp[MAX_STRING];
            get_valid_string(temp, MAX_STRING);
            if (strlen(temp) > 0) {
                format_string(temp);
                strcpy(users[i].name, temp);
            }
            
            printf("Enter new address (press enter to keep current): ");
            get_valid_string(temp, MAX_STRING);
            if (strlen(temp) > 0) {
                strcpy(users[i].address, temp);
            }
            
            printf("Enter new phone (press enter to keep current): ");
            get_valid_string(temp, 15);
            if (strlen(temp) > 0) {
                strcpy(users[i].phone, temp);
            }
            
            printf("User information updated successfully!\n");
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("User with ID %d not found.\n", id);
    }
}

// Function to get current date in string format
void get_current_date(char *date_str) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(date_str, 20, "%Y-%m-%d", tm);
}

// Function to issue a book
void issue_book() {
    if (issue_count >= MAX_ISSUED) {
        printf("Error: Maximum issue records limit reached.\n");
        return;
    }
    
    int book_id, user_id;
    int book_found = 0, user_found = 0, book_index = -1;
    
    printf("\n==== Issue Book ====\n");
    
    // Show all available books
    printf("\nAvailable Books:\n");
    printf("%-5s %-30s %-20s\n", "ID", "Name", "Author");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < book_count; i++) {
        if (books[i].available) {
            printf("%-5d %-30s %-20s\n", books[i].id, books[i].name, books[i].author);
        }
    }
    
    printf("\nEnter book ID to issue: ");
    book_id = get_valid_int();
    
    // Validate book
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            book_found = 1;
            book_index = i;
            
            if (!books[i].available) {
                printf("Error: Book is already issued.\n");
                return;
            }
            break;
        }
    }
    
    if (!book_found) {
        printf("Error: Book with ID %d not found.\n", book_id);
        return;
    }
    
    // Show all users
    printf("\nUsers:\n");
    printf("%-5s %-30s %-15s\n", "ID", "Name", "Phone");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < user_count; i++) {
        printf("%-5d %-30s %-15s\n", users[i].id, users[i].name, users[i].phone);
    }
    
    printf("\nEnter user ID: ");
    user_id = get_valid_int();
    
    // Validate user
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == user_id) {
            user_found = 1;
            break;
        }
    }
    
    if (!user_found) {
        printf("Error: User with ID %d not found.\n", user_id);
        return;
    }
    
    // Create issue record
    Issue new_issue;
    new_issue.id = issue_count + 1;
    new_issue.book_id = book_id;
    new_issue.user_id = user_id;
    new_issue.returned = 0;
    
    get_current_date(new_issue.issue_date);
    strcpy(new_issue.return_date, "Not returned");
    
    issues[issue_count++] = new_issue;
    books[book_index].available = 0;
    
    printf("Book issued successfully! Issue ID: %d\n", new_issue.id);
}

// Function to return a book
void return_book() {
    int issue_id, found = 0;
    
    printf("\n==== Return Book ====\n");
    
    // Show all issued books
    printf("\nCurrently Issued Books:\n");
    printf("%-5s %-5s %-30s %-20s %-15s\n", "Issue", "Book", "Book Name", "User Name", "Issued Date");
    printf("-------------------------------------------------------------------------\n");
    
    for (int i = 0; i < issue_count; i++) {
        if (!issues[i].returned) {
            char book_name[MAX_STRING] = "Unknown";
            char user_name[MAX_STRING] = "Unknown";
            
            // Find book name
            for (int j = 0; j < book_count; j++) {
                if (books[j].id == issues[i].book_id) {
                    strcpy(book_name, books[j].name);
                    break;
                }
            }
            
            // Find user name
            for (int j = 0; j < user_count; j++) {
                if (users[j].id == issues[i].user_id) {
                    strcpy(user_name, users[j].name);
                    break;
                }
            }
            
            printf("%-5d %-5d %-30s %-20s %-15s\n", 
                  issues[i].id, issues[i].book_id, book_name, user_name, issues[i].issue_date);
        }
    }
    
    printf("\nEnter issue ID to return: ");
    issue_id = get_valid_int();
    
    for (int i = 0; i < issue_count; i++) {
        if (issues[i].id == issue_id && !issues[i].returned) {
            issues[i].returned = 1;
            get_current_date(issues[i].return_date);
            
            // Update book status
            for (int j = 0; j < book_count; j++) {
                if (books[j].id == issues[i].book_id) {
                    books[j].available = 1;
                    break;
                }
            }
            
            printf("Book returned successfully!\n");
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Issue record with ID %d not found or already returned.\n", issue_id);
    }
}

// Function to save data to files
void save_data() {
    FILE *book_file = fopen("books.dat", "wb");
    FILE *user_file = fopen("users.dat", "wb");
    FILE *issue_file = fopen("issues.dat", "wb");
    
    if (book_file == NULL || user_file == NULL || issue_file == NULL) {
        printf("Error: Could not open files for saving data.\n");
        return;
    }
    
    fwrite(&book_count, sizeof(int), 1, book_file);
    fwrite(books, sizeof(Book), book_count, book_file);
    
    fwrite(&user_count, sizeof(int), 1, user_file);
    fwrite(users, sizeof(User), user_count, user_file);
    
    fwrite(&issue_count, sizeof(int), 1, issue_file);
    fwrite(issues, sizeof(Issue), issue_count, issue_file);
    
    fclose(book_file);
    fclose(user_file);
    fclose(issue_file);
    
    printf("Data saved successfully!\n");
}

// Function to load data from files
void load_data() {
    FILE *book_file = fopen("books.dat", "rb");
    FILE *user_file = fopen("users.dat", "rb");
    FILE *issue_file = fopen("issues.dat", "rb");
    
    if (book_file != NULL) {
        fread(&book_count, sizeof(int), 1, book_file);
        fread(books, sizeof(Book), book_count, book_file);
        fclose(book_file);
    }
    
    if (user_file != NULL) {
        fread(&user_count, sizeof(int), 1, user_file);
        fread(users, sizeof(User), user_count, user_file);
        fclose(user_file);
    }
    
    if (issue_file != NULL) {
        fread(&issue_count, sizeof(int), 1, issue_file);
        fread(issues, sizeof(Issue), issue_count, issue_file);
        fclose(issue_file);
    }
    
    printf("Data loaded successfully!\n");
}
