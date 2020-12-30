/*
 * main.c
 *
 *  Created on: 2020. 12. 29.
 *      Author: MCES
 */


#include "main.h"



int main(int argc, char *argv[])
{
  hwInit();
  apInit();

  apMain(argc, argv);
}
