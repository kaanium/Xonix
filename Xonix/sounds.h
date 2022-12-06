#pragma once
#include <string>

static std::wstring SOUNDS_PATH;

void replaceAll(std::wstring& str)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(L"\\", start_pos)) !=
           std::wstring::npos) {
        str.replace(start_pos, 1, L"\\\\");
        start_pos += 2;
    }
}

std::wstring currentPath()
{
    std::wstring buffer;
    buffer.resize(MAX_PATH, 0);
    const auto path_size(
        GetModuleFileName(nullptr, &buffer.front(), MAX_PATH));
    buffer.resize(path_size);
    std::wstring::size_type pos =
        std::wstring(buffer).find_last_of(L"\\/");
    std::wstring t = std::wstring(buffer).substr(0, pos);
    replaceAll(t);
    return t;
}

void SetSoundPath() { SOUNDS_PATH = currentPath() + L"\\\\sounds\\\\"; }

void MyPlaySound(std::wstring name, std::wstring volume)
{
    mciSendString((L"close " + SOUNDS_PATH + name).c_str(), NULL, 0, NULL);
    mciSendString((L"open " + SOUNDS_PATH + name).c_str(), NULL, 0, NULL);
    mciSendString(
        (L"setaudio " + SOUNDS_PATH + name + L" volume to " + volume)
            .c_str(),
        NULL, 0, NULL);
    mciSendString((L"play " + SOUNDS_PATH + name).c_str(), NULL, 0, NULL);
}

void PlayMusic(std::wstring x)
{
    mciSendString((L"close " + SOUNDS_PATH + x).c_str(), NULL, 0, NULL);
    mciSendString((L"open " + SOUNDS_PATH + x).c_str(), NULL, 0, NULL);
    mciSendString(
        (L"setaudio " + SOUNDS_PATH + x + L" volume to 30").c_str(), NULL,
        0, NULL);
    mciSendString((L"play " + SOUNDS_PATH + x + L" repeat").c_str(), NULL,
                  0, NULL);
}