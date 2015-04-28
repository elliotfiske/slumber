#include "windowsetup.hpp"
#include "gamestate.hpp"

using namespace std;

GameState *gameState;

void checkServer() {
   int currPossession = 0;
   while (1) {
      int newPossession = system("python client.py") / 256;
      printf("Possessing clock #%d\n", newPossession);

      if (currPossession != newPossession) {
         currPossession = newPossession;
         gameState->bed->ambientColor.x = (float) currPossession / 10.0;
      }
   }
}

int main(int argc, const char* argv[]) {
   GLFWwindow* window;

   // Test this test that
    printf("It's at %s btw\n", *argv);

   window = setupWindow();

   if (window == NULL) {
      printf("Window was null\n");
      return 1;
   }

	gameState = new GameState(window);
	while(window) {
		gameState->update();
		gameState->draw();
	}
   
}
