#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>

class CalendarOrganizer {
private:
    std::vector<std::string> activityList;
    const int secondsPerDay = 24 * 60 * 60; // Seconds in a day

public:
    std::string getCurrentDate() {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d");
        return ss.str();
    }

    void addActivityWithDate(const std::string& activity, const std::string& date) {
        std::string fullActivity = activity + " on " + date;
        activityList.push_back(fullActivity);
        std::cout << "Activity added successfully.\n";
    }

    void removeActivity(const std::string& activity) {
        auto it = std::find(activityList.begin(), activityList.end(), activity);
        if (it != activityList.end()) {
            activityList.erase(it);
            std::cout << "Activity removed successfully.\n";
        } else {
            std::cout << "Activity not found in the list.\n";
        }
    }

    void displayActivityList() {
        std::cout << "\n---- Activity List ----\n";
        if (activityList.empty()) {
            std::cout << "The activity list is empty.\n";
        } else {
            for (const auto& activity : activityList) {
                std::cout << "- " << activity << std::endl;
            }
        }
    }

    void clearActivityList() {
        activityList.clear();
        std::cout << "Activity list cleared.\n";
    }

    void saveActivityListToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& activity : activityList) {
                file << activity << "\n";
            }
            std::cout << "Activity list saved to file: " << filename << std::endl;
        } else {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
        }
        file.close();
    }

    void loadActivityListFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                activityList.push_back(line);
            }
            std::cout << "Activity list loaded from file: " << filename << std::endl;
        } else {
            std::cerr << "Unable to open file for reading: " << filename << std::endl;
        }
        file.close();
    }

    std::string computeDate(const std::string& baseDate, int daysDifference) {
        std::tm timeinfo = {};
        std::istringstream ss(baseDate);

        // Check if the date format is valid
        if (!(ss >> std::get_time(&timeinfo, "%Y-%m-%d")) || !ss.eof()) {
            return "Invalid date format. Please enter a valid date in the format YYYY-MM-DD.";
        }

        std::time_t baseTime = std::mktime(&timeinfo);

        if (baseTime == -1) {
            return "Invalid base date.";
        }

        baseTime += daysDifference * secondsPerDay; // Convert days to seconds and add to base time

        timeinfo = *std::localtime(&baseTime);
        std::stringstream result;
        result << std::put_time(&timeinfo, "%Y-%m-%d");
        return result.str();
    }

    std::string calculateAge(const std::string& birthDate) {
        std::tm timeinfo = {};
        std::istringstream ss(birthDate);

        // Check if the date format is valid
        if (!(ss >> std::get_time(&timeinfo, "%Y-%m-%d")) || !ss.eof()) {
            return "Invalid birth date format.";
        }

        std::time_t birthTime = std::mktime(&timeinfo);
        if (birthTime == -1) {
            return "Invalid birth date format.";
        }

        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        currentTime += 8 * 60 * 60; // Adjust current time to Philippine Timezone (UTC+8)
        
        std::tm* currentTimeInfo = std::localtime(&currentTime);
        int years = currentTimeInfo->tm_year - timeinfo.tm_year;
        int months = currentTimeInfo->tm_mon - timeinfo.tm_mon;
        int days = currentTimeInfo->tm_mday - timeinfo.tm_mday;

        if (months < 0 || (months == 0 && days < 0)) {
            --years;
        }

        std::stringstream result;
        result << "Age: " << years << " years";
        return result.str();
    }

    int calculateDaysBetween(const std::string& startDate, const std::string& endDate) {
        std::tm startTimeinfo = {};
        std::tm endTimeinfo = {};
        std::istringstream startSS(startDate);
        std::istringstream endSS(endDate);

        startSS >> std::get_time(&startTimeinfo, "%Y-%m-%d");
        endSS >> std::get_time(&endTimeinfo, "%Y-%m-%d");

        std::time_t startTime = std::mktime(&startTimeinfo);
        std::time_t endTime = std::mktime(&endTimeinfo);

        if (startTime == -1 || endTime == -1) {
            return -1; // Error occurred while parsing dates
        }

        const int secondsPerDay = 60 * 60 * 24;
        int difference = std::difftime(endTime, startTime) / secondsPerDay;

        return std::abs(difference); // Return absolute difference in days
    }

    void displayCalendar(int month, int year) {
        int dayOfMonth = 1;
        std::cout << "---------------------\n";
        std::cout << "     " << getMonthName(month) << " " << year << "\n";
        std::cout << "---------------------\n";
        std::cout << " Sun Mon Tue Wed Thu Fri Sat\n";

        int startDay = getStartDayOfWeek(month, year);
        int totalDays = getTotalDays(month, year);

        for (int i = 0; i < startDay; ++i) {
            std::cout << "    ";
        }

        for (int day = 1; day <= totalDays; ++day) {
            std::cout << std::setw(4) << day;

            if ((startDay + day) % 7 == 0 || day == totalDays) {
                std::cout << "\n";
            } else {
                std::cout << " ";
            }
        }
    }

private:
    static int getStartDayOfWeek(int month, int year) {
        std::tm timeinfo = { 0, 0, 0, 1, month - 1, year - 1900 };
        std::mktime(&timeinfo);
        return timeinfo.tm_wday;
    }

    static int getTotalDays(int month, int year) {
        if (month == 2) {
            return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
        } else if (month == 4 || month == 6 || month == 9 || month == 11) {
            return 30;
        } else {
            return 31;
        }
    }

    static std::string getMonthName(int month) {
        static const std::string monthNames[] = {
            "January", "February", "March", "April", "May", "June", "July",
            "August", "September", "October", "November", "December"
        };
        return monthNames[month - 1];
    }
};

int main() {
    CalendarOrganizer calendar;
    int choice;

    do {
        std::cout << "\n---- Calendar Organizer Menu ----\n";
        std::cout << "1. Activities\n";
        std::cout << "2. Show Calendar\n";
        std::cout << "3. Calculate Date After a Certain Number of Days\n";
        std::cout << "4. Calculate Date Before a Certain Number of Days\n";
        std::cout << "5. Computation of Age\n";
        std::cout << "6. Calculate Days Between Dates\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number.\n";
            continue; // Restart the loop
        }

        switch (choice) {
            case 1: {
    int activityChoice;
    do {
        std::cout << "\n---- Activity List Menu ----\n";
        std::cout << "1. Add Activity\n";
        std::cout << "2. Remove Activity\n";
        std::cout << "3. View Activity List\n";
        std::cout << "4. Clear Activity List\n";
        std::cout << "5. Save Activity List to File\n";
        std::cout << "6. Load Activity List from File\n";
        std::cout << "0. Back\n";
        std::cout << "Enter your choice: ";
        std::cin >> activityChoice;

        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number.\n";
            continue; // Restart the loop
        }

        switch (activityChoice) {
            case 0:
                std::cout << "Going back to the main menu.\n";
                break;

           case 1: {
    std::string activity, date;

    // Clear the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the activity: ";
    std::getline(std::cin, activity);

    while (true) {
        std::cout << "Enter the date (YYYY-MM-DD): ";
        std::cin >> date;

        // Check if the date format is valid
        if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
            std::cout << "Invalid date format. Please enter a valid date in the format YYYY-MM-DD.\n";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue; // Restart the loop
        }

        break; // Exit the loop if a valid date is entered
    }

    calendar.addActivityWithDate(activity, date);
    break;
}

            case 2: {
                std::string activityToRemove;
                std::cout << "Enter the activity to remove: ";
                std::cin.ignore();
                std::getline(std::cin, activityToRemove);
                calendar.removeActivity(activityToRemove);
                break;
            }

            case 3:
                calendar.displayActivityList();
                break;

            case 4:
                calendar.clearActivityList();
                break;

            case 5: {
                std::string filename;
                std::cout << "Enter the filename to save: ";
                std::cin >> filename;
                calendar.saveActivityListToFile(filename);
                break;
            }

            case 6: {
                std::string filename;
                std::cout << "Enter the filename to load: ";
                std::cin >> filename;
                calendar.loadActivityListFromFile(filename);
                break;
            }

            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }
    } while (activityChoice != 0);
    break;
}

           case 2: {
    int month, year;
    std::string monthInput, yearInput;

    while (true) {
    std::cout << "Enter month and year (MM YYYY) to display calendar: ";

    // Clear the input bufferaa
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Read the entire line as a string
    std::string dateInput;
    std::getline(std::cin, dateInput);

    std::istringstream dateStream(dateInput);

    try {
        // Extract month and year from the string
        dateStream >> month >> year;

        // Check for valid ranges
        if (dateStream.fail() || (month < 1 || month > 12) || year < 1900) {
            throw std::invalid_argument("Invalid input");
        }

        break; // Exit the loop if extraction and validation are successful
    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid input. Please enter valid numbers for month and year.\n";
    }
}

    calendar.displayCalendar(month, year);
    break;
}

            case 3: {
    std::string dateInput;
    int daysAfter;

    while (true) {
        std::cout << "Enter the base date (YYYY-MM-DD): ";
        std::cin >> dateInput;

        // Check if the date format is valid
        if (dateInput.size() != 10 || dateInput[4] != '-' || dateInput[7] != '-') {
            std::cout << "Invalid date format. Please enter a valid date in the format YYYY-MM-DD.\n";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue; // Restart the loop
        }

        break; // Exit the loop if a valid date is entered
    }

    std::cout << "Enter the number of days after: ";
    std::cin >> daysAfter;

    if (std::cin.fail()) {
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cout << "Invalid input. Please enter a number.\n";
        break; // Exit the switch statement
    }

    std::cout << "Date after " << daysAfter << " days: " << calendar.computeDate(dateInput, daysAfter) << std::endl;
    break;
}

            case 4: {
    std::string dateInput;
    int daysBefore;

    while (true) {
        std::cout << "Enter the base date (YYYY-MM-DD): ";
        std::cin >> dateInput;

        // Check if the date format is valid
        if (dateInput.size() != 10 || dateInput[4] != '-' || dateInput[7] != '-') {
            std::cout << "Invalid date format. Please enter a valid date in the format YYYY-MM-DD.\n";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue; // Restart the loop
        }

        break; // Exit the loop if a valid date is entered
    }

    std::cout << "Enter the number of days before: ";
    std::cin >> daysBefore;

    if (std::cin.fail()) {
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cout << "Invalid input. Please enter a number.\n";
        break; // Exit the switch statement
    }

    std::cout << "Date before " << daysBefore << " days: " << calendar.computeDate(dateInput, -daysBefore) << std::endl;
    break;
}

            case 5: {
    std::string birthDate;

    while (true) {
        std::cout << "Enter your birth date (YYYY-MM-DD): ";
        std::cin >> birthDate;

        // Check if the date format is valid
        if (birthDate.size() != 10 || birthDate[4] != '-' || birthDate[7] != '-') {
            std::cout << "Invalid date format. Please enter a valid date in the format YYYY-MM-DD.\n";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue; // Restart the loop
        }

        break; // Exit the loop if a valid date is entered
    }

    std::cout << calendar.calculateAge(birthDate) << std::endl;
    break;
}

            case 6: {
    std::string startDate, endDate;

    while (true) {
        std::cout << "Enter the start date (YYYY-MM-DD): ";
        std::cin >> startDate;

        // Check if the date format is valid
        if (startDate.size() != 10 || startDate[4] != '-' || startDate[7] != '-') {
            std::cout << "Invalid date format. Please enter a valid start date in the format YYYY-MM-DD.\n";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue; // Restart the loop
        }

        break; // Exit the loop if a valid start date is entered
    }

    while (true) {
        std::cout << "Enter the end date (YYYY-MM-DD): ";
        std::cin >> endDate;

        // Check if the date format is valid
        if (endDate.size() != 10 || endDate[4] != '-' || endDate[7] != '-') {
            std::cout << "Invalid date format. Please enter a valid end date in the format YYYY-MM-DD.\n";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue; // Restart the loop
        }

        break; // Exit the loop if a valid end date is entered
    }

    int daysBetween = calendar.calculateDaysBetween(startDate, endDate);

    if (daysBetween >= 0) {
        std::cout << "Days between the two dates: " << daysBetween << std::endl;
    } else {
        std::cout << "Error calculating days between dates.\n";
    }
    break;
}
            case 7:
                std::cout << "Exiting the Calendar Organizer. Goodbye!\n";
                break;

            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }
    } while (choice != 7);

    return 0;
}