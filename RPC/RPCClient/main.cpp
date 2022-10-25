#include "Menu.h"


int main() {
	Menu menu;
	if (!menu.start())
		return 0;
	while (1)
		menu.typeCommand();
}