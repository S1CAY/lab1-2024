#include <windows.h>
#include <iostream>

void CreateAndManageProcessWithTimeout(const std::wstring& applicationName, DWORD timeout) {
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(STARTUPINFO);

    if (CreateProcessW(
        applicationName.c_str(),
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::wcout << L"Процес успішно створено. PID: " << pi.dwProcessId << std::endl;

        DWORD waitResult = WaitForSingleObject(pi.hProcess, timeout);

        if (waitResult == WAIT_OBJECT_0) {
            DWORD exitCode = 0;
            if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
                std::wcout << L"Процес завершився з кодом: " << exitCode << std::endl;

                if (exitCode == 0) {
                    std::wcout << L"Процес завершився успішно." << std::endl;
                } else {
                    std::wcout << L"Процес завершився з помилкою. Код помилки: " << exitCode << std::endl;
                }
            } else {
                std::wcerr << L"Не вдалося отримати код завершення процесу. Код помилки: " << GetLastError() << std::endl;
            }
        } else if (waitResult == WAIT_TIMEOUT) {
            std::wcout << L"Процес виконується занадто довго. Завершення процесу..." << std::endl;

            if (TerminateProcess(pi.hProcess, 1)) {
                std::wcout << L"Процес був успішно завершений примусово." << std::endl;
            } else {
                std::wcerr << L"Не вдалося завершити процес. Код помилки: " << GetLastError() << std::endl;
            }
        } else {
            std::wcerr << L"Помилка очікування завершення процесу. Код помилки: " << GetLastError() << std::endl;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        std::wcerr << L"Не вдалося створити процес. Код помилки: " << GetLastError() << std::endl;
    }
}

int main() {
    std::wcout << L"Запуск Notepad..." << std::endl;

    std::wstring applicationName = L"C:\\Windows\\System32\\notepad.exe";

    DWORD timeout = 10000;

    CreateAndManageProcessWithTimeout(applicationName, timeout);

    std::wcout << L"Робота завершена." << std::endl;
    return 0;
}
