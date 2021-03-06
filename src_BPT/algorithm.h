//
// Created by Rainy Memory on 2021/3/6.
//

#ifndef CODE_ALGORITHM_H
#define CODE_ALGORITHM_H

namespace RainyMemory{
    template<class ptr, class T>
    ptr lower_bound(ptr first, ptr last, const T &val) {
        ptr now;
        int count = last - first, step;
        while (count > 0) {
            step = count >> 1;
            now = first + step;
            if (*now < val) {
                first = ++now;
                count -= step + 1;
            }
            else count = step;
        }
        return first;
    }
    
    template<class ptr, class T>
    ptr upper_bound(ptr first, ptr last, const T &val) {
        ptr now;
        int count = last - first, step;
        while (count > 0) {
            step = count >> 1;
            now = first + step;
            if (!(val < *now)) {
                first = ++now;
                count -= step + 1;
            }
            else count = step;
        }
        return first;
    }
    
}

#endif //CODE_ALGORITHM_H
