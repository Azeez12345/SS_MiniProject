#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8005
#define MAX_BUFFER_SIZE 1024
#define SECRET_TOKEN "mysecrettoken" // Change this to your desired secret token
#define USER_DATA_FILE "user_data.txt"
#define FACULTY_DATA_FILE "faculty_data.txt"
#define STUDENT_DATA_FILE "student_data.txt"
#define FACULTY_COURSES_FILE "facultycourses.txt"

// Function to check if the given username and password match the ones in the data file

void removefacultycourse(char *username, char *courseToRemove)
{
    char filename[] = "facultycourses.txt";
    char line[1000];

    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("tempfile.txt", "w");

    if (fp == NULL || temp == NULL) {
        perror("Error");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp)) {
    	
        char *facultyStart = strstr(line, username);
        if (facultyStart != NULL) {
            // Check for "SS" within the "vishwa" line and remove it
            char *courseStart = strstr(facultyStart, courseToRemove);
            if (courseStart != NULL) {
                // Copy everything before "SS"
                fwrite(line, 1, courseStart - line, temp);
                // Copy everything after "SS"
                fwrite(courseStart + strlen(courseToRemove), 1, strlen(line) - (courseStart - line) - strlen(courseToRemove), temp);
            } else {
                // If "SS" is not found, copy the entire line
                fputs(line, temp);
            }
        } else {
            // If not the "vishwa" line, copy the entire line
            fputs(line, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    // Replace the original file with the temporary file
    if (remove(filename) != 0) {
        perror("Error");
        exit(1);
    }
    if (rename("tempfile.txt", filename) != 0) {
        perror("Error");
        exit(1);
    }

    printf("Course removed successfully.\n");
}

void addfacultycourse(char *username, char *newcourse)
{
	
	char tmpfilename[] = "tempfile.txt";
	FILE *fp = fopen(FACULTY_COURSES_FILE, "r+");
	if(fp == NULL)
	{
		perror("Error opening faculty course file");
		return;
	}
	char line[MAX_BUFFER_SIZE];
	int added = 0;
	while (fgets(line, sizeof(line), fp)) {
		char storedName[MAX_BUFFER_SIZE];
		char storedCourses[MAX_BUFFER_SIZE];
		sscanf(line, "%s %s", storedName,storedCourses);
        if (strcmp(username, storedName) == 0) {
            // Found the faculty, append the new course
            size_t len = strlen(line);
            line[len - 1] = '-';  // Remove the newline character
            strcat(line, newcourse);
            strcat(line, "\n");  // Add back the newline character
            fseek(fp, -len, SEEK_CUR);  // Move the file pointer back to the start of the line
            fputs(line, fp);
        }
    	}

    	fclose(fp);

    // Replace the original file with the temporary file
    	printf("Course added successfully.\n");
	
}
void facultycourse(int client_socket,char *username)
{
	FILE *file = fopen(FACULTY_COURSES_FILE, "r");
	if(file == NULL)
	{
		perror("Error opening faculty course file");
		return ;
	}
	char line[MAX_BUFFER_SIZE];
	int found = 0;
	while(fgets(line, sizeof(line), file))
	{
		char storedName[MAX_BUFFER_SIZE];
		char storedCourses[MAX_BUFFER_SIZE];
		sscanf(line, "%s %s", storedName,storedCourses);
		if(strcmp(username, storedName) == 0)
		{
			send(client_socket, storedCourses, sizeof(storedCourses), 0);
			found = 1;
			break;
		}
	}
	if(found == 1)
	{
		printf("Name found\n");
	}
	else
	{
		printf("Name didn't match\n");
	}
	fclose(file);
	
}
int authenticate(char *username, char *password) {
    FILE *file = fopen(USER_DATA_FILE, "r");
    if (file == NULL) {
        perror("Error opening user data file");
        return 0; // Authentication fails
    }

    char line[MAX_BUFFER_SIZE];
    int authenticated = 0;

    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_BUFFER_SIZE];
        char storedPassword[MAX_BUFFER_SIZE];
        sscanf(line, "%s %s", storedUsername, storedPassword);

        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            authenticated = 1; // Authentication succeeds
            break;
        }
    }

    fclose(file);

    return authenticated;
}
int authenticate1(char *username, char *password) {
    FILE *file = fopen(FACULTY_DATA_FILE, "r");
    if (file == NULL) {
        perror("Error opening user data file");
        return 0; // Authentication fails
    }

    char line[MAX_BUFFER_SIZE];
    int authenticated = 0;

    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_BUFFER_SIZE];
        char storedPassword[MAX_BUFFER_SIZE];
        sscanf(line, "%s %s", storedUsername, storedPassword);

        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            authenticated = 1; // Authentication succeeds
            break;
        }
    }

    fclose(file);

    return authenticated;
}
int authenticate2(char *username, char *password) {
    FILE *file = fopen(STUDENT_DATA_FILE, "r");
    if (file == NULL) {
        perror("Error opening user data file");
        return 0; // Authentication fails
    }

    char line[MAX_BUFFER_SIZE];
    int authenticated = 0;

    while (fgets(line, sizeof(line), file)) {
        char storedUsername[MAX_BUFFER_SIZE];
        char storedPassword[MAX_BUFFER_SIZE];
        sscanf(line, "%s %s", storedUsername, storedPassword);

        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            authenticated = 1; // Authentication succeeds
            break;
        }
    }

    fclose(file);

    return authenticated;
}

void addStudentToFile(const char* fileName, const char* name, int roll_no) {
    FILE* file = fopen(fileName, "a"); // Open the file in append mode
    if (file == NULL) {
        perror("File open failed");
        return;
    }
    fprintf(file, "Name: %s, Roll No: %d\n", name, roll_no);
    fclose(file);
}

void addFacultyToFile(const char* fileName, const char* name, int id)
{
    FILE* file = fopen(fileName, "a"); // Open the file in append mode
    if (file == NULL) {
        perror("File open failed");
        return;
    }
    fprintf(file, "Name: %s, Faculty_id: %d\n", name, id);
    fclose(file);
}

void sendFileContents(int client_socket, const char* fileName) {
    FILE* file = fopen(fileName, "r"); // Open the file in read mode
    if (file == NULL) {
        perror("File open failed");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        send(client_socket, buffer, strlen(buffer), 0);
    }
	
    fclose(file);
}

// Function to modify student details in the file
void modifyStudentDetails(const char* fileName, char *roll_no, const char* new_name) {
    FILE* file = fopen(fileName, "r"); // Open the file in read mode
    if (file == NULL) {
        perror("File open failed");
        return;
    }

    // Create a temporary file to store modified data
    FILE* temp_file = fopen("temp_student.txt", "w");
    if (temp_file == NULL) {
        perror("File creation failed");
        fclose(file);
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    int modified = 0; // Flag to indicate if a modification occurred

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char current_roll_no[1024];
        char current_name[MAX_BUFFER_SIZE];
        if (sscanf(buffer, "Name: %[^,], Roll No: %s", current_name, current_roll_no) == 2) {
            
            if (strcmp(current_roll_no, roll_no) == 0) {
                // Replace the name with the new name
                fprintf(temp_file, "Name: %s, Roll No: %s\n", new_name, roll_no);
                modified = 1;
            } else {
                fprintf(temp_file, "Name: %s, Roll No: %s\n", current_name, current_roll_no);
            }
        } else {
            fprintf(temp_file, "%s", buffer);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the modified file
    remove(fileName);
    rename("temp_student.txt", fileName);
    if (modified) {
        printf("Student details modified successfully.\n");
    } else {
        printf("Student not found for modification.\n");
    }
}

void modifyFacultyDetails(const char* fileName, char *roll_no, const char* new_name) {
    FILE* file = fopen(fileName, "r"); // Open the file in read mode
    if (file == NULL) {
        perror("File open failed");
        return;
    }

    // Create a temporary file to store modified data
    FILE* temp_file = fopen("temp_faculty.txt", "w");
    if (temp_file == NULL) {
        perror("File creation failed");
        fclose(file);
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    int modified = 0; // Flag to indicate if a modification occurred

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char current_roll_no[1024];
        char current_name[MAX_BUFFER_SIZE];
        if (sscanf(buffer, "Name: %[^,], Faculty_id: %s", current_name, current_roll_no) == 2) {
            
            if (strcmp(current_roll_no, roll_no) == 0) {
                // Replace the name with the new name
                fprintf(temp_file, "Name: %s, Faculty_id: %s\n", new_name, roll_no);
                modified = 1;
            } else {
                fprintf(temp_file, "Name: %s, Faculty_id: %s\n", current_name, current_roll_no);
            }
        } else {
            fprintf(temp_file, "%s", buffer);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the modified file
    remove(fileName);
    rename("temp_faculty.txt", fileName);
    if (modified) {
        printf("Faculty details modified successfully.\n");
    } else {
        printf("Faculty not found for modification.\n");
    }
}

void sendFileContents0(int client_socket, const char* fileName) {
    FILE* file = fopen(fileName, "r"); // Open the file in read mode
    if (file == NULL) {
        perror("File open failed");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        send(client_socket, buffer, strlen(buffer), 0);
    }

    fclose(file);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);
    char studentFileName[] = "student.txt";
    char facultyFileName[] = "faculty.txt";
    char facultyCourses[] = "facultycourses.txt";
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(serverSocket, 5) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    char mm[] = ".............Welcome to Academia course registration...........\nLogin Type{1.Admin 2.Professor 3.Student";
    char message[] = "...........Welcome to Admn Menu..........\n1.Add Student\n2.View Student Details\n3.Add Faculty\n4.View Faculty Details\n5.Activate Student\n6.Block Student\n7.Modeify Student Details\n8.Modify Faculty Details\n9.Logout and Exit";
    char message1[] = "...........Welcome to Faculty Menu..........\n1.View Offering Courses\n2.Add New Courses\n3.Remove Courses From Catalog\n4.Update Course Details\n5.Change Password\n6.Logout and Exit";
    char message2[] = "...........Welcome to Student Menu..........\n1.View all courses\n2.Enroll course\n3.Drop Course\n4.View Enrolled Course Details\n5.Change Password\n6.Logout and Exit";
    while (1) {
        // Accept client connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
        if (clientSocket < 0) {
            perror("Accepting connection failed");
            exit(EXIT_FAILURE);
        }
	send(clientSocket, mm, strlen(mm), 0);
        char buffer[MAX_BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        // Receive username and password from the client
        recv(clientSocket, buffer, sizeof(buffer), 0);
	char delimiter[] = ",";
	char *choice, *username, *password;
	choice = strtok(buffer, delimiter);

    // Check if choice is not NULL (there is at least one token)
    	if (choice != NULL) {
        // The second call to strtok should use NULL instead of buffer
        	username = strtok(NULL, delimiter);

        // Check if username is not NULL (there is a second token)
        	if (username != NULL) {
            // The third call to strtok should use NULL instead of buffer
            		password = strtok(NULL, delimiter);
        	} else {
            // Handle the case where there is no username
            		printf("Error: Missing username.\n");
            		return 1; // Exit with an error code
        	}
    	} else {
        // Handle the case where there is no choice
        	printf("Error: Missing choice.\n");
        	return 1; // Exit with an error code
   	 }	

        // Authenticate using the data file
	switch(atoi(choice)){

	    case 1:
		if (authenticate(username, password)) {
            		// Authentication succeeded; send the token
            		send(clientSocket, message, strlen(message), 0);
            		printf("Authentication succeeded for %s\n", username);
            		char buffer1[MAX_BUFFER_SIZE];
            		recv(clientSocket, buffer1, sizeof(buffer1), 0);
			char delimiter1[] = ",";
			char *adminchoice;
			adminchoice = strtok(buffer1, delimiter1);
			if(atoi(adminchoice) == 1)
			{
			char *studentname, *studentrollno;
    		// Check if choice is not NULL (there is at least one token)
    			if (adminchoice != NULL) {
        	// The second call to strtok should use NULL instead of buffer
        			studentname = strtok(NULL, delimiter1);

        		// Check if username is not NULL (there is a second token)
        			if (studentname != NULL) {
            	// The third call to strtok should use NULL instead of buffer
            				studentrollno = strtok(NULL, delimiter1);
        			} else {
            	// Handle the case where there is no username
            				printf("Error: Missing studentname.\n");
            				return 1; // Exit with an error code
        			}
    			} else {
        	// Handle the case where there is no choice
        			printf("Error: Missing choice.\n");
        			return 1; // Exit with an error code
   	 		}
   	 		addStudentToFile(studentFileName, studentname, atoi(studentrollno));

                // Send a confirmation message to the client
                	char confirmationMsg[] = "Student added successfully.";
                	send(clientSocket, confirmationMsg, sizeof(confirmationMsg), 0);
   	 		
   	 		}
   	 		else if(atoi(adminchoice) == 2)
   	 		{
   	 		sendFileContents(clientSocket, studentFileName);
   	 		}
   	 		else if(atoi(adminchoice) == 3)
   	 		{
   	 		char *facultyname, *facultyid;
    		// Check if choice is not NULL (there is at least one token)
    			if (adminchoice != NULL) {
        	// The second call to strtok should use NULL instead of buffer
        			facultyname = strtok(NULL, delimiter1);

        		// Check if username is not NULL (there is a second token)
        			if (facultyname != NULL) {
            	// The third call to strtok should use NULL instead of buffer
            				facultyid = strtok(NULL, delimiter1);
        			} else {
            	// Handle the case where there is no username
            				printf("Error: Missing studentname.\n");
            				return 1; // Exit with an error code
        			}
    			} else {
        	// Handle the case where there is no choice
        			printf("Error: Missing choice.\n");
        			return 1; // Exit with an error code
   	 		}
   	 		addFacultyToFile(facultyFileName, facultyname, atoi(facultyid));

                // Send a confirmation message to the client
                	char confirmationMsg[] = "Student added successfully.";
                	send(clientSocket, confirmationMsg, sizeof(confirmationMsg), 0);
   	 		}
   	 		else if(atoi(adminchoice) == 4)
   	 		{
   	 		sendFileContents(clientSocket, facultyFileName);
   	 		}
   	 		else if(atoi(adminchoice) == 7)
   	 		{
   	 		char *rollno, *newname;
    		// Check if choice is not NULL (there is at least one token)
    			if (adminchoice != NULL) {
        	// The second call to strtok should use NULL instead of buffer
        			rollno = strtok(NULL, ",");

        		// Check if username is not NULL (there is a second token)
        			if (rollno != NULL) {
            	// The third call to strtok should use NULL instead of buffer
            				newname = strtok(NULL, ",");
        			} else {
            	// Handle the case where there is no username
            				printf("Error: Missing studentname.\n");
            				return 1; // Exit with an error code
        			}
    			} else {
        	// Handle the case where there is no choice
        			printf("Error: Missing choice.\n");
        			return 1; // Exit with an error code
   	 		}


                // Modify the student details in the file
                	modifyStudentDetails(studentFileName,rollno, newname);

                // Send a confirmation message to the client
                	char confirmationMsg[] = "Student details modified.";
                	send(clientSocket, confirmationMsg, sizeof(confirmationMsg), 0);
   	 		}
   	 		else if(atoi(adminchoice)==8)
   	 		{
   	 		char *id, *newname;
    		// Check if choice is not NULL (there is at least one token)
    			if (adminchoice != NULL) {
        	// The second call to strtok should use NULL instead of buffer
        			id = strtok(NULL, ",");

        		// Check if username is not NULL (there is a second token)
        			if (id != NULL) {
            	// The third call to strtok should use NULL instead of buffer
            				newname = strtok(NULL, ",");
        			} else {
            	// Handle the case where there is no username
            				printf("Error: Missing studentname.\n");
            				return 1; // Exit with an error code
        			}
    			} else {
        	// Handle the case where there is no choice
        			printf("Error: Missing choice.\n");
        			return 1; // Exit with an error code
   	 		}


                // Modify the student details in the file
                	modifyFacultyDetails(facultyFileName,id, newname);

                // Send a confirmation message to the client
                	char confirmationMsg[] = "Student details modified.";
                	send(clientSocket, confirmationMsg, sizeof(confirmationMsg), 0);
   	 		}
        	} else if(!authenticate(username, password)){
            	// Authentication failed
            		send(clientSocket, "Authentication failed", strlen("Authentication failed"), 0);
            		printf("Authentication failed for %s\n", username);
        	}
        	break;

	    case 2:
		if (authenticate1(username, password)) {
                        // Authentication succeeded; send the token
                        char courses[1024];
                        FILE* faculty_file = fopen("facultyCourses.txt", "r");
                        send(clientSocket, message1, strlen(message1), 0);
                        printf("Authentication succeeded for %s\n", username);
                        char buffer1[MAX_BUFFER_SIZE];
            		recv(clientSocket, buffer1, sizeof(buffer1), 0);
			char delimiter1[] = ",";
			char *facultychoice;
			facultychoice = strtok(buffer1, delimiter1);
			if (atoi(facultychoice) == 1) {
    				facultycourse(clientSocket,username);
			}
			else if(atoi(facultychoice) == 2)
			{
				char *newcourse;
    		// Check if choice is not NULL (there is at least one token)
    				if (facultychoice != NULL) {
        	// The second call to strtok should use NULL instead of buffer
        				newcourse = strtok(NULL, ",");
    				} else {
        	// Handle the case where there is no choice
        				printf("Error: Missing choice.\n");
        				return 1; // Exit with an error code
   	 			}
   	 			addfacultycourse(username, newcourse);
   	 			char confirmationMsg[] = "Course has added.";
                		send(clientSocket, confirmationMsg, sizeof(confirmationMsg), 0);
			}
			else if(atoi(facultychoice) == 3)
			{
				char *deletecourse;
    		// Check if choice is not NULL (there is at least one token)
    				if (facultychoice != NULL) {
        	// The second call to strtok should use NULL instead of buffer
        				deletecourse = strtok(NULL, ",");
    				} else {
        	// Handle the case where there is no choice
        				printf("Error: Missing choice.\n");
        				return 1; // Exit with an error code
   	 			}
   	 			removefacultycourse(username, deletecourse);
   	 			char confirmationMsg[] = "Course has been deleted.";
                		send(clientSocket, confirmationMsg, sizeof(confirmationMsg), 0);
			}
			
                } else {
                // Authentication failed
                        send(clientSocket, "Authentication failed", strlen("Authentication failed"), 0);
                        printf("Authentication failed for %s\n", username);
                }
                break;
	    case 3:
		if (authenticate2(username, password)) {
                        // Authentication succeeded; send the token
                        send(clientSocket, message2, strlen(message2), 0);
                        printf("Authentication succeeded for %s\n", username);
                        
                } else {
                // Authentication failed
                        send(clientSocket, "Authentication failed", strlen("Authentication failed"), 0);
                        printf("Authentication failed for %s\n", username);
                }
                break;
	    default:
		send(clientSocket, "Invalid Input", strlen("Invalid input"), 0);
		break;
	}



        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}

