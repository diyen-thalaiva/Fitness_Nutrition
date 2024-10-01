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
