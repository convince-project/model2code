#include <QCoreApplication>
#include <QScxmlStateMachine>
#include <QDebug>
#include <iostream>
#include <thread>
#include <chrono>
#include "GoToCurrentPoiSkill.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  GoToCurrentPoiSkill stateMachine("GoToCurrentPoi");
  stateMachine.start(argc, argv);

  int ret=app.exec();
  
  return ret;
}

