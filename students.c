#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOK_NAME 50
#define MAX_AUTHOR_NAME 50
#define MAX_STUDENT_NAME 50
#define MAX_CLASS_NAME 50
#define USERNAME "admin"
#define PASSWORD "52662885"

typedef struct
{
    int id;
    char bookName[MAX_BOOK_NAME];
    char authorName[MAX_AUTHOR_NAME];
    char date[12];
    int isIssued;
} Book;

typedef struct
{
    int id;
    char sName[MAX_STUDENT_NAME];
    char sClass[MAX_CLASS_NAME];
    int sRoll;
    char bookName[MAX_BOOK_NAME];
    char date[12];
} IssuedBook;

FILE *booksFile;
FILE *issueFile;

void addBook();
void booksList();
void removeBook();
void issueBook();
void issueList();
void login();

int main()
{
    printf("\n\n\t =====WELCOME TO THE C LIBARY MANAGEMENT SYSTEM=====\t\n\n\n");
    login();

    int choice;

    booksFile = fopen("books.txt", "rb+");
    if (booksFile == NULL)
    {
        booksFile = fopen("books.txt", "wb+");
    }

    issueFile = fopen("issue.txt", "rb+");
    if (issueFile == NULL)
    {
        issueFile = fopen("issue.txt", "wb+");
    }

    while (1)
    {
        system("cls");
        printf("<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. Books List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Book List\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            fclose(booksFile);
            fclose(issueFile);
            exit(0);

        case 1:
            addBook();
            break;

        case 2:
            booksList();
            break;

        case 3:
            removeBook();
            break;

        case 4:
            issueBook();
            break;

        case 5:
            issueList();
            break;

        default:
            printf("Invalid Choice...\n\n");
        }
        printf("Press Any Key To Continue...");
        getchar();
        getchar();
    }

    return 0;
}

void login()
{
    char username[MAX_STUDENT_NAME];
    char password[MAX_STUDENT_NAME];
    int attempts = 3;

    while (attempts > 0)
    {
        printf("Enter username: ");
        fflush(stdin);
        if (fgets(username, MAX_STUDENT_NAME, stdin) == NULL)
        {
            printf("Error reading username.\n");
            return;
        }
        username[strcspn(username, "\n")] = '\0'; // Remove newline character

        printf("Enter password: ");
        fflush(stdin);
        if (fgets(password, MAX_STUDENT_NAME, stdin) == NULL)
        {
            printf("Error reading password.\n");
            return;
        }
        password[strcspn(password, "\n")] = '\0'; // Remove newline character

        if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0)
        {
            printf("Login successful!\n");
            return;
        }
        else
        {
            printf("Invalid username or password. Please try again.\n");
            attempts--;
        }
    }

    printf("Too many failed attempts. Exiting...\n");
    exit(0);
}

void addBook()
{
    Book newBook;
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(newBook.date, myDate);

    printf("Enter book id: ");
    scanf("%d", &newBook.id);

    printf("Enter book name: ");
    fflush(stdin);
    if (fgets(newBook.bookName, MAX_BOOK_NAME, stdin) == NULL)
    {
        printf("Error reading book name.\n");
        return;
    }
    newBook.bookName[strcspn(newBook.bookName, "\n")] = '\0'; // Remove newline character

    printf("Enter author name: ");
    fflush(stdin);
    if (fgets(newBook.authorName, MAX_AUTHOR_NAME, stdin) == NULL)
    {
        printf("Error reading author name.\n");
        return;
    }
    newBook.authorName[strcspn(newBook.authorName, "\n")] = '\0'; // Remove newline character

    newBook.isIssued = 0;

    fwrite(&newBook, sizeof(Book), 1, booksFile);
    printf("Book Added Successfully\n");
}

void booksList()
{
    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-20s %-10s %s\n\n", "Book id", "Book Name", "Author", "Issued", "Date");

    rewind(booksFile);
    Book book;
    while (fread(&book, sizeof(Book), 1, booksFile) == 1)
    {
        printf("%-10d %-30s %-20s %-10s %s\n", book.id, book.bookName, book.authorName, book.isIssued ? "Yes" : "No", book.date);
    }
}

void removeBook()
{
    int id, found = 0;
    system("cls");
    printf("<== Remove Books ==>\n\n");
    printf("Enter Book id to remove: ");
    scanf("%d", &id);

    FILE *tempFile = fopen("temp.txt", "wb");
    if (tempFile == NULL)
    {
        printf("Error opening temporary file!\n");
        return;
    }

    rewind(booksFile);
    Book book;
    while (fread(&book, sizeof(Book), 1, booksFile) == 1)
    {
        if (id == book.id)
        {
            found = 1;
        }
        else
        {
            fwrite(&book, sizeof(Book), 1, tempFile);
        }
    }

    fclose(booksFile);
    fclose(tempFile);

    if (found)
    {
        printf("\n\nBook Deleted Successfully.\n");
        remove("books.txt");
        rename("temp.txt", "books.txt");
        booksFile = fopen("books.txt", "rb+");
    }
    else
    {
        printf("\n\nBook Not Found!\n");
        remove("temp.txt");
    }
}

void issueBook()
{
    int bookId, found = 0;
    printf("Enter Book id to issue: ");
    scanf("%d", &bookId);

    rewind(booksFile);
    Book book;
    while (fread(&book, sizeof(Book), 1, booksFile) == 1)
    {
        if (bookId == book.id && !book.isIssued)
        {
            found = 1;
            book.isIssued = 1;
            fseek(booksFile, sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, booksFile);
            break;
        }
    }

    if (!found)
    {
        printf("Book not found or already issued.\n");
        return;
    }

    IssuedBook issued;
    printf("Enter Student Name: ");
    fflush(stdin);
    if (fgets(issued.sName, MAX_STUDENT_NAME, stdin) == NULL)
    {
        printf("Error reading student name.\n");
        return;
    }
    issued.sName[strcspn(issued.sName, "\n")] = '\0'; // Remove newline character

    printf("Enter Student Class: ");
    fflush(stdin);
    if (fgets(issued.sClass, MAX_CLASS_NAME, stdin) == NULL)
    {
        printf("Error reading student class.\n");
        return;
    }
    issued.sClass[strcspn(issued.sClass, "\n")] = '\0'; // Remove newline character

    printf("Enter Student Roll: ");
    scanf("%d", &issued.sRoll);

    issued.id = book.id;
    strcpy(issued.bookName, book.bookName);
    strcpy(issued.date, book.date);

    fwrite(&issued, sizeof(IssuedBook), 1, issueFile);
    printf("Book Issued Successfully.\n");
}

void issueList()
{
    system("cls");
    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "S.id", "Name", "Class", "Roll", "Book Name", "Date");

    rewind(issueFile);
    IssuedBook issued;
    while (fread(&issued, sizeof(IssuedBook), 1, issueFile) == 1)
    {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", issued.id, issued.sName, issued.sClass, issued.sRoll, issued.bookName, issued.date);
    }
}