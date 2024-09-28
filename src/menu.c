#include <stdio.h>

int main(void) {
  int option = -1;

  while (1) {
    printf("Select option:\n");
    printf("1 Ask LomeshAI\n");
    printf("2 Fitness & Nutrition Tracker\n");
    printf("3 Quit\n");
    printf("Enter input: ");
    scanf("%d", &option);

    switch (option) {
    case 1:
      while (1) {
        if (prompt_user_ai() == -1)
          break;
      }
      break;
    case 2:
      fitness_tracker();
      break;
    case 3:
      goto exit;
    default:
      printf("Invalid input. ");
      break;
    }

    option = -1;
  }

exit:

  return 0;
}
