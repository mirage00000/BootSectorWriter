#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Windows.h>

int main() {
    std::string filePath;
    int driveNumber;

    // Get the file path from the user
    std::cout << "Enter the path to the binary file (absolute or relative): ";
    std::getline(std::cin, filePath);

    // Get the drive number from the user
    std::cout << "Enter the drive number (e.g., 0 for PhysicalDrive0, the current system drive): ";
    std::cin >> driveNumber;

    // Open the binary file for reading
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open the file." << std::endl;
        getchar();
        return 1;
    }

    // Read hex bytes from the file into a vector
    std::vector<char> bytes(std::istreambuf_iterator<char>{file}, {});

    // Close the file
    file.close();

    // Construct the drive path based on user input
    std::wstring drivePath = L"\\\\.\\PhysicalDrive" + std::to_wstring(driveNumber);

    // Open the physical drive for writing
    HANDLE hDevice = CreateFile(
        drivePath.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open the drive. Error code: " << GetLastError() << std::endl;
        getchar();
        return 1;
    }

    // Write bytes to the drive
    DWORD bytesWritten;
    if (!WriteFile(hDevice, bytes.data(), bytes.size(), &bytesWritten, nullptr)) {
        std::cerr << "Failed to write to the drive. Error code: " << GetLastError() << std::endl;
        CloseHandle(hDevice);
        getchar();
        return 1;
    }

    // Close the handle
    CloseHandle(hDevice);

    std::cout << "Write successful." << std::endl;
    getchar();

    return 0;
}
