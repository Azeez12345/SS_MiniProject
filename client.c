#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8005
#define MAX_CHOICE 1024
#define MAX_USERNAME_SIZE 1024 // Maximum username size
#define MAX_PASSWORD_SIZE 1024 // Increase the buffer size


int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Input username and password
    char buffer1[1024];
    recv(clientSocket, buffer1, sizeof(buffer1), 0);
    printf("%s\n",buffer1);
    char choice[MAX_CHOICE];
    char username[MAX_USERNAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
    printf("Enter Choice: ");
    fgets(choice, sizeof(choice), stdin);
    choice[strlen(choice) -1] = '\0';
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strlen(username) - 1] = '\0'; // Remove the newline character

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strlen(password) - 1] = '\0'; // Remove the newline character

    // Send username and password to the server
    char buffer[MAX_CHOICE+MAX_USERNAME_SIZE+MAX_PASSWORD_SIZE+3];
    snprintf(buffer, sizeof(buffer), "%s,%s,%s", choice,username, password);
    send(clientSocket, buffer, strlen(buffer), 0);

    // Receive authentication result from the server
    if(atoi(choice) == 1)
    {
    	memset(buffer, 0, sizeof(buffer));
    	recv(clientSocket, buffer, sizeof(buffer), 0);

    // Display the authentication result
    	printf("%s\n",buffer);
    	char adminchoice[MAX_CHOICE];
    	char studentname[MAX_USERNAME_SIZE];
    	char studentrollno[MAX_PASSWORD_SIZE];
    	printf("Enter your choice as admin: ");
    	fgets(adminchoice, sizeof(adminchoice), stdin);
    	adminchoice[strlen(adminchoice) -1] = '\0';
    	if(atoi(adminchoice) == 1)
    	{
    	printf("Enter name of the student: ");
    	fgets(studentname, sizeof(studentname), stdin);
    	studentname[strlen(studentname) -1] = '\0';
    	printf("Enter roll number of the student: ");
    	fgets(studentrollno, sizeof(studentrollno), stdin);
    	studentrollno[strlen(studentrollno) -1] = '\0';
    	char buff[MAX_CHOICE+MAX_USERNAME_SIZE+MAX_PASSWORD_SIZE+3];
    	snprintf(buff, sizeof(buff), "%s,%s,%s",adminchoice,studentname,studentrollno);
    	send(clientSocket, buff, strlen(buff), 0);
    	memset(buff, 0, sizeof(buff));
    	recv(clientSocket, buff, strlen(buff), 0);
    	printf("%s\n", buff);
    	}
    	else if(atoi(adminchoice) == 2)
    	{
    	char buf[MAX_CHOICE+1];
    	snprintf(buf, sizeof(buf), "%s,",adminchoice);
    	send(clientSocket, buf, strlen(buf), 0);
    	printf("Student Details:\n");
    	memset(buf, 0, sizeof(buf));
        while (recv(clientSocket, buf, sizeof(buf), 0) > 0) {
                printf("%s\n", buf);
            }
        }
        else if(atoi(adminchoice) == 3)
        {
        char facultyname[MAX_USERNAME_SIZE];
    	char facultyrollno[MAX_PASSWORD_SIZE];
        printf("Enter name of the faculty: ");
    	fgets(facultyname, sizeof(facultyname), stdin);
    	facultyname[strlen(facultyname) -1] = '\0';
    	printf("Enter id of the faculty: ");
    	fgets(facultyrollno, sizeof(facultyrollno), stdin);
    	facultyrollno[strlen(facultyrollno) -1] = '\0';
    	char buff[MAX_CHOICE+MAX_USERNAME_SIZE+MAX_PASSWORD_SIZE+3];
    	snprintf(buff, sizeof(buff), "%s,%s,%s",adminchoice,facultyname,facultyrollno);
    	send(clientSocket, buff, strlen(buff), 0);
    	memset(buff, 0, sizeof(buff));
    	recv(clientSocket, buff, strlen(buff), 0);
    	printf("%s\n", buff);
        }
        else if(atoi(adminchoice) == 4)
        {
        char buf[MAX_CHOICE+1];
    	snprintf(buf, sizeof(buf), "%s,",adminchoice);
    	send(clientSocket, buf, strlen(buf), 0);
    	printf("Faculty Details:\n");
    	memset(buf, 0, sizeof(buf));
        while (recv(clientSocket, buf, sizeof(buf), 0) > 0) {
                printf("%s\n", buf);
            }
        }
        else if(atoi(adminchoice) == 7)
        {
            char rollno[MAX_PASSWORD_SIZE];
            char new_name[MAX_USERNAME_SIZE];
	    printf("Enter the roll no of student: ");
	    fgets(rollno, sizeof(rollno),stdin);
	    rollno[strlen(rollno) -1] = '\0';
	    printf("Set new Name: ");
	    fgets(new_name,sizeof(new_name),stdin);
	    new_name[strlen(new_name) -1] = '\0';
            char buff1[MAX_CHOICE+MAX_USERNAME_SIZE+MAX_PASSWORD_SIZE+3];
            snprintf(buff1, sizeof(buff1), "%s,%s,%s",adminchoice,rollno,new_name);
    	    send(clientSocket, buff1, strlen(buff1), 0);

            // Send modification request to the server
 	    

            // Receive and display the confirmation message
            memset(buff1,0,sizeof(buff1));
            recv(clientSocket, buff1, sizeof(buff1), 0);
            printf("%s\n", buff1);
        }
        else if(atoi(adminchoice) == 8)
        {
            char rollno[MAX_PASSWORD_SIZE];
            char new_name[MAX_USERNAME_SIZE];
	    printf("Enter the id of faculty: ");
	    fgets(rollno, sizeof(rollno),stdin);
	    rollno[strlen(rollno) -1] = '\0';
	    printf("Set new Name: ");
	    fgets(new_name,sizeof(new_name),stdin);
	    new_name[strlen(new_name) -1] = '\0';
            char buff1[MAX_CHOICE+MAX_USERNAME_SIZE+MAX_PASSWORD_SIZE+3];
            snprintf(buff1, sizeof(buff1), "%s,%s,%s",adminchoice,rollno,new_name);
    	    send(clientSocket, buff1, strlen(buff1), 0);

            // Send modification request to the server
 	    

            // Receive and display the confirmation message
            memset(buff1,0,sizeof(buff1));
            recv(clientSocket, buff1, sizeof(buff1), 0);
            printf("%s\n", buff1);
        }
    }
    else if(atoi(choice)==2)
    {
    	memset(buffer, 0, sizeof(buffer));
    	recv(clientSocket, buffer, sizeof(buffer), 0);
    	printf("%s\n",buffer);
    	char facultychoice[MAX_CHOICE];
    	printf("Enter your choice as faculty: ");
    	fgets(facultychoice, sizeof(facultychoice), stdin);
    	facultychoice[strlen(facultychoice) -1] = '\0';
    	if(atoi(facultychoice) == 1)
    	{
    	char buf10[MAX_CHOICE+1];
    	snprintf(buf10, sizeof(buf10), "%s,",facultychoice);
    	send(clientSocket, buf10, strlen(buf10), 0);
    	printf("Course Details:\n");
    	memset(buf10,0,sizeof(buf10));
    	recv(clientSocket, buf10, sizeof(buf10), 0);
    	printf("%s\n", buf10);
    	}
    	else if(atoi(facultychoice) == 2)
    	{
    	    char newcourse[MAX_PASSWORD_SIZE];
	    printf("Enter the course name: ");
	    fgets(newcourse, sizeof(newcourse),stdin);
	    newcourse[strlen(newcourse) -1] = '\0';
            char buff1[MAX_CHOICE+MAX_PASSWORD_SIZE+2];
            snprintf(buff1, sizeof(buff1), "%s,%s",facultychoice,newcourse);
    	    send(clientSocket, buff1, strlen(buff1), 0);

            // Send modification request to the server
 	    

            // Receive and display the confirmation message
            memset(buff1,0,sizeof(buff1));
            recv(clientSocket, buff1, sizeof(buff1), 0);
            printf("%s\n", buff1);
    	}
    	else if(atoi(facultychoice) == 3)
    	{
    	    char deletecourse[MAX_PASSWORD_SIZE];
	    printf("Enter the course name you want to delete: ");
	    fgets(deletecourse, sizeof(deletecourse),stdin);
	    deletecourse[strlen(deletecourse) -1] = '\0';
            char buff1[MAX_CHOICE+MAX_PASSWORD_SIZE+2];
            snprintf(buff1, sizeof(buff1), "%s,%s",facultychoice,deletecourse);
    	    send(clientSocket, buff1, strlen(buff1), 0);

            // Send modification request to the server
 	    

            // Receive and display the confirmation message
            memset(buff1,0,sizeof(buff1));
            recv(clientSocket, buff1, sizeof(buff1), 0);
            printf("%s\n", buff1);
    	}
    }

    close(clientSocket);

    return 0;
}

