#include <iostream>
#include <CGAL/config.h>

int main()
{
    std::cout << "My CGAL library is " <<  CGAL_VERSION_NR << " (1MMmmb1000)\n";
    std::cout << "where MM is the major number release, mm is the minor number release, and "
              << "b is the bug fixing number release.\n";
    return 0;
}
