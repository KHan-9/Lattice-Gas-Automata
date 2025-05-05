#include"CelluarAutomata.h"

int main()
{  
    CelluarAutomata a(200, 200);
    a.setup();
   a.fill(5000, 1,199,1,49);
    a.display();

    return 0;
}