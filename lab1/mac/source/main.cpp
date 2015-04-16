#include "windowsetup.hpp"
#include "gamestate.hpp"

int main(int argc, const char* argv[]) {
   GLFWwindow* window;

   // Test this test that
    printf("It's at %s btw\n", *argv);

   window = setupWindow();

   if (window == NULL) {
      printf("Window was null\n");
      return 1;
   }

	GameState gameState = *new GameState(window);
	while(window) {
		gameState.update();
		gameState.draw();
	}
}
