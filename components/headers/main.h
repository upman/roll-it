#ifndef MAIN_H_
#define MAIN_H_
  void resetWorld();
  void menu();
  void createMenu();
  void renderWorld();
  void renderBackground();
  void display();
  void keyboard(unsigned char, int, int);
  void mouse(int, int, int, int);
  void motion(int x, int y);
  void step();
  void init();
  void switchToSimulation();
  void switchToIntro();
  void introKeyboard(unsigned char, int, int);
  void scaleSelectedBody(float);
#endif
