#include <stdio.h>
#include <stdlib.h>

struct studentData {
	unsigned int stdntNum;
	char lastName[20];
	char firstName[20];
	double math;
	double science;
	double technology;
};

//prototypes
unsigned int enterChoice(void);

void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayRecord(FILE *fPtr);

int main (void) 
{
	FILE *cfPtr; // credit.dat file pointer
	unsigned int choice;
	
	// fopen opens the file
	if ( ( cfPtr = fopen("grades.dat", "rb+") ) == NULL) {
		puts("File could not be opened");
	}
	else {
		
		do {
			
			switch( choice ) {
				
				// create text file from record file / save changes to text file
				case 1: 
					textFile( cfPtr );
					break;
				// add / subtract grades
				case 2:
					updateRecord( cfPtr );
					break;
				// add new record of student
				case 3:
					newRecord( cfPtr );
					break;
				// delete record of student
				case 4:
					deleteRecord( cfPtr );
					break;
				// display all records
				case 5:
					displayRecord( cfPtr );
					break;
				default:
					puts("Incorrect choice");
					break;
				}
			} while ( ( choice = enterChoice() )!= 6);
			
		
		fclose( cfPtr );
	}
}

unsigned int enterChoice( void )
{
	unsigned int menuChoice;
	
	 printf( "%s", "\nEnter your choice\n"
	 "1 - store a formatted text file of accounts called\n"
	 " \"students.txt\" for printing\n"
	 "2 - update an account\n"
	 "3 - add a new account\n"
	 "4 - delete an account\n"
	 "5 - display the records\n"
	 "6 - end program\n? " );
	 
	scanf( "%u", &menuChoice ); // receive choice from user
	return menuChoice;	
}

// create formatted text file for printing
void textFile(FILE *readPtr)
{
	FILE *writePtr;
	int result;
	
	struct studentData student = {0, "", "", 0.0, 0.0, 0.0};
	
	// fopen opens the file
	if ( (writePtr = fopen("students.txt", "w") ) == NULL ) {
		puts("File could not be opened");
	}
	else {
		rewind( readPtr ); // sets the pointer to beinning file
		fprintf(writePtr, "%-10s%-20s%-20s%-10s%-7s%10s\n", "Account", "Last Name", "First Name", "Science", "Math", "Technology");
		
		// copy all records from random-access file into text file
		while ( !feof( readPtr ) ) {
			result = fread( &student, sizeof( struct studentData ), 1, readPtr);
			
			// write single record to text file
			if ( result != 0 && student.stdntNum != 0) {
				fprintf( writePtr, "%-10d%-20s%-20s%5.2f%8.2f%10.2f\n", student.stdntNum, student.lastName, student.firstName, student.science, student.math, student.technology);
			}
		}
		
	fclose( writePtr );	
	}
} // end of function textFile

// update student record
void updateRecord( FILE *fPtr )
{
	unsigned int account;
	double editSci;
	double editMath;
	double editTech;
	
	// create studentData with no information
	struct studentData student = {0, "", "", 0.0, 0.0, 0.0};
	
	printf("%s", "Enter account to update (100-999): ");
	scanf("%d", &account);
	
	// move file pointer to correct record in file 
	fseek( fPtr, ( account-1 ) * sizeof( struct studentData ), SEEK_SET);
	
	// read record from file
	fread( &student, sizeof( struct studentData ), 1, fPtr );
	
	// display error if account does not exist
	if ( student.stdntNum == 0 ) {
		printf("Account %d has no information. \n", account);
	} 
	else { // update record
		printf("%-10d%-20s%-20s%5.2f%8.2f%10.2f\n\n", student.stdntNum, student.lastName, student.firstName, student.science, student.math, student.technology);
		
		// request edit science grade from user
		printf("%s", "Enter new grade for Science:\t");
		scanf("%lf", &editSci);
		student.science = editSci; // update record for Science grade
		
		// request edit math grade from user
		printf("%s", "\nEnter new grade for Math:\t");
		scanf("%lf", &editMath);
		student.math = editMath; // update record for Math grade
		
		// request edit technology grade from user
		printf("%s", "\nEnter new grade for Technology:\t");
		scanf("%lf", &editTech);
		student.technology = editTech; // update record for Technology grade
		
		printf("%-10d%-20s%-20s%5.2f%8.2f%10.2f\n", student.stdntNum, student.lastName, student.firstName, student.science, student.math, student.technology);
		
		// move file pointer to correct record in file
		fseek( fPtr, ( account - 1 ) * sizeof( struct studentData ), SEEK_SET);
		
		// write updated record over old record in file 
		fwrite( &student, sizeof( struct studentData ), 1, fPtr );
	}
}

void deleteRecord ( FILE *fPtr )
{
	struct studentData student;
	struct studentData blankStudent = { 0, "", "", 0.0, 0.0, 0.0};
	
	unsigned int studentNum; // account number
	
	// get number of account to delete
	printf("%s", "Enter account number to delete (100 - 999): ");
	scanf("%d", &studentNum);
	
	// move file pointer to correct record in file
	fseek( fPtr, ( studentNum - 1 ) * sizeof( struct studentData ), SEEK_SET);
	
	// read record from file
	fread( &student, sizeof( struct studentData ), 1, fPtr);
	
	// display error if record does not exist
	if ( student.stdntNum == 0 ) {
		printf("Account #%d does not exist.\n", studentNum);
	}
	else { // delete record
		// move file pointer to correct record on file
		fseek( fPtr, ( studentNum -1 ) * sizeof( struct studentData), SEEK_SET);
		
		// read record from file
		fwrite( &blankStudent, sizeof( struct studentData ), 1, fPtr);
	}
	
} // end

void newRecord ( FILE *fPtr )
{
	// create clientData with default information
	struct studentData student = {0, "", "", 0.0, 0.0, 0.0};
	
	unsigned int studentNum;
	
	// get number of account to create
	printf("%s", "Enter new account number (100 - 999): ");
	scanf("%d", &studentNum);
	
	// move file pointer to record in file
	fseek( fPtr, (studentNum - 1) * sizeof(struct studentData), SEEK_SET);
	
	// read record from file
	fread( &student, sizeof( struct studentData ), 1, fPtr);
	
	//
	if (student.stdntNum != 0) {
		printf("Account #%d already contains information\n", student.stdntNum);
	}
	else { // create record
		// user enters last name, first name, sciGrade, mathGrade, techgrade
		printf("%s", "Enter lastname, firstname, science grade, math grade, tech grade\n? ");
		scanf("%19s%19s%lf%lf%lf", &student.lastName, &student.firstName, &student.science, &student.math, &student.technology);
		
	student.stdntNum = studentNum;
	
	// move file pointer to correct record in file
	fseek( fPtr, (student.stdntNum - 1) * sizeof(struct studentData), SEEK_SET);
	
	// insert record in file
	fwrite( &student, sizeof( struct studentData ), 1, fPtr);		
	}
} // end function

void displayRecord ( FILE *fPtr )
{
	
	char str;
	
	
	if ( ( fPtr = fopen( "students.txt", "rb+")) == NULL )
	{
		printf("Can not open");
	}
	else
	{
		printf("\n\n");
		str = fgetc(fPtr);
		while ( str != EOF) {
			printf("%c", str);
			str = fgetc(fPtr);
		}
		printf("\n\n");
	
	}
}

