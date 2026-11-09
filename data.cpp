#include "data.h"
#include <format>

using namespace std;

int Data::get(int i) const
{
    if (i == 0)
    {
        lock_guard lk(mutex0);
        return fields[0];
    }
    lock_guard lk(mutex1);
    return fields[1];
}

void Data::set(int i, int value)
{
    if (i == 0)
    {
        lock_guard lk(mutex0);
        fields[0] = value;
    }
    else
    {
        lock_guard lk(mutex1);
        fields[1] = value;
    }
}

Data::operator std::string() const
{
    int f0, f1;
    {
        lock_guard lk0(mutex0);
        lock_guard lk1(mutex1);
        f0 = fields[0];
        f1 = fields[1];
    }
    return format("{} {}", f0, f1);
}
