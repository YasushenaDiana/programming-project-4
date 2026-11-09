#pragma once

#include <string>
#include <mutex>

class Data
{
public:
    static const int M = 2;

    Data() = default;
    Data(const Data &) = delete;
    Data &operator=(const Data &) = delete;
    ~Data() = default;

    int get(int i) const;
    void set(int i, int value);
    operator std::string() const;

private:
    int fields[M] = {0, 0};
    mutable std::mutex mutex0;
    mutable std::mutex mutex1;
};
