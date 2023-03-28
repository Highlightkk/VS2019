#include"Demo01.h"
#define GPIOA_BASE (0x00000800U)

int main(void)
{

	unsigned int cnt=65531;
	cnt &= (0xFFFE << 0);
	printf("%d", GPIOA_BASE);
	return 0;
}