#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for maximum number of movies, seats, and time slots
#define MAX_MOVIES 100
#define MAX_SEATS 50
#define MAX_TIMESLOTS 5

// Structure to hold movie information
typedef struct {
    int id;                                 // Movie ID
    char name[100];                         // Movie name
    float price;                            // Ticket price
    int seats[MAX_TIMESLOTS][MAX_SEATS];    // Seat availability for each time slot
    char timeslots[MAX_TIMESLOTS][10];      // Time slots
    int timeslotCount;                      // Number of available time slots
} Movie;

// Array to store movies
Movie movies[MAX_MOVIES];
// Counter for the number of movies
int movieCount = 0;

// Function to load movies from a file
void loadMovies() {
    FILE *file = fopen("movies.txt", "r");
    if (file) {
        fscanf(file, "%d\n", &movieCount); // Read the number of movies
        for (int i = 0; i < movieCount; i++) {
            fscanf(file, "%d\n", &movies[i].id); // Read movie ID
            fgets(movies[i].name, sizeof(movies[i].name), file); // Read movie name
            strtok(movies[i].name, "\n"); // Remove newline character from the name
            fscanf(file, "%f\n", &movies[i].price); // Read ticket price
            fscanf(file, "%d\n", &movies[i].timeslotCount); // Read number of time slots
            for (int t = 0; t < movies[i].timeslotCount; t++) {
                fgets(movies[i].timeslots[t], sizeof(movies[i].timeslots[t]), file); // Read time slot
                strtok(movies[i].timeslots[t], "\n"); // Remove newline character
                for (int j = 0; j < MAX_SEATS; j++) {
                    fscanf(file, "%d", &movies[i].seats[t][j]); // Read seat availability
                }
                fscanf(file, "\n");
            }
        }
        fclose(file);
    }
}

// Function to save movies to a file
void saveMovies() {
    FILE *file = fopen("movies.txt", "w");
    if (file) {
        fprintf(file, "%d\n", movieCount); // Write the number of movies
        for (int i = 0; i < movieCount; i++) {
            fprintf(file, "%d\n", movies[i].id); // Write movie ID
            fprintf(file, "%s\n", movies[i].name); // Write movie name
            fprintf(file, "%.2f\n", movies[i].price); // Write ticket price
            fprintf(file, "%d\n", movies[i].timeslotCount); // Write number of time slots
            for (int t = 0; t < movies[i].timeslotCount; t++) {
                fprintf(file, "%s\n", movies[i].timeslots[t]); // Write time slot
                for (int j = 0; j < MAX_SEATS; j++) {
                    fprintf(file, "%d", movies[i].seats[t][j]); // Write seat availability
                    if (j < MAX_SEATS - 1) {
                        fprintf(file, " ");
                    }
                }
                fprintf(file, "\n");
            }
        }
        fclose(file);
    }
}

// Function to add a new movie
void addMovie() {
    if (movieCount >= MAX_MOVIES) {
        printf("Cannot add more movies.\n");
        return;
    }

    Movie movie;
    movie.id = movieCount + 1; // Assign a new ID to the movie
    printf("Enter movie name: ");
    scanf(" %[^\n]", movie.name); // Read movie name
    printf("Enter ticket price: ");
    scanf("%f", &movie.price); // Read ticket price
    printf("Enter number of time slots: ");
    scanf("%d", &movie.timeslotCount); // Read number of time slots
    for (int i = 0; i < movie.timeslotCount; i++) {
        printf("Enter time slot %d (e.g., 10:00AM): ", i + 1);
        scanf(" %[^\n]", movie.timeslots[i]); // Read time slot
        for (int j = 0; j < MAX_SEATS; j++) {
            movie.seats[i][j] = 0; // Initialize all seats to available (0)
        }
    }

    movies[movieCount++] = movie; // Add the movie to the array and increment movieCount
    saveMovies(); // Save the updated movie list to the file
    printf("Movie added successfully.\n");
}

// Function to remove a movie
void removeMovie() {
    int id;
    printf("Enter movie ID to remove: ");
    scanf("%d", &id); // Read the ID of the movie to be removed

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].id == id) {
            for (int j = i; j < movieCount - 1; j++) {
                movies[j] = movies[j + 1]; // Shift the movies to remove the movie with the given ID
            }
            movieCount--; // Decrement movieCount
            saveMovies(); // Save the updated movie list to the file
            printf("Movie removed successfully.\n");
            return;
        }
    }

    printf("Movie not found.\n");
}

// Function to book a ticket
void bookTicket() {
    int id, seat, timeslot;
    printf("Enter movie ID to book: ");
    scanf("%d", &id); // Read the ID of the movie

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].id == id) {
            printf("Available time slots:\n");
            for (int t = 0; t < movies[i].timeslotCount; t++) {
                printf("%d. %s\n", t + 1, movies[i].timeslots[t]); // Display available time slots
            }
            printf("Choose a time slot: ");
            scanf("%d", &timeslot); // Read the selected time slot
            if (timeslot < 1 || timeslot > movies[i].timeslotCount) {
                printf("Invalid time slot.\n");
                return;
            }
            printf("Enter seat number (1-50): ");
            scanf("%d", &seat); // Read the seat number
            if (seat < 1 || seat > MAX_SEATS || movies[i].seats[timeslot-1][seat - 1] == 1) {
                printf("Invalid seat number or seat already booked.\n");
            } else {
                movies[i].seats[timeslot-1][seat - 1] = 1; // Mark the seat as booked
                saveMovies(); // Save the updated movie list to the file
                printf("Ticket booked successfully.\n");
            }
            return;
        }
    }

    printf("Movie not found.\n");
}

// Function to cancel a ticket
void cancelTicket() {
    int id, seat, timeslot;
    printf("Enter movie ID to cancel: ");
    scanf("%d", &id); // Read the ID of the movie

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].id == id) {
            printf("Available time slots:\n");
            for (int t = 0; t < movies[i].timeslotCount; t++) {
                printf("%d. %s\n", t + 1, movies[i].timeslots[t]); // Display available time slots
            }
            printf("Choose a time slot: ");
            scanf("%d", &timeslot); // Read the selected time slot
            if (timeslot < 1 || timeslot > movies[i].timeslotCount) {
                printf("Invalid time slot.\n");
                return;
            }
            printf("Enter seat number (1-50): ");
            scanf("%d", &seat); // Read the seat number
            if (seat < 1 || seat > MAX_SEATS || movies[i].seats[timeslot - 1][seat - 1] == 0) {
                printf("Invalid seat number or seat not booked.\n");
            } else {
                movies[i].seats[timeslot - 1][seat - 1] = 0; // Mark the seat as available
                saveMovies(); // Save the updated movie list to the file
                printf("Ticket canceled successfully.\n");
            }
            return;
        }
    }

    printf("Movie not found.\n");
}

// Function to show the seats for a specific movie and time slot
void showSeats() {
    int id, timeslot;
    printf("Enter movie ID to view seats: ");
    scanf("%d", &id); // Read the ID of the movie

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].id == id) {
            printf("Available time slots:\n");
            for (int t = 0; t < movies[i].timeslotCount; t++) {
                printf("%d. %s\n", t + 1, movies[i].timeslots[t]); // Display available time slots
            }
            printf("Choose a time slot: ");
            scanf("%d", &timeslot); // Read the selected time slot
            if (timeslot < 1 || timeslot > movies[i].timeslotCount) {
                printf("Invalid time slot.\n");
                return;
            }
            printf("Seats for movie %s at %s:\n", movies[i].name, movies[i].timeslots[timeslot - 1]);
            for (int j = 0; j < MAX_SEATS; j++) {
                printf("Seat %d: %s\n", j + 1, movies[i].seats[timeslot - 1][j] ? "Booked" : "Available"); // Display seat status
            }
            return;
        }
    }

    printf("Movie not found.\n");
}

// Function to edit the price of a movie ticket
void editPrice() {
    int id;
    float price;
    printf("Enter movie ID to edit price: ");
    scanf("%d", &id); // Read the ID of the movie

    for (int i = 0; i < movieCount; i++) {
        if (movies[i].id == id) {
            printf("Enter new price: ");
            scanf("%f", &price); // Read the new price
            movies[i].price = price; // Update the price
            saveMovies(); // Save the updated movie list to the file
            printf("Price updated successfully.\n");
            return;
        }
    }

    printf("Movie not found.\n");
}

// Function to display all movies
void showMovies() {
    if (movieCount == 0) {
        printf("No movies available.\n");
        return;
    }

    printf("\nAvailable Movies:\n");
    printf("%-5s %-30s %-10s\n", "ID", "Name", "Price");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < movieCount; i++) {
        printf("%-5d %-30s $%-9.2f\n", movies[i].id, movies[i].name, movies[i].price); // Display movie details
        printf("Time Slots:\n");
        for (int t = 0; t < movies[i].timeslotCount; t++) {
            printf("  %d. %s\n", t + 1, movies[i].timeslots[t]); // Display available time slots
        }
    }
}

// Function to display the menu options
void menu() {
    printf("\nMovie Ticket Management System\n");
    printf("1. Add Movie\n");
    printf("2. Remove Movie\n");
    printf("3. Book Ticket\n");
    printf("4. Cancel Ticket\n");
    printf("5. Show Seats\n");
    printf("6. Edit Movie Price\n");
    printf("7. Show All Movies\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

// Main function
int main() {
    loadMovies(); // Load movies from the file

    int choice;
    do {
        menu(); // Display menu
        scanf("%d", &choice); // Read user's choice

        switch (choice) {
            case 1:
                addMovie(); // Add a new movie
                break;
            case 2:
                removeMovie(); // Remove an existing movie
                break;
            case 3:
                bookTicket(); // Book a ticket
                break;
            case 4:
                cancelTicket(); // Cancel a ticket
                break;
            case 5:
                showSeats(); // Show seats for a specific movie and time slot
                break;
            case 6:
                editPrice(); // Edit the price of a movie ticket
                break;
            case 7:
                showMovies(); // Show all movies
                break;
            case 8:
                saveMovies(); // Save movies to the file before exiting
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}

