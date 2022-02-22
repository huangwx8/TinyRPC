#include <example/NameService.hh>

static int get_belonged_bucket(int k)
{
    if (k < 200000)
    {
        return 0;
    }
    else if (k < 400000)
    {
        return 1;
    }
    else if (k < 600000)
    {
        return 2;
    }
    else if (k < 800000)
    {
        return 3;
    }
    else 
    {
        return 4;
    }
}

std::string NameServiceBase::GetNameInternal(int k)
{
    int index = get_belonged_bucket(k);
    std::string name;

    dict_locks[index].lock();
    auto it = dicts[index].find(k);
    if (it != dicts[index].end()) {
        name = it->second;
    }
    else {
        name = "not found";
    }
    dict_locks[index].unlock();

    return name;
}

int NameServiceBase::SetNameInternal(int k, std::string v)
{
    int index = get_belonged_bucket(k);
    int ret = 0;

    dict_locks[index].lock();
    auto it = dicts[index].find(k);
    if (it != dicts[index].end()) {
        it->second = v;
        ret = 1;
    }
    else {
        dicts[index][k] = v;
        ret = 2;
    }
    dict_locks[index].unlock();

    return ret;
}
