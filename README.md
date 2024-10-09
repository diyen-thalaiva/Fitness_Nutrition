# ACTIV X

<img src="https://github.com/TAX-2905/Fitness_Nutrition/blob/patch-1/Logo.png?raw=true" alt="Logo" width="400" height="400">

## Project Overview

**ACTIV X** is a comprehensive fitness and nutrition management application, built to assist users in tracking their physical activities, monitoring calorie intake, and promoting overall well-being. This command-line based application, developed primarily in C, offers an interactive and dynamic user experience using the **ncurses** library. It provides users with a seamless way to manage their health by tracking daily workouts, meals, and personal data, all from within the terminal. 

With its intuitive menu-driven system, users can easily navigate through options, create and manage profiles, log exercises, and monitor their food intake. The application also integrates intelligent suggestions through an AI module that offers personalized recommendations based on the user’s activity and nutrition data. 

---

## Features

### Fitness Tracking

ACTIV X provides a robust fitness module that allows users to log and track their workouts. Users can enter details about their exercises, including the type of activity, duration, and calories burned. This module is designed to be highly flexible, enabling users to track various forms of physical activity, whether it's cardio, strength training, or any custom exercise. The data is stored in an accessible format, allowing users to review their progress over time.

### Nutrition Management

The nutrition module in ACTIV X helps users monitor their daily calorie intake by logging their meals. Users can input food items, calories, and portion sizes, with the application offering a simple interface for entering and managing this data. The stored data is organized in CSV files, making it easy to retrieve and analyze meal patterns. The combination of exercise tracking and nutrition management helps users maintain a balanced approach to health.

### AI-Driven Recommendations

One of the standout features of ACTIV X is its **AI module**, which leverages the user's personal data to provide intelligent, personalized recommendations for workouts and meals. By analyzing the user’s exercise and nutritional habits, the AI can suggest optimized meal plans or training regimens tailored to their specific fitness goals. This enhances the user experience by offering proactive health advice based on real data, taking the guesswork out of personal health management.

---

## Architecture

### Modular Design

ACTIV X has been developed with a modular architecture, making it easy to maintain and extend. Each component—fitness, nutrition, users, and AI—is handled by a dedicated module. The **fitness** and **nutrition** modules focus on logging and tracking data, while the **users** module manages profile creation and data storage. The **utils** module provides common functionality used across the application, ensuring that the core structure is clean and efficient.

This modular approach also makes it easier for future developers to add new features or modify existing ones without affecting other parts of the application. The architecture supports scalability, with the potential for future integration of advanced features like real-time data synchronization or external API usage.

### Platform Compatibility

To ensure cross-platform compatibility, ACTIV X provides build scripts for various environments:
- **PowerShell** script (`build.ps1`) for Windows.
- **Bash** script (`build.sh`) for Linux.
- **Batch** script (`build1.bat`) for additional Windows setups.

These scripts streamline the process of compiling and running the application, making it easy for users to install and use it on different platforms.

---

## User Interface and Experience

### Command-Line Interface (CLI)

ACTIV X features a well-designed command-line interface powered by ncurses, ensuring smooth user interaction. The terminal-based design is enhanced by colored interfaces, intuitive prompts, and blinking notifications to guide users through different sections of the application. Whether users are inputting new workout data or navigating through their meal logs, the UI ensures that the process is simple and visually appealing, despite being entirely text-based.

### Dynamic User Profiles

The user profile management system allows for the creation and storage of multiple user profiles. Each profile includes key information such as the user's name, age, fitness goals, and more. Users can switch between profiles or edit their details, making the system versatile for individuals or families who wish to use the same application on shared devices.

---

## Conclusion

ACTIV X is more than just a fitness tracker—it's a complete health management tool that combines exercise and nutrition tracking with intelligent recommendations. Its modular structure, dynamic user interface, and cross-platform compatibility make it a powerful tool for anyone looking to improve their health. Whether you're an individual aiming to track your daily progress or someone looking for personalized workout and meal suggestions, ACTIV X is the perfect solution to centralize all your fitness and nutrition data in one place.

