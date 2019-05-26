//********************************************************
//
// Helicopter Project
// Authors:  Zoren, Jaime, Ellis
// Last modified:   20/03/2019
//
// This is the main source file for running the heli rigs.
// This main.c calls the kernel to run and the rest is up to
// the user!! Nice and simple, Bob's your Uncle. I can assure
// all comments are thorough and necessary ie not just put in
// for adding a comment.
//
// Enjoy the song lyrics that best describes being an edgy teen/early adult
//
//********************************************************

//*************************************************************************
//
// Yeah, I'm gonna take my horse to the old town road
// I'm gonna ride 'til I can't no more
// I'm gonna take my horse to the old town road
// I'm gonna ride 'til I can't no more (Kio, Kio)
//
//*************************************************************************

// Of course it is vital to include the kernel header file!!
#include "kernel.h"

int main(void)
{
    // To make the Tiva board function, all the peripherals need to be set up.
    // This function will initiate all the interrupts etc.
    startThisPuppyUp ();

    while (1)
    {
        // All controls are given to the kernel.
        // The kernel controls every process on the Tiva (background and foreground)
        runTheKernelPorFavor ();
    }
}
