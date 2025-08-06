#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#define tab printf("\t\t")

struct List
{
    char name[100];
    char number[100];
    char email[200];
} list, check;

// All Function (Features)
void searchanime(char *a , int repeat);
void SortContacts();
void New_Contact();
void EditContact();
void SearchContact();
void AllContatc();
void DeleteContact();
void DeleteAllContacts();
void userguildline();
void aboutus();

void searchanime(char *a , int repeat)
{
    for (int r = 0; r < repeat; r++)
    {
        for (int i = 0; i < strlen(a); i++)
        {
            printf("%c", a[i]);
            fflush(stdout);
            Sleep(50);
        }
        printf("\r");
        fflush(stdout);
    }

    for (int i = 0; i < strlen(a); i++)
    {
        printf(" ");
    }
    printf("\r");
    fflush(stdout);
}

void SortContacts()
{
    FILE *Ffile = fopen("Contact.txt", "r");
    if (Ffile == NULL) {
        return;
    }

    // Count number of contacts
    int count = 0;
    while (fscanf(Ffile, "%s %s %s", list.name, list.number, list.email) != EOF) {
        count++;
    }
    rewind(Ffile);

    // Create array to hold all contacts
    struct List *contacts = malloc(count * sizeof(struct List));
    for (int i = 0; i < count; i++) {
        fscanf(Ffile, "%s %s %s", contacts[i].name, contacts[i].number, contacts[i].email);
    }
    fclose(Ffile);

    // Sort contacts alphabetically by name (due to ascii table )
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(contacts[i].name, contacts[j].name) > 0) {
                struct List temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp;
            }
        }
    }

    // Rewrite the sorted contacts back to file
    Ffile = fopen("Contact.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(Ffile, "%s %s %s\n", contacts[i].name, contacts[i].number, contacts[i].email);
    }
    fclose(Ffile);
    free(contacts);
}

void New_Contact()
{
    system("cls");
    FILE *Ffile = fopen("Contact.txt", "a+");
    if (Ffile == NULL)
    {
        printf("Error Opening File\n");
        return;
    }

    printf("\t\t\t\t\t==========================[ Add New Contact ]==========================\n");
    
    // Variables for duplicate checking
    char existingName[100], existingNumber[100], existingEmail[100];
    int isDuplicate;
    
    // Get name with duplicate check and validation
    do {
        isDuplicate = 0;
        printf("\n\t\t\t\t\tInput Name : ");
        fflush(stdin);
        fgets(list.name, sizeof(list.name), stdin);
        list.name[strcspn(list.name, "\n")] = '\0';
        
        // Check if first character is a number
        if (list.name[0] != '\0' && isdigit(list.name[0])) {
            printf("\n\t\t\t\t\tName cannot start with a number!\n");
            isDuplicate = 1;
            continue;
        }
        
        int i = 0;
        while(list.name[i] != '\0'){
            if(list.name[i]==' '){
                list.name[i] = '_';
            }
            i++;
        }
        
        // Check for duplicate name
        rewind(Ffile);
        while (fscanf(Ffile, "%s %s %s", existingName, existingNumber, existingEmail) != EOF) {
            if (strcmp(list.name, existingName) == 0) {
                printf("\n\t\t\t\t\tName already exists in contacts!\n");
                isDuplicate = 1;
                break;
            }
        }
    } while (isDuplicate);

    // Get phone number with validation and duplicate check
    do {
        isDuplicate = 0;
        printf("\n\t\t\t\t\tInput Phone Number : ");
        fflush(stdin);
        fgets(list.number, sizeof(list.number), stdin);
        list.number[strcspn(list.number, "\n")] = '\0';
        
        // Phone number validation
        int isValid = 1;
        for (int i = 0; list.number[i]; i++) {
            if (!isdigit(list.number[i]) && 
                list.number[i] != '+' && 
                list.number[i] != '-' && 
                list.number[i] != ' ') {
                printf("\n\t\t\t\t\tInvalid Phone Number (only digits, + or - allowed)\n");
                isValid = 0;
                break;
            }
        }
        
        if (!isValid) {
            isDuplicate = 1;
            continue;
        }
        
        // Check for duplicate number
        rewind(Ffile);
        while (fscanf(Ffile, "%s %s %s", existingName, existingNumber, existingEmail) != EOF) {
            if (strcmp(list.number, existingNumber) == 0) {
                printf("\n\t\t\t\t\tPhone number already exists in contacts!\n");
                isDuplicate = 1;
                break;
            }
        }
    } while (isDuplicate);

    // Get email with duplicate check
    do {
        isDuplicate = 0;
        printf("\n\t\t\t\t\tInput Email : ");
        fflush(stdin);
        fgets(list.email, sizeof(list.email), stdin);
        list.email[strcspn(list.email, "\n")] = '\0';
        
        // Simple email validation
        if (!strchr(list.email, '@') || !strchr(list.email, '.')) {
            printf("\n\t\t\t\t\tInvalid Email Address (must contain @ and .)\n");
            isDuplicate = 1;
            continue;
        }
        
        // Check for duplicate email
        rewind(Ffile);
        while (fscanf(Ffile, "%s %s %s", existingName, existingNumber, existingEmail) != EOF) {
            if (strcmp(list.email, existingEmail) == 0) {
                printf("\n\t\t\t\t\tEmail address already exists in contacts!\n");
                isDuplicate = 1;
                break;
            }
        }
    } while (isDuplicate);

    // Write the new contact to file
    fprintf(Ffile, "%s %s %s\n", list.name, list.number, list.email);
    fclose(Ffile);
    
    // Sort contacts after adding new one
    SortContacts();
    
    printf("\n\t\t\t\t\t\t\t     Successfully Added New Contact\n\n");
    printf("\t\t\t\t\t========================================================================\n");
    system("pause");
    system("cls");
}

void EditContact() {
    system("cls");
    printf("\t\t\t\t================================[  Edit Contact   ]===============================\n\n");

    FILE *Ffile = fopen("Contact.txt", "r");
    if (Ffile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Read all contacts into memory
    struct List contacts[1000];
    int count = 0;
    while (fscanf(Ffile, "%s %s %s", contacts[count].name, contacts[count].number, contacts[count].email) == 3) {
        count++;
    }
    fclose(Ffile);

    char searchName[100];
    printf("\t\t\t\tEnter the name to search: ");
    fflush(stdin);
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    // Replace spaces with underscores in searchName
    for (int i = 0; searchName[i] != '\0'; i++) {
        if (searchName[i] == ' ') {
            searchName[i] = '_';
        }
    }

    // Find the contact
    int contactIndex = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(searchName, contacts[i].name) == 0) {
            contactIndex = i;
            break;
        }
    }

    if (contactIndex == -1) {
        printf("\n\t\t\t\tContact cannot be found.\n");
        system("pause");
        return;
    }

    // Store original values
    char originalName[100], originalNumber[100], originalEmail[200];
    strcpy(originalName, contacts[contactIndex].name);
    strcpy(originalNumber, contacts[contactIndex].number);
    strcpy(originalEmail, contacts[contactIndex].email);

    int ch;
    do {
        system("cls");
        printf("\n");
        printf("\t\t\t\t==================================================================================\n");
        printf("\t\t\t\t=                                  Contact Found                                 =\n");
        printf("\t\t\t\t==================================================================================\n");
        printf("\t\t\t\tName : %s\tPhone Number : %s\tEmail : %s\t\n", 
               contacts[contactIndex].name, contacts[contactIndex].number, contacts[contactIndex].email);
        printf("\t\t\t\t==================================================================================\n\n");
        printf("\t\t\t\t================================[  Edit Contact   ]===============================\n\n");
        printf("\t\t\t\t\tWhat Do You Want to Edit\n");
        printf("\t\t\t\t\t1. Name\n");
        printf("\t\t\t\t\t2. Phone Number\n");
        printf("\t\t\t\t\t3. Email\n");
        printf("\t\t\t\t\t0. Save Changes and Exit\n\n");
        printf("\t\t\t\t==================================================================================\n\n");
        printf("\t\t\t\t\tInput Your Choice : ");
        scanf("%d", &ch);
        fflush(stdin);

        switch (ch) {
            case 1: { // Edit Name
                char newName[100];
                int isDuplicate, isSame, isValid;
                do {
                    isDuplicate = isSame = isValid = 0;
                    printf("\t\t\t\t\tEnter New Name: ");
                    fgets(newName, sizeof(newName), stdin);
                    newName[strcspn(newName, "\n")] = '\0';

                    // Replace spaces with underscores
                    for (int i = 0; newName[i] != '\0'; i++) {
                        if (newName[i] == ' ') {
                            newName[i] = '_';
                        }
                    }

                    if (strlen(newName) == 0) {
                        printf("\n\t\t\t\t\tName cannot be empty!\n");
                        isValid = 1;
                        continue;
                    }

                    if (strcmp(newName, originalName) == 0) {
                        printf("\n\t\t\t\t\tNew name cannot be the same as original!\n");
                        isSame = 1;
                        continue;
                    }

                    if (isdigit(newName[0])) {
                        printf("\n\t\t\t\t\tName cannot start with a number!\n");
                        isValid = 1;
                        continue;
                    }

                    for (int i = 0; newName[i]; i++) {
                        if (!isalpha(newName[i]) && newName[i] != '.' && newName[i] != '_' && !isdigit(newName[i])) {
                            printf("\n\t\t\t\t\tName can only contain letters, dots, underscores and numbers!\n");
                            isValid = 1;
                            break;
                        }
                    }

                    if (isValid) continue;

                    for (int i = 0; i < count; i++) {
                        if (i != contactIndex && strcmp(newName, contacts[i].name) == 0) {
                            printf("\n\t\t\t\t\tName already exists in contacts!\n");
                            isDuplicate = 1;
                            break;
                        }
                    }
                } while (isDuplicate || isSame || isValid);
                strcpy(contacts[contactIndex].name, newName);
                break;
            }

            case 2: { // Edit Phone Number
                char newNumber[100];
                int isDuplicate, isSame, isValid;
                do {
                    isDuplicate = isSame = isValid = 0;
                    printf("\t\t\t\t\tEnter New Phone Number: ");
                    fgets(newNumber, sizeof(newNumber), stdin);
                    newNumber[strcspn(newNumber, "\n")] = '\0';

                    if (strlen(newNumber) == 0) {
                        printf("\n\t\t\t\t\tPhone number cannot be empty!\n");
                        isValid = 1;
                        continue;
                    }

                    if (strcmp(newNumber, originalNumber) == 0) {
                        printf("\n\t\t\t\t\tPhone cannot be the same as original!\n");
                        isSame = 1;
                        continue;
                    }

                    for (int i = 0; newNumber[i]; i++) {
                        if (!isdigit(newNumber[i]) && newNumber[i] != '+' && newNumber[i] != '-') {
                            printf("\n\t\t\t\t\tInvalid Phone Number (only digits, + and - allowed)\n");
                            isValid = 1;
                            break;
                        }
                    }

                    if (isValid) continue;

                    if (newNumber[0] != '+' && !isdigit(newNumber[0])) {
                        printf("\n\t\t\t\t\tPhone must start with + or digit!\n");
                        isValid = 1;
                        continue;
                    }

                    int digits = 0;
                    for (int i = 0; newNumber[i]; i++) {
                        if (isdigit(newNumber[i])) digits++;
                    }
                    if (digits < 8 || digits > 15) {
                        printf("\n\t\t\t\t\tPhone number must have 8-15 digits!\n");
                        isValid = 1;
                        continue;
                    }

                    for (int i = 0; i < count; i++) {
                        if (i != contactIndex && strcmp(newNumber, contacts[i].number) == 0) {
                            printf("\n\t\t\t\t\tPhone number already exists!\n");
                            isDuplicate = 1;
                            break;
                        }
                    }
                } while (isDuplicate || isSame || isValid);
                strcpy(contacts[contactIndex].number, newNumber);
                break;
            }

            case 3: { // Edit Email
                char newEmail[200];
                int isDuplicate, isSame, isValid;
                do {
                    isDuplicate = isSame = isValid = 0;
                    printf("\t\t\t\t\tEnter New Email: ");
                    fgets(newEmail, sizeof(newEmail), stdin);
                    newEmail[strcspn(newEmail, "\n")] = '\0';

                    if (strlen(newEmail) == 0) {
                        printf("\n\t\t\t\t\tEmail cannot be empty!\n");
                        isValid = 1;
                        continue;
                    }

                    if (strcmp(newEmail, originalEmail) == 0) {
                        printf("\n\t\t\t\t\tEmail cannot be the same as original!\n");
                        isSame = 1;
                        continue;
                    }

                    int atCount = 0, dotCount = 0;
                    for (int i = 0; newEmail[i]; i++) {
                        if (newEmail[i] == '@') atCount++;
                        if (newEmail[i] == '.') dotCount++;

                        if (!isalnum(newEmail[i]) && newEmail[i] != '@' && newEmail[i] != '.' &&
                            newEmail[i] != '_' && newEmail[i] != '-') {
                            printf("\n\t\t\t\t\tInvalid character in email!\n");
                            isValid = 1;
                            break;
                        }
                    }

                    if (isValid) continue;

                    if (atCount != 1 || dotCount == 0) {
                        printf("\n\t\t\t\t\tInvalid Email format (must contain exactly one @ and at least one . after @)\n");
                        isValid = 1;
                        continue;
                    }

                    if (newEmail[0] == '@' || newEmail[strlen(newEmail)-1] == '@' ||
                        newEmail[0] == '.' || newEmail[strlen(newEmail)-1] == '.') {
                        printf("\n\t\t\t\t\tInvalid Email format (@ or . at wrong position)\n");
                        isValid = 1;
                        continue;
                    }

                    char *atPos = strchr(newEmail, '@');
                    char *dotPos = strrchr(newEmail, '.');
                    if (dotPos < atPos) {
                        printf("\n\t\t\t\t\tInvalid Email format (. must come after @)\n");
                        isValid = 1;
                        continue;
                    }

                    for (int i = 0; i < count; i++) {
                        if (i != contactIndex && strcmp(newEmail, contacts[i].email) == 0) {
                            printf("\n\t\t\t\t\tEmail already exists!\n");
                            isDuplicate = 1;
                            break;
                        }
                    }
                } while (isDuplicate || isSame || isValid);
                strcpy(contacts[contactIndex].email, newEmail);
                break;
            }

            case 0:
                break;

            default:
                printf("\t\t\t\t\tInvalid Choice\n");
                system("pause");
        }
    } while (ch != 0);

    // Save changes
    Ffile = fopen("Contact.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(Ffile, "%s %s %s\n", contacts[i].name, contacts[i].number, contacts[i].email);
    }
    fclose(Ffile);

    printf("\n\t\t\t\tContact updated successfully!\n");
    system("pause");
}

void SearchContact()
{
    FILE *Ffile;
    Ffile = fopen("Contact.txt", "r");
    int found = 0;
    system("cls");

    tab;
    printf("\t\t\t=============================[  Search For Contact  ]============================\n\n");
    printf("\t\t\t\t\tEnter the name to search: ");
    fflush(stdin);
    fgets(check.name, sizeof(check.name), stdin);
    check.name[strcspn(check.name, "\n")] = '\0'; // Remove newline character

    // Replace spaces with underscores
    for (int i = 0; check.name[i] != '\0'; i++) {
        if (check.name[i] == ' ') {
            check.name[i] = '_';
        }
    }

    printf("\n");
    searchanime("Searching....", 2);

    while (fscanf(Ffile, "%s %s %s", list.name, list.number, list.email) != EOF)
    {
        if (strcmp(check.name, list.name) == 0)
        {
            found = 1;
            printf("\t\t\t\t\t=================================================================================\n");
            printf("\t\t\t\t\t\t                          Your Contact                               \n");
            printf("\t\t\t\t\t=================================================================================\n");
            printf("\t\t\t\t\tName : %s\tPhone Number : %s\tEmail : %s\t\n", list.name, list.number, list.email);
            printf("\t\t\t\t\t=================================================================================\n");
            system("pause");
            break;
        }
    }

    fclose(Ffile);

    if (found == 0)
    {
        printf("\n\t\t\t\t\tContact Cannot Be Found\n");
        system("pause");
    }
}


void AllContatc()
{
    int found = 0;
    FILE *Ffile = fopen("Contact.txt", "r");
    if (Ffile == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    searchanime("Loading....." , 1);
    system("cls");

    
    // Sort contacts before displaying
    SortContacts();
    
    printf("\t\t\t\t================================[  Show All ConTact   ]===============================\n\n");
    printf("\t\t\t\t\t%-25s %-30s %-20s\n\n", "NAME", "Phone Number", "Email");
    while (fscanf(Ffile, "%s %s %s", list.name, list.number, list.email) != EOF)
    {
        found = 1;
        printf("\t\t\t\t\t%-25s %-30s %-25s\n\n", list.name, list.number, list.email);
    }
    printf("\t\t\t\t=====================================================================================\n\n");

    fclose(Ffile);
    if (found == 0)
        printf("\t\t\t\t\tCannot Find Any Record\n");
    system("pause");
}

void DeleteContact()
{
    system("cls");
    printf("\t\t\t\t================================[ Delete Contact ]================================\n\n");
    
    struct List contacts[1000];
    int count = 0;
    
    FILE *Ffile = fopen("Contact.txt", "r");
    if (Ffile == NULL) {
        printf("\t\t\t\tError opening contacts file!\n");
        system("pause");
        return;
    }
    
    while (fscanf(Ffile, "%s %s %s", contacts[count].name, contacts[count].number, contacts[count].email) == 3) {
        count++;
    }
    fclose(Ffile);
    
    if (count == 0) {
        printf("\t\t\t\tNo contacts found to delete!\n");
        system("pause");
        return;
    }
    
    // Get contact name to delete
    char nameToDelete[100];
    printf("\t\t\t\tEnter the name to delete: ");
    fflush(stdin);
    fgets(nameToDelete, sizeof(nameToDelete), stdin);
    nameToDelete[strcspn(nameToDelete, "\n")] = '\0';

    // Replace spaces with underscores
    for (int i = 0; nameToDelete[i] != '\0'; i++) {
        if (nameToDelete[i] == ' ') {
            nameToDelete[i] = '_';  
        }
    }

    printf("\n");

    // Find the contact
    int contactIndex = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(nameToDelete, contacts[i].name) == 0) {
            contactIndex = i;
            break;
        }
    }
    
    if (contactIndex == -1) {
        printf("\t\t\t\tContact not found!\n");
        system("pause");
        return;
    }

    // Show contact details before deletion
    printf("\t\t\t\tContact Found:\n");
    printf("\t\t\t\tName : %s\n", contacts[contactIndex].name);
    printf("\t\t\t\tPhone: %s\n", contacts[contactIndex].number);
    printf("\t\t\t\tEmail: %s\n\n", contacts[contactIndex].email);

    // Double confirmation
    char confirm;
    printf("\t\t\t\tAre you sure you want to delete this contact? (y/n): ");
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        FILE *tempFile = fopen("Temp.txt", "w");
        if (tempFile == NULL) {
            printf("\t\t\t\tError creating temporary file!\n");
            system("pause");
            return;
        }

        for (int i = 0; i < count; i++) {
            if (i != contactIndex) {
                fprintf(tempFile, "%s %s %s\n", contacts[i].name, contacts[i].number, contacts[i].email);
            }
        }

        fclose(tempFile);
        remove("Contact.txt");
        rename("Temp.txt", "Contact.txt");

        printf("\n\t\t\t\tContact deleted successfully!\n");
    } else {
        printf("\n\t\t\t\tDeletion canceled.\n");
    }

    system("pause");
}


void DeleteAllContacts()
{
    char confirm;

    printf("\t\t\t\t\t\t\tAre you sure you want to delete ALL contacts? (y/n): ");
    fflush(stdin);
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y')
    {
        FILE *Ffile = fopen("Contact.txt", "w");
        if (Ffile == NULL)
        {
            printf("Error opening file.\n");
            return;
        }

        fclose(Ffile);
        searchanime("Deleting All Contact..." , 1);
        printf("\t\t\t\t\t\t\tAll contacts deleted successfully.\n");
    }
    else
    {
        printf("\t\t\t\t\t\t\tDeletion canceled. No contacts were deleted.\n");
    }

    system("pause");
}

void userguildline()
{
    system("cls");
    searchanime("Loading..." , 2);
    printf("========================[ TermiContact User Guideline ]========================\n\n");
    printf("This app lets you manage your contacts easily through a simple menu system.\n\n");
    printf("1.Add New Contact     -> Input name, phone number, and email. No duplicates.\n");
    printf("2.Edit Contact        -> Change a contact's name, phone number, or email.\n");
    printf("3.Show All Contacts   -> Displays all saved contacts in a list format.\n");
    printf("4.Search for Contact  -> Find a contact by typing its name.\n");
    printf("5.Delete Contact      -> Remove a contact by name.\n");
    printf("6.Delete All Contacts -> WARNING: Permanently deletes all saved contacts.\n");
    printf("7.User Guideline      -> Shows this help page.\n");
    printf("8.About Us            -> Information about the creator and contact links.\n");
    printf("0.Exit                -> Exit the application with confirmation.\n\n");
    printf("===============================================================================\n");
    printf("Contact support: https://t.me/Senghab68\n");
    char choice;
    printf("Do You want to Scan the QR Code for Quick Access (Y/N)? ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        system("start contactus/contactus.png");
    }
    printf("===============================================================================\n");
    system("pause");
}

void aboutus()
{
    system("cls");
    printf("===================================================================================\n");
    printf("=                      About Us - TermiContact                                    =\n");
    printf("===================================================================================\n\n");

    printf(" Project Name : TermiContact\n");
    printf(" Description  : A simple contact management system\n");
    printf(" Version      : 1.0\n");
    printf(" Developed by Cambodia Academy of Digital Technology (CADT) FreshMan Students\n");

    printf("-----------------------------------------------------------------------------------\n");
    printf(" Meet the Team:\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf(" Name       : Chay SengHap\n");
    printf(" Major      : Computer Science\n");
    printf(" Role       : Developer & FileHandling\n");
    printf(" Telegram   : https://t.me/Senghab68\n");

    printf(" Name       : Am Vathanak\n");
    printf(" Major      : Telecom & Networking\n");
    printf(" Role       : Developer & Tester\n");
    printf(" Telegram   : https://t.me/nakkk47\n");
    printf("-----------------------------------------------------------------------------------\n");

    printf(" For support or questions, contact us via Telegram.\n");
    printf(" You can also scan the QR code for quick access.\n");

    system("start contactus/contactus.png");

    printf("===================================================================================\n");
    system("pause");
}

int main()
{
    system("color 1");
    int option;
    char confirm;
    system("cls");
    printf("\t\t\t\t\t\t\t");
    char app[] = "Welcome To TermiContact...........";
    for (int i = 0; i < strlen(app); i++)
    {
        printf("%c", app[i]);
        Sleep(20);
    }
    printf("\n");
    do
    {
        system("cls");
        printf("\t\t\t\t\t\t\t================================\n");
        printf("\t\t\t\t\t\t\t=====     TermiContact     =====\n");
        printf("\t\t\t\t\t\t\t================================\n");
        printf("\t\t\t\t\t\t\t=            MENU              =\n");
        printf("\t\t\t\t\t\t\t= 1. Add New Contact           =\n");
        printf("\t\t\t\t\t\t\t= 2. Edit Contact              =\n");
        printf("\t\t\t\t\t\t\t= 3. Show All Contacts         =\n");
        printf("\t\t\t\t\t\t\t= 4. Search Contact            =\n");
        printf("\t\t\t\t\t\t\t= 5. Delete Contact            =\n");
        printf("\t\t\t\t\t\t\t= 6. Delete All Contacts       =\n");
        printf("\t\t\t\t\t\t\t= 7. User Guide                =\n");
        printf("\t\t\t\t\t\t\t= 8. About Us                  =\n");
        printf("\t\t\t\t\t\t\t= 0. Exit                      =\n");
        printf("\t\t\t\t\t\t\t================================\n");
        printf("\t\t\t\t\t\t\tEnter an Option : ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            New_Contact();
            break;
        case 2:
            EditContact();
            break;
        case 3:
            AllContatc();
            break;
        case 4:
            SearchContact();
            break;
        case 5:
            DeleteContact();
            break;
        case 6:
            DeleteAllContacts();
            break;
        case 7:
            userguildline();
            break;
        case 8:
            aboutus();
            break;
        case 0:
            printf("\n\t\t\t\t\t\tAre you sure you want to exit? (y/n): ");
            scanf(" %c", &confirm);
            if (confirm != 'y' && confirm != 'Y')
            {
                option = -1;
            }
            break;
        default:
            printf("\n\t\t\t\t\t\tInvalid option! Please try again.\n");
            system("pause");
        }
    } while (option != 0);

    system("cls");
    printf("\n\n");
    searchanime("Exiting..." , 2);
    printf("\t\t\t\t\t\t================================\n");
    printf("\t\t\t\t\t\t=                              =\n");
    printf("\t\t\t\t\t\t=      THANK YOU FOR USING     =\n");
    printf("\t\t\t\t\t\t=                              =\n");
    printf("\t\t\t\t\t\t=        TERMICONTACT          =\n");
    printf("\t\t\t\t\t\t=                              =\n");
    printf("\t\t\t\t\t\t=    We appreciate your time!  =\n");
    printf("\t\t\t\t\t\t=                              =\n");
    printf("\t\t\t\t\t\t================================\n");

    return 0;
}