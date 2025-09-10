#include <QCoreApplication>
#include <QScxmlStateMachine>
#include <QDebug>
#include <iostream>
#include <thread>
#include <chrono>
#include "SecondTutorialSkill.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  SecondTutorialSkill stateMachine("SecondTutorial");
  stateMachine.start(argc, argv);

  int ret=app.exec();
  
  return ret;
}

