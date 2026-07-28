# Luftschloss

> Luftschloss (German) - literally "air castle". A castle built in the sky, on
> nothing. The word for a daydream you have no business chasing, a grand plan
> with no foundation under it. You build it anyway.

A Discord bot written in C++, built to automate the grind of studying for
competitive programming.

## Origin

The name is a confession. Luftschloss is what the Germans call the palace you
raise in thin air - all spires and no ground beneath it. That is the honest
shape of this project. A Discord bot has no reason to be written in C++. It has
even less reason to reach for hand-written assembly, to eye Ada across the
border, to dream in RISC-V on a machine that speaks x86. None of it is
necessary. All of it is the point.

So the castle goes up in the air. The foundation can come later, or never. What
matters is that it is being built, one impractical stone at a time, and that it
does something real in the end - drills problems, tracks streaks, and drags a
student toward the next contest whether they feel like it or not.

Every overbuilt corner of this repo is load-bearing for exactly one thing: the
vibe. That is enough.

## What it does

The plan, such as it is:

- Pull problems from the usual judges by rating and tag
- Announce upcoming contests in a channel
- Track solved problems, streaks, and progress over time
- Serve a problem sized to your current level, on command

## Build

Install the DPP Discord library once, then build. The Makefile handles the
dependency for you:

    make

Run it:

    make run

Parallel compile jobs are capped so the build cannot exhaust memory. Raise the
cap if your machine can take it:

    make JOBS=4

Requires a C++20 compiler, CMake 3.16 or newer, and DPP. On Arch the Makefile
installs DPP from the AUR through yay or paru if it is missing.

## Layout

- main.cpp - entry point
- src/ - bot sources
- lib/ - vendored dependencies
