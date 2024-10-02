#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // For usleep function

#define BLINK_DELAY 500000  // 500 milliseconds (0.5 second delay for blinking)

// Function to display the current time
void display_time() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[9];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);

    // Print the time
    printf("TIME: %s\n", buffer);
}

// Function to display the ACTIV X logo
void display_logo() {
    const char *logo[] = {
        "        ***      ******   **********  **********  **             **",
        "      **   **   **    **      **          **       **           **",
        "     **     **  **            **          **        **         **",
        "    **       ** **            **          **         **       **",
        "    **       ** **            **          **          **     **",
        "    *********** **            **          **           **   **",
        "    **       ** **            **          **            ** **",
        "    **       **  **    **     **          **             ***",
        "    **       **   ******      **      **********          *",
        "",
        "",
        "                            **       **      ",
        "                             **     **       ",
        "                              **   **        ",
        "                               ** **         ",
        "                                ***          ",
        "                               ** **         ",
        "                              **   **        ",
        "                             **     **       ",
        "                            **       **      "
    };

    for (int i = 0; i < 20; ++i) {
        printf("%s\n", logo[i]);
    }

    // Print the motivational quote below the logo
    printf("\n Your success is found in your daily routine!\n\n");
}

// Function to display a blinking message
void blink_message() {
    int visible = 1;  // Start with the message visible
    time_t last_time_update = time(NULL);  // Store the last time update

    while (1) {
        // Get the current time
        time_t current_time = time(NULL);

        // Update the blinking message
        if (visible) {
            printf("\rPress ENTER to continue ");
            fflush(stdout);  // Force the output to display
        } else {
            printf("\r                       ");  // Clear the message
            fflush(stdout);
        }

        // Wait for the blink delay
        usleep(BLINK_DELAY);
        visible = !visible;   // Toggle the visibility

        // Check if 1 second has passed since the last time update
        if (difftime(current_time, last_time_update) >= 1.0) {
            printf("\n");
            display_time();  // Update the time display
            last_time_update = current_time;   // Reset the last time update
        }

        // Check for user input to stop blinking and continue
        if (getchar() == '\n') {
            break;  // Exit loop when Enter is pressed
        }
    }
}

int main() {
    // Display the ACTIV X logo
    display_logo();

    // Display the current time initially
    display_time();

    // Implement manual blinking for "Press ENTER to continue..."
    blink_message();

    return 0;
}
