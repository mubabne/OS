#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main(void)
{
    char sourceName[256];
    char destinationName[256];

    printf("Enter source file: ");
    scanf("%255s", sourceName);

    printf("Enter destination file: ");
    scanf("%255s", destinationName);

    HANDLE sourceFile = CreateFileA(
        sourceName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (sourceFile == INVALID_HANDLE_VALUE)
    {
        DWORD errorCode = GetLastError();

        printf("Error opening source file. Error code: %lu\n", errorCode);
        return 1;
    }

    HANDLE destinationFile = CreateFileA(
        destinationName,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (destinationFile == INVALID_HANDLE_VALUE)
    {
        DWORD errorCode = GetLastError();

        printf("Error creating destination file. Error code: %lu\n", errorCode);

        CloseHandle(sourceFile);
        return 1;
    }

    char buffer[BUFFER_SIZE];

    DWORD bytesRead;
    DWORD bytesWritten;

    while (1)
    {
        BOOL readResult = ReadFile(
            sourceFile,
            buffer,
            BUFFER_SIZE,
            &bytesRead,
            NULL
        );

        if (!readResult)
        {
            DWORD errorCode = GetLastError();

            printf("Error reading source file. Error code: %lu\n", errorCode);

            CloseHandle(sourceFile);
            CloseHandle(destinationFile);

            return 1;
        }

        if (bytesRead == 0)
        {
            break;
        }

        DWORD totalWritten = 0;

        while (totalWritten < bytesRead)
        {
            BOOL writeResult = WriteFile(
                destinationFile,
                buffer + totalWritten,
                bytesRead - totalWritten,
                &bytesWritten,
                NULL
            );

            if (!writeResult)
            {
                DWORD errorCode = GetLastError();

                printf("Error writing destination file. Error code: %lu\n", errorCode);

                CloseHandle(sourceFile);
                CloseHandle(destinationFile);

                return 1;
            }

            totalWritten += bytesWritten;
        }
    }

    if (!CloseHandle(sourceFile))
    {
        DWORD errorCode = GetLastError();

        printf("Error closing source file. Error code: %lu\n", errorCode);
    }

    if (!CloseHandle(destinationFile))
    {
        DWORD errorCode = GetLastError();

        printf("Error closing destination file. Error code: %lu\n", errorCode);

        return 1;
    }

    printf("File copied successfully.\n");

    return 0;
}