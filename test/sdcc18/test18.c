#include <pic18f452.h>

int test = 0;

void main(void)
{
  float fvar = TRISA;
  int a = TRISB;
  LATA = 0xFF;
  LATAbits.LATA1 = 0;
//       UIE = 0x55;
  fvar /= a;
  a = fvar;
  TRISA = a;
  while(1);
}
