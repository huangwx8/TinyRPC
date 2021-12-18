#pragma once

#include <vector>

struct Guid
{
public:
    /**
     * @brief Get a guid(global unique identifier), as less as more. 
     * Guid start from 1 to 1024, return -1 for no any guid cound be allocated
     * @return int 
     */
    static int GetGuid();
    /**
     * @brief Recycle a guid that no longer used
     * @return void 
     */
    static void RecycleGuid(int id);
private:
    static std::vector<int> Heap;
    static int Bound;
};
